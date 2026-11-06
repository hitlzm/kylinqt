cmake_minimum_required(VERSION 3.16)

project(kylinqt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "E:/opencv-4.2.0/DNN/install")

include(taoQuick.cmake)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)


#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# libmpv 库路径
set(MPV_ROOT "E:/QTproject/mpv" CACHE PATH "libmpv SDK root")

# libmpv SDK 头文件、导入库和运行库
find_path(MPV_INCLUDE_DIR mpv/client.h
    PATHS "${MPV_ROOT}/include"
    NO_DEFAULT_PATH)
find_file(MPV_LIBRARY
    NAMES libmpv.dll.a libmpv.a mpv.lib libmpv.lib
    PATHS "${MPV_ROOT}" "${MPV_ROOT}/lib"
    NO_DEFAULT_PATH)
find_file(MPV_DLL
    NAMES libmpv-2.dll mpv-2.dll
    PATHS "${MPV_ROOT}" "${MPV_ROOT}/bin"
    NO_DEFAULT_PATH)

# Eigen（坐标变换头文件）
find_path(EIGEN_INCLUDE_DIR Eigen/Dense
    PATHS "E:/onnxruntime/cmake/external/eigen"
          "E:/matlab2/toolbox/shared/robotics/externalDependency/eigen/include/eigen3"
    NO_DEFAULT_PATH)


find_package(Qt5 COMPONENTS Core Quick Gui SerialPort Gamepad Network Widgets QuickControls2 REQUIRED REQUIRED)
find_package(OpenCV REQUIRED)

if(NOT OPENCV_BIN_DIR)
    # 尝试从 OpenCV_DIR 推导
    get_filename_component(OPENCV_BIN_DIR "${OpenCV_DIR}/x64/mingw/bin" ABSOLUTE)
endif()

# ── ONNX Runtime ──
# 需指向包含 include/onnxruntime_cxx_api.h 与 lib/ 库文件的 ONNX Runtime 安装目录。
# 注意：本工程使用 MinGW 工具链，ONNX Runtime 必须是 MinGW 兼容的构建产物
#（官方 Release 为 MSVC ABI，无法直接链接；需用相同工具链自行编译）。
set(ONNXRUNTIME_ROOT "" CACHE PATH "ONNX Runtime install prefix (must contain include/ and lib/)")
if(NOT ONNXRUNTIME_ROOT)
    set(ONNXRUNTIME_ROOT "E:/onnxruntime-1.5.3/onnxruntime-1.5.3/build/install")
endif()

# 源码树中公共头文件位于 include/onnxruntime/core/session/，安装包中位于 include/
find_path(ONNXRUNTIME_INCLUDE_DIR onnxruntime_cxx_api.h
    PATHS "${ONNXRUNTIME_ROOT}/include"
          "${ONNXRUNTIME_ROOT}/include/onnxruntime/core/session"
    NO_DEFAULT_PATH)
find_path(ONNXRUNTIME_C_API_INCLUDE_DIR onnxruntime_c_api.h
    PATHS "${ONNXRUNTIME_ROOT}/include"
          "${ONNXRUNTIME_ROOT}/include/onnxruntime/core/session"
    NO_DEFAULT_PATH)
find_library(ONNXRUNTIME_LIBRARY
    NAMES onnxruntime libonnxruntime
    PATHS "${ONNXRUNTIME_ROOT}/lib"
          "${ONNXRUNTIME_ROOT}/lib64"
          "${ONNXRUNTIME_ROOT}/bin"
    NO_DEFAULT_PATH)
find_file(ONNXRUNTIME_DLL onnxruntime.dll
    PATHS "${ONNXRUNTIME_ROOT}/bin" "${ONNXRUNTIME_ROOT}/lib"
    NO_DEFAULT_PATH)

if(ONNXRUNTIME_INCLUDE_DIR AND ONNXRUNTIME_C_API_INCLUDE_DIR AND ONNXRUNTIME_LIBRARY)
    set(ONNXRUNTIME_DETECTOR_SOURCE opencv/onnxyolov5detector.cpp)
    message(STATUS "ONNX Runtime found: ${ONNXRUNTIME_ROOT}")
else()
    set(ONNXRUNTIME_DETECTOR_SOURCE opencv/onnxyolov5detector_stub.cpp)
    message(WARNING "ONNX Runtime not found; building without YOLO inference")
endif()

if(ANDROID)
    add_library(kylinqt SHARED
      main.cpp
      qml.qrc
      serialport/serialport.cpp
      serialport/serialport_image.cpp
      serialport/serialport_laser.cpp
      serialport/serialport_turntable_HEX.cpp
      utils/program_tracking_file.cpp
      kalman/AlphaBetaTracker.cpp
      video/vlcplayer.cpp
    )
