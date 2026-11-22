from __future__ import annotations

from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH

from docx_common import (
    keep_table_together,
    keep_heading_relationships,
    replace_table,
    set_paragraph_text,
    set_update_fields,
)


BASE = Path(__file__).resolve().parent
SOURCE = BASE / "15_source.docx"
OUTPUT = BASE / "15 DD-GF试验系统软件-计算机编程手册_修改版.docx"

doc = Document(SOURCE)
p = doc.paragraphs

set_paragraph_text(p[20], "文档版本：V1.1")
doc.tables[0].rows[2].cells[3].paragraphs[0].text = "V1.1"
doc.tables[2].rows[9].cells[1].paragraphs[0].text = "V1.1"

set_paragraph_text(p[28], "本文件依据 GJB 438C-2021《军用软件开发文档通用要求》附录 R 规定的《计算机编程手册》正文格式编写，说明本工程的实际技术栈、构建环境、目录与模块、主要输入输出接口、示例代码和排故方法，供维护、修改、接口适配和功能扩展人员使用。")
set_paragraph_text(p[31], "本文档以当前仓库中的 C++/QML 源码、CMakeLists.txt、.vscode/settings.json 和现有构建产物为依据。文中区分“交付目标平台”和“当前源码构建环境”：交付目标按项目资料执行，当前仓库的已验证构建为 Windows x86_64、MinGW 7.3.0、Qt 5.12.8；目标 Kylin/AArch64 构建须重新配置并复测。")
set_paragraph_text(p[34], "同批文档中的需求条目、接口和单元标识按已批准版本引用；若个别参数不一致，以最新评审确认版本和当前源码为准。")

set_paragraph_text(p[60], "本文档为 DD/GF 试验系统软件（以下简称“本软件”或“本 CSCI”）的计算机编程手册，规定当前工程的编程环境、源码组织、主要框架、构建方法、输入输出接口、典型实现和诊断手段。本软件及本文档标识见表 1。")
set_paragraph_text(p[63], "本软件由 C++14、Qt 5 和 QML 2.12 实现，构建系统为 CMake。视频显示与录像依赖 libmpv，图像处理依赖 OpenCV 4.2；ONNX Runtime 为可选依赖，未找到兼容库时 CMake 编译 onnxyolov5detector_stub.cpp，YOLO 推理不可用。当前仓库已生成 Windows x86_64 开发构建，目标平台部署构建需单独验证。")

set_paragraph_text(p[66], "1. 计算机系统的用途")
set_paragraph_text(p[66], "本软件运行于方舱上位机，完成图像导引头和激光导引头数据接收、CCD 控制、北斗时间/位置接收、三轴转台控制、六自由度平台 UDP 控制、视频显示与录制，以及三路结构化数据保存。界面和硬件访问由同一桌面进程承担，不设置独立服务端。")
set_paragraph_text(p[68], "2. 本软件在系统中的位置")
set_paragraph_text(p[68], "主程序 main.cpp 创建数据对象、串口工作对象、模式控制器、数据记录器、认证与日志对象，并通过 QQmlApplicationEngine 加载 main.qml。登录完成后才启动串口工作线程、手柄线程、视频处理线程和硬件初始化。")
set_paragraph_text(p[70], "3. 编程工作的范围")
set_paragraph_text(p[70], "维护工作集中在 main.cpp、components/*.qml、serialport/*、network/SixDofMotionClient.*、vlcvideo/*、opencv/*、record/DataRecorder.*、ModeControl/*、kalman/*、utils/*、auth/* 和 log/*。本文档不覆盖操作系统内核、串口驱动和设备固件开发。")

set_paragraph_text(p[72], "本文档用于指导程序人员阅读、修改、构建、调试和回归验证本软件，也可作为接口适配和交付构建的检查依据。")
set_paragraph_text(p[73], "本文档共 5 章：第 1 章说明标识和系统范围；第 2 章列出引用资料；第 3 章说明实际构建环境和构建流程；第 4 章说明编程特征、接口、示例和错误诊断；第 5 章为注释。")
set_paragraph_text(p[74], "GJB 438C-2021 附录 R 面向使用计算机系统进行编程的人员。本软件为应用层工程，不包含手写汇编、操作系统内核或设备驱动源码，因此手册以实际使用的 C++14、Qt、QML、CMake、libmpv、OpenCV 和平台接口为主体；AArch64 指令与调用约定仅作为交付目标平台的阅读参考。")
set_paragraph_text(p[75], "本文件密级为 SS。文中代码示例为构造示例，不包含实际任务参数、口令或涉密算法；配置地址、端口和模型路径均为示例。")

set_paragraph_text(p[77], "下列资料中的版本或接口约定通过引用成为本文档的工作依据。工程未固化、未随源码交付的厂商资料不得用推定内容替代。")
set_paragraph_text(p[80], "当前仓库未包含飞腾处理器优化指南、Qt 商业授权资料或设备厂商限制发放文档。程序人员需要查阅时，应通过项目资料管理员或厂商授权渠道获取，不得从公共网络下载后直接用于交付。")

