// 测试用前置头：用 -include 抢先定义 serialport.h 的包含保护，
// 使工程里真实的 serialport/serialport.h（依赖 Qt）不被展开，
// 同时补上该头文件里与外引导数据包相关的结构体定义。
// struct sendExGuideData 与 serialport/serialport.h 中定义完全一致（字段顺序/类型不变）。
#ifndef STUB_PRESERIALPORT_H
#define STUB_PRESERIALPORT_H

#define SERIALPORT_H

#include <cstdint>

// 外引导模式下，单轴数据（原定义：serialport/serialport.h:60）
struct sendExGuideData
{
    double angle1;
    double angle2;
    double angle3;
    double angle4;
    uint32_t time;
};

#endif
