#ifndef SERIALPORT_IMAGE_H
#define SERIALPORT_IMAGE_H

#include "serialport.h"

class SerialPortImage : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortImage(QObject *parent = nullptr);
    ~SerialPortImage() override;

protected:
    QByteArray parseData(const QByteArray &rawData) override;
private:
     static uint16_t crc16_table[256];
     void init_crc16_table(uint16_t poly = 0x1021) ;
     uint16_t crc16_ccitt_fast(const uint8_t *data, size_t len, uint16_t init = 0xFFFF);
};

//图像导引头接收数据结构体
#pragma pack(push,1)
typedef struct {
    // 字节0-1: 帧头
    quint8 frame_header1;       // 0x77
    quint8 frame_header2;       // 0xAB

    // 字节2: 帧长（除去帧头和校验位的字节数）
    quint8 frame_length;

    // 字节3-4: B帧流水号
    quint16 b_frame_sequence;

    // 字节5-6: A帧流水号回告
    quint16 a_frame_sequence_reply;

    // 字节7: A帧有效标志 (0xAA=收到新帧, 0x00=未收到)
    quint8 a_frame_valid_flag;

    // 字节8: 导引头控制字回告
    quint8 seeker_ctrl_reply;

    // 字节9: 光学参数装订回告
    quint8 optical_param_reply;

    // 字节10: 预留1
    quint8 reserved1;

    // 字节11: 当前工作通道 (0x02=红外, 0x03=电视)
    quint8 current_work_channel;

    // 字节12: 自检标志 (位域)
    quint8 self_check_flag;

    // 字节13-14: 俯仰视线角速度 (单位:°/s, 精度0.002)
    qint16 pitch_line_of_sight_ang_vel;

    // 字节15-16: 偏航视线角速度
    qint16 yaw_line_of_sight_ang_vel;

    // 字节17: 目标/背景类型预装回告
    quint8 target_background_type;

    // 字节18: 光学工作状态
    quint8 optical_work_state;

    // 字节19-20: 俯仰框架角
    qint16 pitch_frame_angle;

    // 字节21-22: 偏航框架角
    qint16 yaw_frame_angle;

    // 字节23: 修正指令状态回告 (bit0)
    quint8 correction_cmd_status;

    // 字节24: 修正指令次数回告
    quint8 correction_cmd_count;

    // 字节25-26: 俯仰陀螺
    qint16 pitch_gyro;

    // 字节27-28: 偏航陀螺
    qint16 yaw_gyro;

    // 字节29-31: 预留3字节
    quint8 reserved3[3];

    // 字节32: 跟踪状态
    quint8 tracking_state;

    // 字节33: 跟踪器状态
    quint8 tracker_state;

    // 字节34-52: 预留19字节
    quint8 reserved4[19];

    // 字节53-54: 方位偏差像素
    qint16 azimuth_deviation_pixel;

    // 字节55-56: 俯仰偏差像素
    qint16 pitch_deviation_pixel;

    // 字节57-58: 方位轴陀螺输出 (LSB=0.01°/s)
    qint16 azimuth_gyro_output;

    // 字节59-60: 俯仰轴陀螺输出
    qint16 pitch_gyro_output;

    // 字节61-62: 方位主令
    qint16 azimuth_master_cmd;

    // 字节63-64: 俯仰主令
    qint16 pitch_master_cmd;

    // 字节65-72: 预留8字节
    quint8 reserved5[8];

    // 字节73: 平台自检结果
    quint8 platform_self_check;

    // 字节74-81: 预留8字节
    quint8 reserved6[8];

    // 字节82: 伺服运行时间
    quint8 servo_running_time;

    // 字节83-89: 预留7字节
    quint8 reserved7[7];

    // 字节90: 伺服阶跃
    quint8 servo_step;

    // 字节91-94: 红外帧编号
    quint32 infrared_frame_num;

    // 字节95-100: 预留6字节
    quint8 reserved8[6];

    // 字节101-102: Cbh_tv4405 (上位机新增)
    quint16 cbh_tv4405;

    // 字节103-104: 未定义，作为预留
    quint8 reserved9[2];

    // 字节105-110: 预留6字节
    quint8 reserved10[6];

    // 字节111: 红外帧频
    quint8 infrared_frame_rate;

    // 字节112: 电视帧频
    quint8 tv_frame_rate;

    // 字节113-116: 预留4字节
    quint8 reserved11[4];

    // 字节117-118: 波门尺寸
    quint16 gate_size;

    // 字节119-130: 预留12字节
    quint8 reserved12[12];

    // 字节131: 软件1版本号
    quint8 software_version1;

    // 字节132: 软件2版本号
    quint8 software_version2;

    // 字节133: 软件3版本号
    quint8 software_version3;

    // 字节134-153: 预留20字节
    quint8 reserved13[20];

    // 字节154-155: CRC16校验
    quint16 crc16;
} image_recv_frame;
#pragma pack(pop)