set_paragraph_text(p[83], "本软件存在交付目标环境和当前源码构建环境两套口径。项目资料声明的交付目标为飞腾腾锐 D2000、银河麒麟桌面操作系统 V10 国防版；当前仓库的 CMakeCache.txt 和 .vscode/settings.json 固化的是 Windows x86_64、MinGW 7.3.0 和 Qt 5.12.8 MinGW 套件。两者不能互相替代，跨平台代码须在目标环境重新构建和复测。")
set_paragraph_text(p[84], "表 3  运行与开发环境配置")
set_paragraph_text(p[88], "源码按功能目录组织，所有 C++ 源文件编译进单一可执行目标 kylinqt；当前工程没有拆分为多个动态业务库。QML 和 TaoQuick 资源通过 Qt 资源系统和 import 路径加载。目录与产物见表 5。")

set_paragraph_text(p[92], "本节按附录 R 3.2 条列出与本工程有关的操作特性。下列性质以交付目标平台为主，当前 Windows/MinGW 开发构建用于编码、界面和多数功能验证；凡涉及时序、字节序、线程调度或图形后端的内容，必须在最终目标平台上复测。")
set_paragraph_text(p[93], "a) 时钟周期：项目资料采用 2.3～2.6 GHz 量级的处理器。软件不依赖固定周期估算业务时间，超时和周期由 QTimer、QElapsedTimer、设备返回时标和 QDateTime 共同构成。时延测试工具位于 utils/，通过实测而非指令周期推算。")
set_paragraph_text(p[94], "b) 字长：交付目标为 64 位 AArch64，当前开发构建为 64 位 x86_64。协议字段和位操作应使用 Qt 的 qint8/quint8、qint16/quint16、qint32/quint32、qint64/quint64，或 <cstdint> 定长类型；不得用 long 表示外部协议宽度。")
set_paragraph_text(p[95], "c) 内存：项目资料中的上位机内存为 8 GB 量级。主要动态占用来自 libmpv 解码、OpenCV 图像处理、视频帧副本和保存缓冲。当前代码没有统一的进程内存上限，排查内存问题时应结合任务管理器、top/ps 和 qDebug 输出。")
set_paragraph_text(p[96], "d) 指令集：交付目标为 ARMv8-A AArch64，当前开发机为 x86_64。源码没有汇编文件，业务代码依赖编译器生成目标指令。NEON/ASIMD、缓存行和 AArch64 寄存器内容只适用于目标构建，不应用于推断当前 Windows 构建的代码行为。")
set_paragraph_text(p[97], "e) 中断能力：应用不直接处理硬件中断，也不注册设备中断服务程序。串口通过 QSerialPort::readyRead 进入 Qt 事件循环，UDP 通过 QUdpSocket::readyRead 处理，视频事件由 libmpv 唤醒并在主线程事件循环中分派。")
set_paragraph_text(p[98], "f) 操作方式：软件为交互式桌面程序，启动时先显示登录页；认证成功后由 QML 调用 beginStartup，main.cpp 才启动硬件线程。运行期包含串口工作线程、手柄线程、视频处理线程以及主线程中的模式控制、UDP、视频显示和记录对象。")
set_paragraph_text(p[99], "g) 操作寄存器：应用运行在用户态，不直接访问系统寄存器。设备状态通过 QSerialPort、QUdpSocket、libmpv API 和文件系统获取，不需要手工维护浮点状态寄存器。")
set_paragraph_text(p[100], "h) 错误指示：Qt 对象通过 errorOccurred、portError、popupMessage、lastError 等信号报告错误；qDebug/qWarning 写入终端或开发环境输出。工程没有统一的数字错误码表，界面提示和数据层错误需结合信号名称、串口错误串和 qDebug 上下文定位。SIGSEGV 等进程级异常由操作系统处理，当前代码不安装自定义信号处理函数。")
set_paragraph_text(p[101], "i) 输入输出：串口设备通过 Qt Serial Port 访问；六自由度平台通过 QUdpSocket 广播收发；视频通过 libmpv 打开 RTSP、UDP 或其他 mpv 支持的 URL；数据通过 QFile 写入；QML 与 C++ 通过上下文属性、Q_PROPERTY 和信号槽交互。")
set_paragraph_text(p[102], "j) 特殊特征：协议字节序并不统一。图像接收帧按代码中的结构体和 CRC16 处理，激光帧使用 0x55 0xAA 0xDC 帧头、定长结构和异或校验，转台帧使用 0x55 0xAA 帧头、29 字节定长结构及 bytes 2～26 累加和。修改协议代码时必须逐字段核对字节偏移，不得假设三类设备共用一种编码。")