else()
    add_executable(kylinqt
      main.cpp
      qml.qrc
      serialport/serialport.cpp
      serialport/serialport_image.cpp
      serialport/serialport_laser.cpp
      serialport/serialport_turntable_HEX.cpp
      utils/program_tracking_file.cpp
      serialport/serialport_BD.cpp
      serialport/serialport_CCD.cpp
      kalman/AlphaBetaTracker.cpp
      # 倾角仪串口已停用（改为人工观看数显屏）：文件保留，仅不再被 main.cpp 使用
      #serialport/serialport_tiltmeter.cpp
      vlcvideo/VlcVideoItem.cpp
      vlcvideo/VlcFrameItem.cpp
      #opencv/onnxyolodetector.cpp
      ${ONNXRUNTIME_DETECTOR_SOURCE}
      opencv/onnxyolov5detector.h
      opencv/streamprocessor.cpp
      opencv/pixelkalmantracker.cpp 
      opencv/cvconvert.h
      record/DataRecorder.cpp
      handle/myhandle.h
      ModeControl/ModeController.h
      auth/AuthManager.cpp
      auth/AuthManager.h
      log/LogManager.cpp
      #serialport/circularbuffer.h
      #kalman/seekKalman.cpp
      utils/videolatencyprobe.cpp
      utils/videolatencyprobe.h
      utils/sendlatencyprobe.cpp
      utils/sendlatencyprobe.h
      #network/TemplateBindingClient.cpp
      network/SixDofMotionClient.cpp
      ${TaoQuickRes}
    )
endif()

target_include_directories(kylinqt PRIVATE
    ${MPV_INCLUDE_DIR}
    ${EIGEN_INCLUDE_DIR}
)

if(ONNXRUNTIME_INCLUDE_DIR AND ONNXRUNTIME_C_API_INCLUDE_DIR AND ONNXRUNTIME_LIBRARY)
    target_include_directories(kylinqt PRIVATE
        ${ONNXRUNTIME_INCLUDE_DIR}
        ${ONNXRUNTIME_C_API_INCLUDE_DIR}
    )
endif()

target_compile_definitions(kylinqt
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(kylinqt
  PRIVATE
    Qt5::Core Qt5::Quick Qt5::Gui Qt5::SerialPort Qt5::Gamepad Qt5::Network Qt5::Widgets Qt5::QuickControls2
    ${MPV_LIBRARY}
    ${OpenCV_LIBS}
)

if(ONNXRUNTIME_LIBRARY)
    target_link_libraries(kylinqt PRIVATE ${ONNXRUNTIME_LIBRARY})
endif()

if(WIN32)
    target_link_libraries(kylinqt PRIVATE ws2_32)
endif()

# 可选的无界面自测目标；默认不参与构建。
option(KYLINQT_BUILD_SELFTESTS "Build kylinqt command-line self-tests" OFF)
if(KYLINQT_BUILD_SELFTESTS)
    enable_testing()
    add_executable(kylinqt_program_tracking_file_selftest
        tests/program_tracking_file_selftest.cpp
        utils/program_tracking_file.cpp
    )
    target_link_libraries(kylinqt_program_tracking_file_selftest PRIVATE Qt5::Core)
    add_test(NAME program_tracking_file_selftest
             COMMAND kylinqt_program_tracking_file_selftest)
endif()

# ── 视频接收侧延迟探针（utils/videolatencyprobe.*）─────────────────────
# 默认关闭，对现有功能零影响。需要出延迟报告时取消下面注释并重新编译，
# 也可以在运行时用环境变量 KYLIN_LATPROBE=1 临时打开（无需重编译）。
# target_compile_definitions(kylinqt PRIVATE
#     KYLIN_LATPROBE_DEFAULT_ON=1
#     KYLIN_LATPROBE_DEFAULT_CSV="/tmp/video_latency.csv"
# )
#   说明：KYLIN_LATPROBE_DEFAULT_CSV 的值必须带引号，写成字符串字面量。

# ── 串口发送延迟探针（utils/sendlatencyprobe.*）────────────────────────
# 默认关闭，对现有功能零影响。运行时用环境变量 KYLIN_SENDLAT=1 打开即可，
# 无需重新编译。下面两个宏是可选的：
#   SENDLAT_COMPILED=0    探针完全不参与编译：sendlatencyprobe.cpp 编译成空
#                         文件，打点调用点也编译成空函数，二进制里不留任何
#                         探针代码（连 setEnabled/stats 这些接口一起没有）
#   SENDLAT_DEFAULT_ON=1  编译期就把探针默认打开（一般不用，留 0）
# target_compile_definitions(kylinqt PRIVATE
#     SENDLAT_COMPILED=1
#     SENDLAT_DEFAULT_ON=0
# )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}  # OpenCV的头文件路径
)

set(OPENCV_DLLS
    libopencv_core420.dll
    libopencv_imgproc420.dll
    libopencv_imgcodecs420.dll
    libopencv_dnn420.dll
    libopencv_video420.dll
    libopencv_features2d420.dll
    libopencv_flann420.dll
)

foreach(DLL ${OPENCV_DLLS})
    add_custom_command(TARGET kylinqt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENCV_BIN_DIR}/${DLL}"
            "$<TARGET_FILE_DIR:kylinqt>"
    )
endforeach()

# ── 编译后自动复制 mpv DLL 到 exe 目录 ──
if(MPV_DLL)
    add_custom_command(TARGET kylinqt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MPV_DLL}"
            "$<TARGET_FILE_DIR:kylinqt>"
    )
endif()

if(ONNXRUNTIME_DLL)
    add_custom_command(TARGET kylinqt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ONNXRUNTIME_DLL}"
            "$<TARGET_FILE_DIR:kylinqt>"
    )
endif()
