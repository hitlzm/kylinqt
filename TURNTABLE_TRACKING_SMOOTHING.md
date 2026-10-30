# 转台外引导大角度平滑方案

## 1. 目标

图像XXX、激光XXX和 CCD 输出的偏差角是相对视轴的测量值。现有下发
逻辑把它直接叠加到转台当前角上。目标刚进入视场、目标丢失后重新捕获，或者
测量发生较大跳变时，这个叠加值可能形成十几度甚至几十度的大阶跃。转台在
跟踪模式下按绝对角度点运行，大阶跃会使它接近最大速度运行，并带来明显超调。

本方案在转台串口线程内完成绝对目标预测和“绝对目标角整形”：

```text
图像/激光/CCD逐帧相对偏差
    -> 内框滚转坐标变换
    -> 当前转台反馈角 ± 相对偏差 = 绝对目标角
    -> AB 对绝对目标角滤波、预测
    -> TrackingTargetSmoother 做零点消抖和速度/加速度整形
    -> 1 秒四个绝对目标点，或 20 ms 最新测量闭环
```

1 秒模式不再让 AB 预测相对误差。目标静止时，转台反馈角增加会与导引头
相对偏差减小相互抵消，因此 AB 不会把转台自身运动误识别为目标速度。

## 2. 控制规则

单轴目标整形器位于 `utils/tracking_target_smoother.h`。默认参数如下：

| 参数 | 默认值 | 含义 |
| --- | ---: | --- |
| `zeroDeadbandDeg` | 0.004° | 小于该值的偏差视为零，保持上一有效目标 |
| `zeroDeadbandReleaseDeg` | 0.006° | 偏差超过该值后才退出零点保持 |
| `nearTrackRadiusDeg` | 1.0° | 近目标低速区的边界 |
| `nearMaxSpeedDegPerSec` | 1.5°/s | 近目标最大跟踪速度 |
| `nearMaxAccelerationDegPerSec2` | 3.0°/s² | 近目标最大跟踪加速度 |
| `farTrackRadiusDeg` | 3.0° | 近目标到正常跟踪能力的过渡终点 |
| `offsetFilterTimeConstantSec` | 0.04 s | 稳态输入低通时间常数 |
| `offsetFilterFastTimeConstantSec` | 0.02 s | 快速运动时使用的低通时间常数 |
| `offsetFilterFastSlewThresholdDeg` | 0.03° | 新旧测量差超过该值时进入快速滤波 |
| `offsetFilterFastResetDeltaDeg` | 0.50° | 大偏差突然下降时释放旧滤波状态 |
| `offsetFilterSignResetMinDeg` | 0.006° | 仅超过该值的反号测量才绕过普通低通 |
| `offsetFilterResetOnSignChange` | `true` | 测量越过零点时立即释放旧方向 |
| `maxSpeedDegPerSec` | 12.0°/s | 正常跟踪最大速度 |
| `maxAccelerationDegPerSec2` | 10.0°/s² | 正常跟踪最大加速度 |
| `settleDeadbandDeg` | 0.002° | 内部目标位置允许的静态误差 |

规则如下：

1. 偏差不超过 0.004° 时进入零点保持，并冻结上一有效目标。已经进入保持后，
   偏差要达到 0.006° 才退出，以免在量化边界反复启停。
2. 退出保持后，先使用软死区得到有效偏差：
   `effectiveOffset = offset - sign(offset) * zeroDeadbandDeg`。
3. 普通帧间变化使用 0.04 s 低通；新旧测量差超过 0.03° 时切换到 0.02 s
   快速滤波，避免真实机动被过度延迟。
4. 大偏差突然下降超过 0.50°，或目标越过零点导致测量反号时，立即释放旧滤波
   状态，避免滤波器继续沿用已经过期的目标方向。
5. 近目标区使用 1.5°/s、3.0°/s² 的低速低加速度跟踪。
6. 偏差从 1° 增加到 3° 时，速度和加速度上限连续过渡到 12°/s、10°/s²，
   不存在从直通模式切换到斜坡模式的跳跃。
7. 零点迟滞只用于抑制量化噪声；退出后仍从低速连续恢复，不直接跳到完整偏差。
8. 目标反向或变小时，输出仍从上一目标位置连续过渡。
9. 非有限值（NaN/Inf）或无效时间间隔只保持上一目标，不推进转台。

当前项目的角度方向约定为：

```cpp
外框目标 = 外框反馈 - 反滚转后的XXX方位偏差;
中框目标 = 中框反馈 + 反滚转后的XXX俯仰偏差;
```