set_paragraph_text(p[104], "a) 编译与汇编：源码不含 .s/.S 汇编文件，C++14 和 QML 由编译器及 Qt 工具链处理。反汇编只用于排查编译器输出、协议结构布局和崩溃地址，不作为发布源码的组成部分。")
set_paragraph_text(p[105], "b) 编译程序与版本：当前仓库验证工具链为 MinGW-W64 g++ 7.3.0，语言参数为 -std=gnu++14，CMake 最低版本 3.16，生成器为 MinGW Makefiles。交付目标 AArch64 的编译器、sysroot 和 Qt 套件在仓库中尚未固化，须由交付构建环境提供。")
set_paragraph_text(p[106], "c) 连接程序与库：CMake 查找 Qt5 Core、Quick、Gui、SerialPort、Gamepad、Network、Widgets、QuickControls2，链接 libmpv、OpenCV，可选链接 ONNX Runtime；Windows 还链接 ws2_32。当前构建没有把业务代码拆成多个动态库。")
set_paragraph_text(p[107], "d) 编译与链接选项：当前 Debug 构建实际使用 -g、QT_QML_DEBUG 和 QT_DECLARATIVE_DEBUG；C++ 标准由 CMAKE_CXX_STANDARD=14 设置。工程没有配置 -Wall、-Wextra、-Werror、-ffast-math 或自定义 rpath。编译选项修改应同步维护 CMakeLists.txt。")
set_paragraph_text(p[108], "表 6  构建配置与依赖参数")
set_paragraph_text(p[110], "e) 构建流程：生成构建目录后执行 CMake 配置和构建。AUTOMOC、AUTOUIC、AUTORCC 自动处理元对象、界面和资源；qml.qrc 把主界面资源编入程序。构建后，CMake 仅自动复制 OpenCV、libmpv 和可选的 ONNX Runtime DLL，不会自动执行 Windows 的 windeployqt，也不会生成校验清单。")
set_paragraph_text(p[111], "f) 加载与执行：当前 Windows 构建产物为 build/kylinqt.exe。运行时需确保 Qt、libmpv、OpenCV、ONNX Runtime 和模型等依赖路径正确。程序由用户直接启动，登录成功后才启动串口、手柄和视频处理线程；仓库没有 start_gbeo.sh 或 systemd 启动脚本。")
set_paragraph_text(p[112], "g) 记录结果：构建记录应至少保存源码版本、CMake 配置、编译器版本、Qt/OpenCV/libmpv/ONNX Runtime 版本和构建时间。当前仓库没有自动生成构建清单的脚本，上述信息需人工登记；不得把仅在本机生成、未经过目标平台验证的产物作为正式交付物。")

set_paragraph_text(p[115], "本节说明本工程实际涉及的 C++、Qt 和平台特征。AArch64 数据布局、调用约定和指令类别用于交付目标阅读；当前 Windows/MinGW 构建的 ABI 与之不同。")
set_paragraph_text(p[116], "a) 数据表示：协议解析使用 Qt 定长整数和显式移位拼接。图像与激光接收代码还存在按结构体布局读取帧的方式，修改结构体时必须检查编译器对齐、固定长度和字节序；跨接口传输的数据不得直接依赖 bool 的位模式。")
set_paragraph_text(p[119], "b) 指令格式与寻址：交付目标为 AArch64 时，C++ 数组和结构体访问由 GCC 生成加载/存储及地址计算指令。当前开发构建为 x86_64，不能通过阅读 Windows 反汇编推断目标平台寄存器分配。影响代码移植的主要因素是结构体填充、长整型宽度和字节序。")
set_paragraph_text(p[120], "c) 专用寄存器：本软件不直接使用架构寄存器。AArch64 上的 x29/x30、SIMD 寄存器和平台保留寄存器由编译器及运行库管理。需要平台级优化时，应在目标构建中通过性能测量确认，不手工绑定寄存器。")
set_paragraph_text(p[121], "d) 调用约定：Qt 信号槽、QObject 元对象、QThread::moveToThread 和跨线程 QueuedConnection 是主要调用边界。跨线程对象必须以 QObject 及其信号槽传递，不通过裸函数指针或跨线程直接访问 QML 对象。目标 AArch64 构建遵循 AAPCS64，当前 x86_64 构建遵循 MinGW/Win64 ABI。")
set_paragraph_text(p[124], "e) 子程序与过程：业务对象以 QObject 派生类实现，参数通过值、const 引用或 Qt 元类型信号传递。main.cpp 将串口工作对象移动到工作线程，数据对象保留在主线程；ImageData、LaserData、TurntableDataHex、BDData、CCDData、ModeController、DataRecorder 和 SixDofMotionClient 通过上下文属性暴露给 QML。")
set_paragraph_text(p[125], "f) 异步事件：Qt 的 readyRead、timer、signal/slot 和 libmpv 的 update callback 构成主要异步模型。串口工作线程只负责 I/O 与解析，解析结果通过 QueuedConnection 回主线程；视频渲染通过 QQuickFramebufferObject 与渲染线程交互。")
set_paragraph_text(p[126], "g) 定时器与时钟：周期任务使用 QTimer，高精度测量使用 QElapsedTimer；业务时间来自设备返回时标、北斗 RMC、转台毫秒时间或本机 QDateTime。图像和激光在返回时标无效时回退本机时间，转台保存协议毫秒时间，各来源不能假定天然同步。")
set_paragraph_text(p[127], "h) 内存保护：用户态不能自定义 MPU/MMU 保护。串口和 UDP 解析器必须先积累完整报文、检查长度和校验，再按固定偏移访问。图像、激光和转台接收缓冲在 readyRead 中追加数据并查找帧头，半帧需保留到下一批数据。")
set_paragraph_text(p[128], "i) 其他特征：视频原始帧、OpenCV 推理结果和 UI 帧副本是主要大块内存访问对象；保存采用 200 ms 文本缓冲；视频录像采用 mpv record-file 写 TS 后由 ffmpeg 流复制为 MP4。性能优化应先使用现有延迟探针和 qDebug/top 数据确认瓶颈。")

