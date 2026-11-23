# 配置化导引头协议适配层

## 目标

本实现参考 QAmigo 将协议描述与业务代码分离的思路，但不把 JSON 当作任意脚本，
也不把配置直接绑定到 QML 属性。

厂家协议差异不再直接进入 QML 和业务流程。数据链路调整为：

```text
串口字节流
  -> 启动/恢复预置设备：软件内置帧解析器
  -> 用户选择 JSON 后：JSON 分帧、校验、字段解码
     -> CanonicalSeekerFrame（厂家无关中间数据）
  -> LaserData / ImageData
  -> 显示、录制、外引导、转台控制
```

控制命令走独立反向链路：

```text
QML 语义命令
  -> ProtocolCommandRequest
  -> JSON 命令模板、命令码映射、字段编码、校验
  -> 串口帧
```

不采用“解析后重新生成旧软件帧，再让旧解析器解析”的内部闭环。旧
`laser_recv_frame` 和原图像帧解析路径保留为正式启动模式；只有用户主动选择
JSON 协议后，接收链路才切换到配置化解析器。

## 已实现

- JSON 协议配置加载与结构校验。
- 固定长度帧、帧头搜索、半包缓存、粘包拆分和噪声丢弃。
- `xor8`、`sum8`、`CRC16/Modbus`、`CRC16/CCITT` 校验，支持配置初值和存储字节序。
- 有符号/无符号整数、32/64 位整数、浮点数。
- 大小端、比例系数、偏置、单位和位域。
- 枚举值到稳定名称/中文标签的映射。
- 命令模板、物理量到原始值换算、命令码映射和发送校验。
- 激光与图像导引头配置化收发接入。
- 配置加载失败自动回退到原激光解析器。
- Golden self-test，验证解码和命令编码的逐字节兼容性。

## 当前配置文件

当前厂家配置位于：

```text
protocols/legacy_laser_v1.json
protocols/legacy_image_v1.json
```

资源内路径：

```text
:/protocols/legacy_laser_v1.json
```

可以通过环境变量指定默认命令编码配置，便于联调：

```text
KYLIN_LASER_PROTOCOL_PROFILE=E:\protocols\vendor_a_laser.json
KYLIN_IMAGE_PROTOCOL_PROFILE=E:\protocols\vendor_a_image.json
```

未设置环境变量时，默认命令编码配置的加载顺序为：

1. 可执行文件目录下的 `protocols/legacy_laser_v1.json` 或
   `protocols/legacy_image_v1.json`。
2. 编译进程序的对应 `:/protocols/...` 默认配置。

程序启动时，图像和激光接收仍使用软件原有的内置解析器。上述 JSON 仅用于
保持原有控制命令编码；它不会改变启动时的接收格式。

## 运行时切换

图像导引头和激光导引头的“当前使用设备”下拉框选择“其他”时，会弹出协议
文件选择框。用户可选择任意可访问的本地 JSON；该文件无需加入
`protocols.qrc`。用户选定文件并确认后：

1. 工作线程先在临时对象中完整加载和校验 JSON。
2. 校验成功后，同时替换该导引头的接收解码器和命令编码器，并清空旧的半包缓存。
3. 加载失败时继续使用切换前的协议，界面中的设备选择也恢复为原值。

弹窗中点击“退出”不会加载文件，并保持原设备和原协议配置。由“其他”切回
预置设备时，会清空 JSON 解码器并恢复软件原有解析器；控制命令编码则恢复为
默认命令配置。

该文件不存在、JSON 错误或字段越界时，程序会记录诊断信息，并回退到原有
`SerialPortLaser::parseData()`。

## 配置结构

根节点关键字段：

```json
{
  "schemaVersion": 1,
  "id": "vendor-model-protocol-version",
  "source": "laser",
  "capabilities": {},
  "frames": [],
  "commands": {}
}
```

JSON 注释使用保留字段 `_comment`（对象内）或 `_comments`（根节点说明表）。
不要使用 `//` 或 `/* */`，因为 Qt 的标准 `QJsonDocument` 会将其判为非法 JSON。
解析器会忽略命令表和 `commandCodes` 中以 `_` 开头的说明键。

帧配置负责接收：

```json
{
  "name": "status",
  "header": "55 AA DC",
  "length": 48,
  "checksum": {
    "type": "xor8",
    "start": 3,
    "length": 44,
    "storedOffset": 47
  },
  "fields": []
}
```

字段描述支持：

- `offset`：相对帧头起点的字节偏移。
- `type`：`uint8/int8/uint16/int16/uint32/int32/uint64/int64/float32/float64`。
- `endian`：`little` 或 `big`。
- `scale`、`bias`：物理量换算 `physical = raw * scale + bias`。
- `bitOffset`、`bitWidth`：位域提取。
- `canonical`：写入中间模型的稳定路径。
- `unit`：标准单位。
- `enum`：原始值与稳定名称、显示标签的映射。

命令配置负责发送：

```json
{
  "commands": {
    "vendor.control": {
      "template": "55 AA ...",
      "commandCodeField": "commandCode",
      "commandCodes": {
        "lock": 2,
        "circleSearch": 8
      },
      "fields": [],
      "checksum": {}
    }
  }
}
```

模板中的动态字段由物理量写入，编码器负责类型范围、字节序、比例和校验。

## 标准字段约定

协议配置应尽可能映射到以下稳定路径：

| 路径 | 含义 |
|---|---|
| `tracking.state` | 跟踪状态枚举名 |
| `tracking.stateLabel` | 跟踪状态显示文本 |
| `measurement.losAzimuthDeg` | 光轴方位角，度 |
| `measurement.losElevationDeg` | 光轴俯仰角，度 |
| `measurement.deviationAzimuthDeg` | 方位偏差角，度 |
| `measurement.deviationElevationDeg` | 俯仰偏差角，度 |
| `measurement.laserPeriodMs` | 激光周期，毫秒 |
| `time.deviceUtcMs` | 设备 UTC 毫秒时间 |
| `status.detectorSignal` | 标准化探测器状态 |
| `status.faultState` | 标准化故障状态 |
| `status.faultCode` | 厂家故障码 |
| `status.axisFault.*` | 标准化轴系故障位 |

坐标系、正方向、绝对/相对角、时间基准仍必须由协议配置和厂家文档明确。JSON
只能完成编码转换，不能自动推断物理语义。

## 新增厂家

1. 根据厂家协议新增 `protocols/vendor_x_laser_v1.json`。
2. 保留唯一的 `id`、`source` 和 `schemaVersion=1`。
3. 描述分帧、校验、字段偏移、字节序、比例和位域。
4. 把厂家字段映射到标准路径；特殊字段可保留在 `rawFields`。
5. 描述每个控制命令的模板和参数映射。
6. 准备至少一组真实接收帧和一组期望发送帧。
7. 运行 `protocol_selftest`，并增加厂家专属 golden fixture。
8. 设置 `KYLIN_LASER_PROTOCOL_PROFILE` 联调，确认无误后再设为默认配置。

如果厂家协议包含动态 TLV、加密、压缩或复杂状态机，应增加一个受控的 C++
协议插件，而不是在 JSON 中发明通用脚本语言。

## 后续迁移

图像导引头已经复用同一套协议配置、解码器和编码器：156 字节状态帧和 224 字节
控制帧均来自 `legacy_image_v1.json`。下一步如需支持动态长度或同一配置内多帧
类型，再扩展帧长度表达式与 ID 选择器；QML 应继续向语义属性迁移，不直接使用
厂家位掩码和协议码。