该符号由当前硬件坐标定义决定。若转台方向标定改变，只需调整
`serialport_turntable_HEX.cpp` 中两处偏差符号，不应在坐标变换函数内修改。

## 3. 两种周期

### 20 ms 模式（0x0B）

每个新的 20 ms 目标时刻调用一次整形器。状态跨帧保存在转台串口工作线程，
实际时间间隔限制在 5～100 ms，串口线程短时卡顿不会一次推进过大的角度。
同一个 20 ms 时间戳只处理一次，重复定时器触发会被丢弃。

XXX数据率高于或低于 20 ms 时，发送端使用最近一次有效测量，但“反馈角 +
当前偏差”的闭环计算仍发生在转台发送时刻。

进入零点死区后，20 ms 定时器仍按协议发送目标帧，但目标保持上一有效值，
因此转台不会响应导引头约 0.002° 的量化抖动。0.01° 对应五个量化台阶。

### 1 秒模式（0x0A）

三路导引头每收到一帧相对测量就发送到转台工作线程。转台线程使用当时的内框
角反滚转，并组合中框、外框反馈形成绝对目标角，然后喂入绝对目标 AB。

1 秒节拍到达时，AB 预测四个绝对目标角，转台线程将预测角转换为相对当前反馈
的偏移，再交给 `preview()` 生成受速度和加速度限制的四个 250 ms 点。

每次新测量都会更新绝对目标航迹；下一包根据最新转台反馈和绝对目标重新生成
轨迹。源切换或测量超时会清除旧航迹，防止旧目标被继续外推。

## 4. 模式切换与安全

- 进入外引导模式、切换外引导源、修改跟踪周期时清除旧斜坡状态，第一拍从
  当前转台反馈起坡。
- 离开外引导模式时先禁止新跟踪帧，并向三轴发送停车指令；随后到达的旧
  0x0A/0x0B 帧会被丢弃，避免停车后又被旧目标重新驱动。
- 大角度斜坡只限制目标轨迹，不替代机械限位、超时看门狗和人工急停。
- 现场参数应由转台实际能力决定。建议最大跟踪速度先取 8～10°/s，确认目标
  最大角速度、超调量和串口时序后再调整。

## 5. 验证

纯算法自测：

```powershell
& 'E:/QT5.12.8/Tools/mingw730_64/bin/g++.exe' `
  -std=c++14 -Wall -Wextra -Werror `
  tests/tracking_target_smoother_selftest.cpp `
  -o build/tracking_target_smoother_selftest.exe

.\build\tracking_target_smoother_selftest.exe
```

带量化噪声、伺服滞后和参数扫描的闭环仿真：

```powershell
& 'E:/QT5.12.8/Tools/mingw730_64/bin/g++.exe' `
  -std=c++14 -O2 -Wall -Wextra -Werror `
  tests/tracking_noise_simulation.cpp `
  -o build/tracking_noise_simulation.exe

.\build\tracking_noise_simulation.exe
```

绝对目标 AB 与旧相对误差 AB 的对照测试：

```powershell
& 'E:/QT5.12.8/Tools/mingw730_64/bin/g++.exe' `
  -std=c++14 -O2 -Wall -Wextra -Werror `
  -Itests/filter_sim/stub `
  -include tests/filter_sim/stub/preserialport.h `
  tests/absolute_target_ab_sim.cpp kalman/AlphaBetaTracker.cpp `
  -o build/absolute_target_ab_sim.exe

.\build\absolute_target_ab_sim.exe
```

该测试模拟静止 10° 目标。转台接近目标时，旧相对误差 AB 叠加反馈后会出现
明显反向预测；绝对目标 AB 则保持在目标角附近。

建议的台架测试：

1. 静止小偏差 0.5°：目标应低速连续靠近，每 20 ms 的角度增量不超过约
   0.02°，不能直接跳到最终目标。
2. 偏角在 0.00/0.002/0.004° 间跳动：转台绝对目标应保持不动，且不能先
   回追当前反馈角。
3. 偏角从 0.01° 增加到 0.05°：应从零附近连续、低速地恢复跟踪，不能
   跳变到完整偏差。
4. 静止大偏差 20°：转台目标每 20 ms 连续变化，首秒加速，随后限速，不应出现
   从 0° 直接跳到 20° 的帧。
5. 跟踪过程中撤销目标：目标应在零点死区内保持上一有效值，不反向大幅跳变。
6. 1 秒模式输入 20° 固定偏差：四个 250 ms 点应依次增大，相邻点角度差不超过
   速度限制，并且加速度不是无限大。
7. XXX断流与模式切换：应立即停止产生新跟踪目标，不得沿用最后一帧继续追击。