set_paragraph_text(p[130], "本工程不包含手写机器指令或汇编源程序。程序人员主要修改 C++14、QML、CMake、Qt 信号槽、串口/UDP 协议和设备控制逻辑。下表仅列出阅读目标平台反汇编时可能遇到的指令类别；完整定义以 ARM Architecture Reference Manual 为准。")
set_paragraph_text(p[133], "性能判断以实际链路和实测时延为准。工程提供视频接收延迟、CCD 直显延迟、串口发送延迟和滤波仿真工具，可在相应环境中启用；不得用单条指令周期或理论主频替代端到端测量。")
set_paragraph_text(p[134], "条件码和寄存器内容属于目标平台反汇编层面。阅读代码和维护接口时，应以源码中的固定字节协议、校验函数和 Qt 对象边界为依据。")

set_paragraph_text(p[136], "a) 设备输入：图像帧通过串口工作线程的 readyRead 累积，按 0x77 0xAB 帧头切出定长帧并执行 CRC16；激光帧按 0x55 0xAA 0xDC 帧头切出定长帧并执行异或校验；转台帧按 0x55 0xAA 帧头切出 29 字节并执行 bytes 2～26 累加校验；北斗只解析 $GBRMC/$BDRMC。")
set_paragraph_text(p[139], "b) 程序加载与校验：程序镜像由操作系统加载，软件不在运行期校验自身。当前仓库不读取 INI/JSON 配置文件；串口、波特率、视频 URL、六自由度网络参数等主要由界面选中值或代码默认值提供。修改默认值后须重新构建并执行接口回归。")
set_paragraph_text(p[140], "c) 数据通道：当前使用图像串口 460800 bps、激光串口 115200 bps、转台串口 230400 bps、北斗和 CCD 串口 9600 bps（均以界面选项为准）；六自由度平台使用 QUdpSocket，默认本机 8080、运动广播 192.168.15.255:7408、状态广播 255.255.255.255:8081。")
set_paragraph_text(p[141], "d) 离散输入输出：手柄通过 Qt Gamepad/GamepadBridge 提供轴量、油门和按键信号，经 QueuedConnection 送入手柄工作线程；界面按钮和开关通过 QML 调用槽函数。输出指示灯和状态文字由 QML 属性绑定，不在串口线程直接修改界面对象。")
set_paragraph_text(p[142], "e) 接口部件：主要接口包括 Qt Serial Port、QUdpSocket、libmpv 视频 URL、Qt Quick OpenGL 渲染上下文和本地文件系统。串口端口由可用端口列表和下拉框选择，网络地址默认值位于 SixDofMotionClient，视频 URL 默认值位于视频组件；当前没有统一配置管理模块。")
set_paragraph_text(p[143], "f) 设备操作码：转台命令码定义在 serialport_turntable_HEX.h，图像和激光发送字段定义在各自发送数据结构中。新增或修改协议常量时，应同步更新发送构帧、接收解析、界面显示和测试用例，避免只改一侧。")

set_paragraph_text(p[145], "a) QML 与 C++：main.cpp 使用 rootContext 暴露 auth、imageData、laserData、turntableData、bdData、ccdData、sixDofMotion、modeController、gamepadBridge、logManager 和 dataRecorder；VlcVideoItem/VlcFrameItem 通过 qmlRegisterType 注册。QML 直接绑定属性并调用 Q_INVOKABLE 接口。")
set_paragraph_text(p[146], "b) 线程模型：图像、激光、转台、北斗各自拥有 QThread；手柄对象在 Handlethread；StreamProcessor 在独立视频处理线程；CCD 串口、模式控制、六自由度 UDP、视频显示和 DataRecorder 主要在主线程。对象必须在创建线程中使用，跨线程结果通过信号槽投递。")
set_paragraph_text(p[147], "c) 构建单元：当前工程只生成一个 kylinqt 可执行目标，未拆分为 4 个动态业务库。新增模块应优先加入 CMakeLists.txt 的现有目标；只有在接口稳定、部署收益明确时才考虑新增库，并同步更新依赖复制和手册。")
set_paragraph_text(p[148], "d) 资源与配置：主界面和 TaoQuick 资源通过 qml.qrc、qrc:/ URL 和 TaoQuickImportPath 加载。当前没有 system.ini、interface.ini、deploy.ini；参数缺省值在 QML 或 C++ 中。不得把当前不存在的配置文件写成维护入口。")
set_paragraph_text(p[149], "e) 日志与诊断：LogManager 在内存中最多保存 500 条界面日志，按 200 ms 节流，不落盘；串口、网络、视频组件通过 qDebug/qWarning 输出当前帧、状态和错误。关键链路排故时应同时保存终端输出、界面日志和可复现的操作步骤。")
set_paragraph_text(p[150], "f) 安全编程：认证使用程序内置盐和 HMAC-SHA256 摘要，明文口令不保存；登录连续失败 5 次锁定 60 s。设备输入按不可信数据处理，长度和校验未通过时不得进入业务解析。不得在日志中输出明文口令或敏感任务参数。")