//图像导引头发送数据结构体
#pragma pack(push,1)
typedef struct {
    // 定义发送数据的字段
    // 字节0-1: 帧头
    quint8 frame_header1;       // 0x77
    quint8 frame_header2;       // 0xAA

    // 字节2: A帧帧长（除去帧头和校验位的字节数）
    quint8 frame_length;

    // 字节3-4: A帧流水号 (0~65535)
    quint16 a_frame_sequence;

    // 字节5: 导引头控制字 (见文档说明)
    quint8 seeker_ctrl_word;

    // 字节6: 光学参数装订控制字
    quint8 optical_param_ctrl;

    // 字节7: 模板序号 (1-10)
    quint8 template_index;

    // 字节8: 目标/背景类型预装 (位域)
    quint8 target_background_type;

    // 字节9-10: 弹目距离 (单位: m, 精度1, 范围20~20000)
    quint16 missile_target_distance;

    // 字节11-12: 导弹速度 (单位: m/s, 精度0.1, 范围0~500)
    quint16 missile_speed;      // 实际值 = 存储值 * 0.1

    // 字节13-14: 弹体俯仰角 (单位: °, 精度0.01, 范围-90~+90)
    qint16 body_pitch_angle;

    // 字节15-16: 弹体方位角 (单位: °, 精度0.01, 范围-180~+180)
    qint16 body_yaw_angle;

    // 字节17-18: 弹体滚转角 (单位: °, 精度0.01, 范围-180~+180)
    qint16 body_roll_angle;

    // 字节19-20: 弹体俯仰角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_pitch_rate;

    // 字节21-22: 弹体方位角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_yaw_rate;

    // 字节23-24: 弹体滚转角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_roll_rate;

    // 字节25-26: 弹体实时X向速度 (目标系, 单位: m/s, 精度0.1, 范围-500~+500)
    qint16 body_vel_x;

    // 字节27-28: 弹体实时Y向速度 (目标系)
    qint16 body_vel_y;

    // 字节29-30: 弹体实时Z向速度 (目标系)
    qint16 body_vel_z;

    // 字节31-32: 弹体实时X坐标 (目标系, 单位: m, 精度1, 范围-10000~+10000)
    qint16 body_pos_x;

    // 字节33-34: 弹体实时Y坐标
    qint16 body_pos_y;

    // 字节35-36: 弹体实时Z坐标
    qint16 body_pos_z;

    // 字节37-38: 俯仰框架角预装 (单位: °, 精度0.002, 范围-18~+18)
    qint16 pitch_gimbal_preset;

    // 字节39-40: 偏航框架角预装 (单位: °, 精度0.002, 范围-18~+18)
    qint16 yaw_gimbal_preset;

    // 字节41: 红外积分时间 (单位: ms, 精度0.1, 范围0.5~19.5)
    quint8 ir_integration_time; // 实际值 = 存储值 * 0.1

    // 字节42-45: 预留4字节
    quint8 reserved1[4];

    // 字节46: 跟踪位置修正指令 (Bit0: 0-非修正, 1-修正)
    quint8 tracking_correction_cmd;

    // 字节47-49: 修正图像帧序号 (24位, 低字节在前)
    quint8 correction_frame_num[3]; // 小端序

    // 字节50-51: 修正后俯仰跟踪位置 (0~511像素)
    quint16 corrected_pitch_pos;

    // 字节52-53: 修正后偏航跟踪位置 (0~639像素)
    quint16 corrected_yaw_pos;

    // 字节54-55: 搜索俯仰角速度 (单位: °/s, 精度0.01)
    qint16 search_pitch_rate;

    // 字节56-57: 搜索方位角速度 (单位: °/s, 精度0.01)
    qint16 search_yaw_rate;

    // 字节58: 预留1字节
    quint8 reserved2;

    // 字节59: 波门 (0xAA=大, 0x55=小)
    quint8 gate_size;

    // 字节60: 字符叠加 (0xAA=开, 0x55=关)
    quint8 osd_switch;

    // 字节61: 拍摄参考图指令 (0xAA=拍摄, 发三拍后恢复默认)
    quint8 capture_ref_img_cmd;

    // 字节62-152: 预留91字节
    quint8 reserved3[91];       // 62~152 inclusive count = 152-62+1=91

    // 字节153-154: 目标海拔高度 (单位: m, 精度1)
    qint16 target_altitude;

    // 字节155-156: 飞机俯仰角 (单位: °, 精度0.01)
    qint16 aircraft_pitch;

    // 字节157-158: 飞机方位角 (单位: °, 精度0.01)
    qint16 aircraft_yaw;

    // 字节159-160: 飞机滚转角 (单位: °, 精度0.01)
    qint16 aircraft_roll;

    // 字节161-162: 焦距 (单位: mm, 精度1)
    quint16 focal_length;

    // 字节163-164: 吊舱俯仰框架角 (单位: °, 精度0.01)
    qint16 pod_pitch_angle;

    // 字节165-166: 吊舱方位框架角 (单位: °, 精度0.01)
    qint16 pod_yaw_angle;

    // 字节167-200: 预留34字节 (200-167+1=34)
    quint8 reserved4[34];

    // 字节201: 卫星图比例 (单像素代表实际尺寸, 精度0.1, 默认0)
    quint8 satellite_map_scale; // 实际值 = 存储值 * 0.1

    // 字节202: 吊舱类型 (待定)
    quint8 pod_type;

    // 字节203-206: 目标经度 (单位: °, 精度1e-6, 东经正西经负)
    qint32 target_longitude;    // 实际值 = 存储值 * 1e-6

    // 字节207-210: 目标纬度 (单位: °, 精度1e-6)
    qint32 target_latitude;

    // 字节211-214: 飞机经度
    qint32 aircraft_longitude;

    // 字节215-218: 飞机纬度
    qint32 aircraft_latitude;

    // 字节219-220: 飞机海拔高度 (单位: m, 精度1)
    qint16 aircraft_altitude;

    // 字节221: 像元尺寸 (单位: um, 精度1)
    quint8 pixel_size;

    // 字节222-223: CRC16校验位 (多项式X16+X12+X5+1)
    quint16 crc16;

} image_send_frame;
#pragma pack(pop)
#endif // SERIALPORT_IMAGE_H
