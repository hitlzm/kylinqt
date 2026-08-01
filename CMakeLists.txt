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
set(MPV_ROOT "E:/QTproject/mpv")

# libmpv SDK 头文件和库
set(MPV_INCLUDE_DIR "${MPV_ROOT}/include")
set(MPV_LIBRARY_DIR "${MPV_ROOT}/lib")


find_package(Qt5 COMPONENTS Core Quick Gui SerialPort Gamepad Network REQUIRED)
find_package(OpenCV REQUIRED)

if(NOT OPENCV_BIN_DIR)
    # 尝试从 OpenCV_DIR 推导
    get_filename_component(OPENCV_BIN_DIR "${OpenCV_DIR}/x64/mingw/bin" ABSOLUTE)
endif()

if(ANDROID)
    add_library(kylinqt SHARED
      main.cpp
      qml.qrc
      serialport/serialport.cpp
      serialport/serialport_image.cpp
      serialport/serialport_laser.cpp
      serialport/serialport_turntable_HEX.cpp
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
      serialport/serialport_BD.cpp
      serialport/serialport_CCD.cpp
      vlcvideo/VlcVideoItem.cpp
      vlcvideo/VlcFrameItem.cpp
      handle/myhandle.h
      ModeControl/ModeController.h
      serialport/circularbuffer.h
      kalman/seekKalman.cpp
      network/TemplateBindingClient.cpp
    #   opencv/cvconvert.h
      ${TaoQuickRes}
    )
endif()

target_include_directories(kylinqt PRIVATE
    ${MPV_INCLUDE_DIR}
)

target_link_directories(kylinqt PRIVATE
    ${MPV_LIBRARY_DIR}
)

target_compile_definitions(kylinqt
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(kylinqt
  PRIVATE
    Qt5::Core Qt5::Quick Qt5::Gui Qt5::SerialPort Qt5::Gamepad Qt5::Network
    libvlc libvlccore
    ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}  # OpenCV的头文件路径
)

set(OPENCV_DLLS
    libopencv_core420.dll
    libopencv_imgproc420.dll
    libopencv_imgcodecs420.dll
    libopencv_dnn420.dll
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
add_custom_command(TARGET kylinqt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MPV_ROOT}/mpv-2.dll"
        "$<TARGET_FILE_DIR:kylinqt>"
)