set_paragraph_text(p[152], "本节给出六个贴近当前工程的示例。示例用于说明修改位置和调用关系，省略了头文件与完整错误处理；直接使用时须结合现有类接口补齐。")
set_paragraph_text(p[155], "示例 1 展示串口按字节流收帧。串口一次 readyRead 可能只到达半帧，因此必须保留缓冲、查找帧头、按定长切帧。激光帧使用三重帧头 0x55 0xAA 0xDC，图像帧使用 0x77 0xAB，转台帧使用 0x55 0xAA。")
set_paragraph_text(p[156], "void SerialPortLaser::onReadyRead()\n{\n    m_rxBuffer.append(m_serialPort->readAll());\n    const int frameLen = static_cast<int>(sizeof(laser_recv_frame));\n    while (m_rxBuffer.size() >= 3) {\n        int i = m_rxBuffer.indexOf(QByteArray(\"\\x55\\xAA\\xDC\", 3));\n        if (i < 0) { m_rxBuffer = m_rxBuffer.right(2); break; }\n        if (i > 0) m_rxBuffer.remove(0, i);\n        if (m_rxBuffer.size() < frameLen) break;\n        const QByteArray frame = m_rxBuffer.left(frameLen);\n        m_rxBuffer.remove(0, frameLen);\n        parseData(frame);\n    }\n}", 8.5)
set_paragraph_text(p[157], "示例 2 展示不同协议的校验与字节序。图像接收帧使用 CRC16-CCITT 比对；激光协议使用异或校验且接收字段按大端解释；转台状态帧的校验为 bytes 2～26 累加和。不能把某一个设备的校验规则复用到其他设备。")
set_paragraph_text(p[158], "quint8 xorChecksum(const QByteArray &data)\n{\n    quint8 sum = 0;\n    for (char c : data) sum ^= static_cast<quint8>(c);\n    return sum;\n}\n\nquint16 readBe16(const quint8 *p)\n{\n    return (quint16(p[0]) << 8) | quint16(p[1]);\n}\n", 8.5)
set_paragraph_text(p[159], "示例 3 展示 C++ 对象与 QML 的连接。现工程在 main.cpp 中使用 setContextProperty 暴露数据和控制对象，QML 通过属性绑定显示状态，通过 Q_INVOKABLE 方法发起打开串口、保存、连接或发送指令。跨线程对象通过信号和 QueuedConnection 回推状态。")
set_paragraph_text(p[160], "engine.rootContext()->setContextProperty(\"laserData\", laserData);\nengine.rootContext()->setContextProperty(\"dataRecorder\", dataRecorder);\nQObject::connect(laserPort, &SerialPortLaser::laserFrameReceived,\n                 laserData, &LaserData::updateFromFrame,\n                 Qt::QueuedConnection);\nQObject::connect(imageData, &ImageData::frameParsed,\n                 dataRecorder, &DataRecorder::onImageFrame,\n                 Qt::DirectConnection);", 8.5)
set_paragraph_text(p[161], "示例 4 展示 libmpv 视频显示与录像。VlcVideoItem 在主线程持有 mpv 句柄，渲染通过 QQuickFramebufferObject；startRecord 设置 mpv 的 record-file 属性写原始 TS，stopRecord 置空后结束录制。录像结束后 DataRecorder 调用 ffmpeg -c copy 转为 MP4。")
set_paragraph_text(p[162], "mpv_set_option_string(m_mpv, \"vo\", \"libmpv\");\nmpv_initialize(m_mpv);\nmpv_set_property_string(m_mpv, \"record-file\", tsPath.toUtf8().constData());\n// 显示结束后：\nmpv_set_property_string(m_mpv, \"record-file\", \"\");", 8.5)
set_paragraph_text(p[163], "示例 5 展示当前数据记录方式。DataRecorder 在主线程接收解析完成的数据，按各来源的中文表头生成 CSV 行，先写入 SourceState::buffer；共享 QTimer 每 200 ms 调用 flushAll，停止某一路时立即冲刷并关闭该路文件。录像请求按三路保存计数共享。")
set_paragraph_text(p[164], "void DataRecorder::onImageFrame()\n{\n    if (!m_image.saving || !m_imageData) return;\n    ensureSourceFile(m_image, \"image\", kImageHeader, \"图像\");\n    if (m_image.openFailed) return;\n    m_image.buffer.append(buildImageCsvLine().toUtf8());\n}\n// m_flushTimer.interval = 200 ms; timeout -> flushAll()", 8.5)
set_paragraph_text(p[165], "示例 6 展示当前 CMake 构建命令。Windows/MinGW 开发环境先清除旧缓存或使用新构建目录，再指定 Qt、OpenCV、libmpv 和可选 ONNX Runtime 路径。目标平台构建必须使用对应工具链重新配置。")
set_paragraph_text(p[166], "cmake -S . -B build -G \"MinGW Makefiles\" ^\n  -DCMAKE_BUILD_TYPE=Debug ^\n  -DCMAKE_C_COMPILER=E:/QT5.12.8/Tools/mingw730_64/bin/gcc.exe ^\n  -DCMAKE_CXX_COMPILER=E:/QT5.12.8/Tools/mingw730_64/bin/g++.exe ^\n  -DCMAKE_PREFIX_PATH=E:/QT5.12.8/5.12.8/mingw73_64 ^\n  -DOpenCV_DIR=E:/opencv-4.2.0/DNN/install ^\n  -DMPV_ROOT=E:/QTproject/mpv\ncmake --build build -j8", 8.5)

set_paragraph_text(p[168], "a) 编译期诊断：当前 CMake 未强制开启 -Wall/-Wextra，也未配置 Werror。修改代码后应至少在 Debug 和 Release 两种配置下构建，检查 AUTOMOC、Qt 信号槽签名、QML 资源和控制台告警。可选自测通过 KYLINQT_BUILD_SELFTESTS 打开，目前 CMake 接入的测试目标覆盖 program_tracking_file。")
set_paragraph_text(p[169], "b) 运行期错误：工程没有统一的“模块＋类别＋序号”数字错误码。常见错误以 QSerialPort 错误串、portError/popupMessage、SixDofMotionClient::lastError、VlcVideoItem::error、DataRecorder::errorOccurred 和 qDebug 消息表现。定位时应先记录信号来源和发生操作，再检查对应对象状态。")
set_paragraph_text(p[170], "表 12  常见错误与诊断")
set_paragraph_text(p[172], "c) 诊断顺序：先复现操作并记录界面提示；再查看终端中的 qDebug/qWarning 与 LogManager 条目；串口问题检查端口、波特率、帧头、帧长和校验；UDP 问题检查本机端口、广播地址、控制卡 IP 和 200 ms 状态查询；视频问题检查 URL、网络、mpv 解码和 record-file 路径；保存问题检查目录、权限、磁盘空间和 ffmpeg 路径。")
set_paragraph_text(p[173], "d) 崩溃分析：如需 core dump，应由目标操作系统配置转储路径和大小限制；当前程序未启用自定义崩溃处理器。core 文件和内存快照可能包含任务数据，须按涉密载体管理。Windows 开发环境可使用 Qt Creator/GDB 或 MinGW 调试器附加构建产物。")
set_paragraph_text(p[174], "e) 启动自检：当前没有 check_env.sh 或独立配置完整性校验器。登录成功后 main.cpp 创建串口/手柄/视频线程；组件通过端口扫描、连接状态、帧解析、UDP 应答和播放器错误暴露问题。发布前应人工确认依赖 DLL、模型、视频 URL、串口和网络环境。")

set_paragraph_text(p[180], "a) 本文档依据 GJB 438C-2021 附录 R 的章节设置编制，同时按当前工程实际技术栈描述编程环境、编译连接、程序接口、示例与诊断方法。")
set_paragraph_text(p[181], "b) 附录 R 为资料性附录。本文档不虚构 qmake 工程、多个业务动态库、配置文件、数字错误码或当前不存在的自检脚本；对尚未固化的目标平台构建工具链，明确说明须在交付环境重新配置和验证。")
set_paragraph_text(p[182], "c) 本文档与《软件设计说明》的分工为：设计说明规定软件结构和接口，本手册说明如何阅读、修改、构建和诊断当前源码。两者不一致时，以评审通过的源码和设计基线为准，并同步修订。")
set_paragraph_text(p[183], "d) 本文档的更新条件为：CMake 或 Qt 版本变更、编译器或目标平台变更、模块目录调整、依赖库替换、接口协议变化、构建或诊断流程变化。")

replace_table(
    doc,
    doc.tables[2],
    ["项目", "内容"],
    [
        ["计算机系统制造厂商", "交付目标按项目资料为飞腾/麒麟；当前源码构建主机为通用 x86_64 Windows 环境"],
        ["计算机系统型号", "交付目标：飞腾腾锐 D2000 平台与银河麒麟 V10；当前开发构建：MinGW 7.3.0"],
        ["系统标识号", "DD/GF 001（对地光电 XX 试验系统）"],
        ["软件名称", "DD/GF 试验系统软件"],
        ["软件标识号", "DD/GF 007"],
        ["软件版本号", "V2.5.0（Build 20261101）"],
        ["文档名称", "DD/GF 试验系统软件计算机编程手册"],
        ["文档标识号", "DD/GF 007-CPM-001"],
        ["文档版本", "V1.1"],
        ["文档密级", "SS"],
    ],
    [4.1, 10.4],
    [WD_ALIGN_PARAGRAPH.LEFT, WD_ALIGN_PARAGRAPH.LEFT],
    9.5,
)

replace_table(
    doc,
    doc.tables[3],
    ["序号", "编号 / 版本", "名称", "编写单位"],
    [
        ["1", "GJB 438C-2021", "军用软件开发文档通用要求", "中央军委装备发展部"],
        ["2", "GJB 2786A-2009", "军用软件开发通用要求", "中国人民解放军总装备部"],
        ["3", "DD/GF 007-SDD-001", "软件设计说明（V1.0）", "哈尔滨工业大学"],
        ["4", "DD/GF 007-UM-001", "软件用户手册（V1.0）", "哈尔滨工业大学"],
        ["5", "ISO/IEC 14882:2014", "C++14 语言标准", "ISO/IEC"],
        ["6", "Qt 5.12", "Qt Core、Quick、SerialPort、Gamepad、Network 文档", "The Qt Company"],
        ["7", "CMake 3.16", "CMake Build System 文档", "Kitware"],
        ["8", "OpenCV 4.2 / libmpv", "图像处理与视频接口文档", "OpenCV Foundation / mpv 项目"],
    ],
    [1.0, 3.1, 6.8, 3.6],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    9.0,
)

replace_table(
    doc,
    doc.tables[4],
    ["环境", "配置", "与本工程相关的特性"],
    [
        ["交付目标处理器", "飞腾腾锐 D2000，AArch64", "项目资料声明的交付平台；目标构建须重新验证"],
        ["交付目标系统", "银河麒麟桌面操作系统 V10 国防版", "系统、Qt 和驱动兼容性以目标环境为准"],
        ["当前开发环境", "Windows x86_64", "现有 build/ 为 MinGW 7.3.0 生成的 x86_64 Debug 构建"],
        ["内存与存储", "项目资料按 8 GB 内存、固态硬盘配置", "软件无固定容量上限，视频与模型加载为主要占用"],
        ["显示与视频", "桌面显示与硬件/软件视频解码", "VlcVideoItem 使用 OpenGL FBO 和 libmpv"],
        ["网络", "UDP 广播/单播与 RTSP/UDP 视频", "六自由度默认使用 192.168.15.x 网段"],
        ["串口与 USB", "多路 QSerialPort 设备与 USB HID 手柄", "端口由界面扫描后选择，不依赖固定设备节点"],
    ],
    [3.0, 6.4, 5.2],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    9.0,
)

replace_table(
    doc,
    doc.tables[5],
    ["类别", "名称与版本", "用途与约束"],
    [
        ["语言与界面", "C++14、QML 2.12", "C++ 实现业务与硬件接口，QML 实现界面和交互"],
        ["构建系统", "CMake 3.16+，MinGW Makefiles", "AUTOMOC/AUTOUIC/AUTORCC 开启"],
        ["当前已验证编译器", "MinGW-W64 g++ 7.3.0", "x86_64 Windows；目标 AArch64 工具链未在仓库固化"],
        ["Qt", "Qt 5.12.8 MinGW73_64", "Core、Quick、Gui、SerialPort、Gamepad、Network、Widgets、QuickControls2"],
        ["图像处理", "OpenCV 4.2.0 DNN", "CMake 默认路径 E:/opencv-4.2.0/DNN/install"],
        ["视频", "libmpv SDK", "MPV_ROOT 默认 E:/QTproject/mpv；构建后复制 libmpv DLL"],
        ["可选 AI 推理", "ONNX Runtime（MinGW 兼容构建）", "未找到时使用 stub，YOLO 推理不可用"],
        ["测试与诊断", "Qt Test、qDebug、延迟探针", "可选自测由 KYLINQT_BUILD_SELFTESTS 控制"],
    ],
    [3.0, 5.2, 6.4],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

replace_table(
    doc,
    doc.tables[6],
    ["目录 / 文件", "主要内容", "构建与运行关系"],
    [
        ["main.cpp", "对象创建、线程装配、信号连接、QML 上下文", "程序入口和总装点"],
        ["main.qml / qml.qrc", "主界面和资源清单", "编译进 qrc 资源"],
        ["components/", "业务界面、弹窗、控件和视频组件", "QML 通过资源 URL 加载"],
        ["serialport/", "串口基类、图像、激光、转台、BD、CCD", "QSerialPort 工作线程与协议解析"],
        ["network/", "SixDofMotionClient", "UDP 状态查询、运动指令与链路看门狗"],
        ["vlcvideo/", "VlcVideoItem、VlcFrameItem", "libmpv 显示、录像和帧访问"],
        ["opencv/", "StreamProcessor、检测器、像素跟踪", "视频处理线程，ONNX 为可选依赖"],
        ["record/、log/、auth/", "数据保存、内存日志、HMAC 登录", "主线程对象和 QML 接口"],
        ["ModeControl/、kalman/、utils/", "模式控制、跟踪滤波、坐标与延迟工具", "业务解算与诊断支撑"],
        ["tests/", "自测和滤波仿真", "部分目标由 CMake 选项接入"],
    ],
    [3.4, 6.6, 4.6],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

replace_table(
    doc,
    doc.tables[7],
    ["配置 / 选项", "当前用途", "维护要求"],
    [
        ["CMAKE_CXX_STANDARD=14", "C++14", "不得无评审地切到更高标准"],
        ["CMAKE_BUILD_TYPE=Debug", "当前缓存配置", "正式交付应建立 Release 构建并单独验证"],
        ["QT_QML_DEBUG / QT_DECLARATIVE_DEBUG", "Debug QML 诊断", "仅用于 Debug/RelWithDebInfo"],
        ["AUTOMOC/AUTOUIC/AUTORCC", "生成元对象、界面和资源代码", "新增 QObject/QML 文件后重新配置"],
        ["MPV_ROOT", "libmpv SDK 根目录", "必须含头文件、链接库和运行库"],
        ["OpenCV_DIR", "OpenCV 4.2 安装目录", "DLL 由 POST_BUILD 复制到程序目录"],
        ["ONNXRUNTIME_ROOT", "可选 ONNX Runtime 根目录", "须与 MinGW ABI 一致，否则使用 stub"],
        ["KYLINQT_BUILD_SELFTESTS", "构建命令行自测", "默认 OFF；启用后按测试结果回归"],
    ],
    [4.4, 5.2, 5.0],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

replace_table(
    doc,
    doc.tables[11],
    ["接口 / 参数项", "当前设置", "说明"],
    [
        ["通用串口配置", "8 数据位、无校验、1 停止位、无流控", "SerialPort::openSerialDevice 统一设置"],
        ["图像导引头", "460800 bps", "界面默认值"],
        ["激光导引头", "115200 bps", "界面默认值"],
        ["三轴转台", "230400 bps", "界面默认值"],
        ["北斗模块", "9600 bps", "界面默认值"],
        ["CCD 控制", "9600 bps", "界面默认值"],
        ["读取方式", "readyRead + 接收缓冲", "按帧头或换行切帧，不假设一次读对应一帧"],
        ["设备选择", "QSerialPortInfo 扫描 + QML 下拉框", "当前不依赖 udev 固定映射配置文件"],
    ],
    [4.0, 5.4, 5.2],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

replace_table(
    doc,
    doc.tables[12],
    ["序号", "示例", "对应源码位置", "要点"],
    [
        ["1", "串口按字节流收帧", "serialport/serialport_*.cpp", "帧头、帧长、半帧保留"],
        ["2", "校验与字节序", "图像 CRC16、激光异或、转台累加和", "按设备协议分别处理"],
        ["3", "C++ 对象暴露给 QML", "main.cpp、Q_PROPERTY、Q_INVOKABLE", "上下文属性与信号槽"],
        ["4", "libmpv 显示与录像", "vlcvideo/VlcVideoItem.*", "record-file 与 TS 录制"],
        ["5", "CSV 缓冲写入", "record/DataRecorder.*", "200 ms 缓冲、独立启停"],
        ["6", "CMake 构建", "CMakeLists.txt、.vscode/settings.json", "MinGW 和依赖路径"],
    ],
    [1.0, 4.1, 4.6, 4.9],
    [
        WD_ALIGN_PARAGRAPH.CENTER,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

error_table = replace_table(
    doc,
    doc.tables[13],
    ["类别", "典型现象", "定位位置", "处置"],
    [
        ["串口打开", "portError、端口列表无目标设备", "SerialPort/openPort", "检查设备连接、占用、权限和型号"],
        ["串口解析", "无数据更新、CRC/异或/累加校验失败", "各 parseData", "检查帧头、波特率、帧长和字节序"],
        ["UDP 链路", "lastError、linkAlive=false", "SixDofMotionClient", "检查 8080 本机端口、7408/8081 广播与控制卡 IP"],
        ["登录", "密钥错误、剩余次数减少、锁定", "AuthManager", "使用正确密钥；5 次失败后等待 60 s"],
        ["视频", "error、黑屏、反复重连", "VlcVideoItem、mpv 日志", "检查 URL、网络、解码器和播放状态"],
        ["录像", "TS 未生成、MP4 转换失败", "DataRecorder", "检查 record-file、ffmpegPath、路径与权限"],
        ["数据保存", "errorOccurred、文件未创建", "DataRecorder", "检查保存目录、首帧是否到达和磁盘权限"],
        ["QML 绑定", "属性为空、按钮无响应", "main.cpp、qDebug", "检查上下文属性、对象名和信号槽签名"],
        ["AI 推理", "模型未加载、无检测结果", "StreamProcessor、stub", "检查 ONNX Runtime、best.onnx 与模型路径"],
        ["构建", "找不到 Qt/OpenCV/mpv/ONNX", "CMakeCache.txt", "修正 CMAKE_PREFIX_PATH 和依赖根目录"],
    ],
    [2.7, 4.2, 3.8, 3.9],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.5,
)
keep_table_together(error_table)

replace_table(
    doc,
    doc.tables[14],
    ["术语 / 缩略语", "全称或含义", "在本手册中的说明"],
    [
        ["CPM", "Computer Programming Manual", "即本文档"],
        ["CMake", "跨平台构建系统", "本工程唯一的构建描述入口"],
        ["AUTOMOC", "Qt 自动元对象编译", "自动生成 QObject 相关代码"],
        ["QML", "Qt Meta Language", "界面和交互描述语言"],
        ["QSerialPort", "Qt 串口模块", "串口工作线程的核心接口"],
        ["QUdpSocket", "Qt UDP 套接字", "六自由度平台通信"],
        ["libmpv", "mpv 客户端库", "视频显示、帧访问和原始码流录制"],
        ["OpenCV", "开源计算机视觉库", "流处理、检测和像素跟踪"],
        ["ONNX Runtime", "ONNX 模型推理运行时", "可选；未安装时使用 stub"],
        ["TS / MP4", "视频传输流 / MPEG-4 容器", "录像暂存与最终输出格式"],
        ["Q_DECLARE_METATYPE", "Qt 元类型声明", "支持跨线程信号参数"],
    ],
    [3.1, 5.4, 5.9],
    [
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
        WD_ALIGN_PARAGRAPH.LEFT,
    ],
    8.8,
)

keep_heading_relationships(p)
set_update_fields(doc)
doc.core_properties.title = "DD/GF 试验系统软件 计算机编程手册"
doc.core_properties.subject = "按当前 CMake、Qt、C++ 与 QML 工程实现修订"
doc.core_properties.version = "1.1"
doc.save(OUTPUT)
print(OUTPUT)
