/****************************************************************************
** Meta object code from reading C++ file 'serialport_image.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_image.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_image.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageData_t {
    QByteArrayData data[118];
    char stringdata0[2116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageData_t qt_meta_stringdata_ImageData = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ImageData"
QT_MOC_LITERAL(1, 10, 18), // "frameLengthChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 21), // "bFrameSequenceChanged"
QT_MOC_LITERAL(4, 52, 26), // "aFrameSequenceReplyChanged"
QT_MOC_LITERAL(5, 79, 22), // "aFrameValidFlagChanged"
QT_MOC_LITERAL(6, 102, 22), // "seekerCtrlReplyChanged"
QT_MOC_LITERAL(7, 125, 24), // "opticalParamReplyChanged"
QT_MOC_LITERAL(8, 150, 25), // "currentWorkChannelChanged"
QT_MOC_LITERAL(9, 176, 20), // "selfCheckFlagChanged"
QT_MOC_LITERAL(10, 197, 21), // "selfCheckFlag1Changed"
QT_MOC_LITERAL(11, 219, 21), // "selfCheckFlag2Changed"
QT_MOC_LITERAL(12, 241, 21), // "selfCheckFlag3Changed"
QT_MOC_LITERAL(13, 263, 21), // "selfCheckFlag4Changed"
QT_MOC_LITERAL(14, 285, 21), // "selfCheckFlag5Changed"
QT_MOC_LITERAL(15, 307, 21), // "selfCheckFlag6Changed"
QT_MOC_LITERAL(16, 329, 21), // "pitchLosAngVelChanged"
QT_MOC_LITERAL(17, 351, 19), // "yawLosAngVelChanged"
QT_MOC_LITERAL(18, 371, 27), // "targetBackgroundTypeChanged"
QT_MOC_LITERAL(19, 399, 28), // "targetBackgroundType1Changed"
QT_MOC_LITERAL(20, 428, 28), // "targetBackgroundType2Changed"
QT_MOC_LITERAL(21, 457, 28), // "targetBackgroundType3Changed"
QT_MOC_LITERAL(22, 486, 28), // "targetBackgroundType4Changed"
QT_MOC_LITERAL(23, 515, 23), // "opticalWorkStateChanged"
QT_MOC_LITERAL(24, 539, 22), // "pitchFrameAngleChanged"
QT_MOC_LITERAL(25, 562, 20), // "yawFrameAngleChanged"
QT_MOC_LITERAL(26, 583, 26), // "correctionCmdStatusChanged"
QT_MOC_LITERAL(27, 610, 25), // "correctionCmdCountChanged"
QT_MOC_LITERAL(28, 636, 16), // "pitchGyroChanged"
QT_MOC_LITERAL(29, 653, 14), // "yawGyroChanged"
QT_MOC_LITERAL(30, 668, 20), // "trackingStateChanged"
QT_MOC_LITERAL(31, 689, 19), // "trackerStateChanged"
QT_MOC_LITERAL(32, 709, 28), // "azimuthDeviationPixelChanged"
QT_MOC_LITERAL(33, 738, 26), // "pitchDeviationPixelChanged"
QT_MOC_LITERAL(34, 765, 24), // "azimuthGyroOutputChanged"
QT_MOC_LITERAL(35, 790, 22), // "pitchGyroOutputChanged"
QT_MOC_LITERAL(36, 813, 23), // "azimuthMasterCmdChanged"
QT_MOC_LITERAL(37, 837, 21), // "pitchMasterCmdChanged"
QT_MOC_LITERAL(38, 859, 24), // "platformSelfCheckChanged"
QT_MOC_LITERAL(39, 884, 23), // "servoRunningTimeChanged"
QT_MOC_LITERAL(40, 908, 16), // "servoStepChanged"
QT_MOC_LITERAL(41, 925, 23), // "infraredFrameNumChanged"
QT_MOC_LITERAL(42, 949, 16), // "cbhTv4405Changed"
QT_MOC_LITERAL(43, 966, 24), // "infraredFrameRateChanged"
QT_MOC_LITERAL(44, 991, 18), // "tvFrameRateChanged"
QT_MOC_LITERAL(45, 1010, 15), // "gateSizeChanged"
QT_MOC_LITERAL(46, 1026, 23), // "softwareVersion1Changed"
QT_MOC_LITERAL(47, 1050, 23), // "softwareVersion2Changed"
QT_MOC_LITERAL(48, 1074, 23), // "softwareVersion3Changed"
QT_MOC_LITERAL(49, 1098, 15), // "portOpenChanged"
QT_MOC_LITERAL(50, 1114, 21), // "availablePortsChanged"
QT_MOC_LITERAL(51, 1136, 18), // "errorStringChanged"
QT_MOC_LITERAL(52, 1155, 15), // "requestOpenPort"
QT_MOC_LITERAL(53, 1171, 8), // "portName"
QT_MOC_LITERAL(54, 1180, 8), // "baudRate"
QT_MOC_LITERAL(55, 1189, 16), // "requestClosePort"
QT_MOC_LITERAL(56, 1206, 16), // "requestScanPorts"
QT_MOC_LITERAL(57, 1223, 11), // "setPortOpen"
QT_MOC_LITERAL(58, 1235, 4), // "open"
QT_MOC_LITERAL(59, 1240, 11), // "setPortList"
QT_MOC_LITERAL(60, 1252, 5), // "ports"
QT_MOC_LITERAL(61, 1258, 8), // "setError"
QT_MOC_LITERAL(62, 1267, 3), // "msg"
QT_MOC_LITERAL(63, 1271, 15), // "updateFromFrame"
QT_MOC_LITERAL(64, 1287, 5), // "frame"
QT_MOC_LITERAL(65, 1293, 8), // "openPort"
QT_MOC_LITERAL(66, 1302, 9), // "closePort"
QT_MOC_LITERAL(67, 1312, 9), // "scanPorts"
QT_MOC_LITERAL(68, 1322, 11), // "frameLength"
QT_MOC_LITERAL(69, 1334, 14), // "bFrameSequence"
QT_MOC_LITERAL(70, 1349, 19), // "aFrameSequenceReply"
QT_MOC_LITERAL(71, 1369, 15), // "aFrameValidFlag"
QT_MOC_LITERAL(72, 1385, 15), // "seekerCtrlReply"
QT_MOC_LITERAL(73, 1401, 17), // "opticalParamReply"
QT_MOC_LITERAL(74, 1419, 18), // "currentWorkChannel"
QT_MOC_LITERAL(75, 1438, 13), // "selfCheckFlag"
QT_MOC_LITERAL(76, 1452, 14), // "selfCheckFlag1"
QT_MOC_LITERAL(77, 1467, 14), // "selfCheckFlag2"
QT_MOC_LITERAL(78, 1482, 14), // "selfCheckFlag3"
QT_MOC_LITERAL(79, 1497, 14), // "selfCheckFlag4"
QT_MOC_LITERAL(80, 1512, 14), // "selfCheckFlag5"
QT_MOC_LITERAL(81, 1527, 14), // "selfCheckFlag6"
QT_MOC_LITERAL(82, 1542, 14), // "pitchLosAngVel"
QT_MOC_LITERAL(83, 1557, 12), // "yawLosAngVel"
QT_MOC_LITERAL(84, 1570, 20), // "targetBackgroundType"
QT_MOC_LITERAL(85, 1591, 21), // "targetBackgroundType1"
QT_MOC_LITERAL(86, 1613, 21), // "targetBackgroundType2"
QT_MOC_LITERAL(87, 1635, 21), // "targetBackgroundType3"
QT_MOC_LITERAL(88, 1657, 21), // "targetBackgroundType4"
QT_MOC_LITERAL(89, 1679, 16), // "opticalWorkState"
QT_MOC_LITERAL(90, 1696, 15), // "pitchFrameAngle"
QT_MOC_LITERAL(91, 1712, 13), // "yawFrameAngle"
QT_MOC_LITERAL(92, 1726, 19), // "correctionCmdStatus"
QT_MOC_LITERAL(93, 1746, 18), // "correctionCmdCount"
QT_MOC_LITERAL(94, 1765, 9), // "pitchGyro"
QT_MOC_LITERAL(95, 1775, 7), // "yawGyro"
QT_MOC_LITERAL(96, 1783, 13), // "trackingState"
QT_MOC_LITERAL(97, 1797, 12), // "trackerState"
QT_MOC_LITERAL(98, 1810, 21), // "azimuthDeviationPixel"
QT_MOC_LITERAL(99, 1832, 19), // "pitchDeviationPixel"
QT_MOC_LITERAL(100, 1852, 17), // "azimuthGyroOutput"
QT_MOC_LITERAL(101, 1870, 15), // "pitchGyroOutput"
QT_MOC_LITERAL(102, 1886, 16), // "azimuthMasterCmd"
QT_MOC_LITERAL(103, 1903, 14), // "pitchMasterCmd"
QT_MOC_LITERAL(104, 1918, 17), // "platformSelfCheck"
QT_MOC_LITERAL(105, 1936, 16), // "servoRunningTime"
QT_MOC_LITERAL(106, 1953, 9), // "servoStep"
QT_MOC_LITERAL(107, 1963, 16), // "infraredFrameNum"
QT_MOC_LITERAL(108, 1980, 9), // "cbhTv4405"
QT_MOC_LITERAL(109, 1990, 17), // "infraredFrameRate"
QT_MOC_LITERAL(110, 2008, 11), // "tvFrameRate"
QT_MOC_LITERAL(111, 2020, 8), // "gateSize"
QT_MOC_LITERAL(112, 2029, 16), // "softwareVersion1"
QT_MOC_LITERAL(113, 2046, 16), // "softwareVersion2"
QT_MOC_LITERAL(114, 2063, 16), // "softwareVersion3"
QT_MOC_LITERAL(115, 2080, 8), // "portOpen"
QT_MOC_LITERAL(116, 2089, 14), // "availablePorts"
QT_MOC_LITERAL(117, 2104, 11) // "errorString"

    },
    "ImageData\0frameLengthChanged\0\0"
    "bFrameSequenceChanged\0aFrameSequenceReplyChanged\0"
    "aFrameValidFlagChanged\0seekerCtrlReplyChanged\0"
    "opticalParamReplyChanged\0"
    "currentWorkChannelChanged\0"
    "selfCheckFlagChanged\0selfCheckFlag1Changed\0"
    "selfCheckFlag2Changed\0selfCheckFlag3Changed\0"
    "selfCheckFlag4Changed\0selfCheckFlag5Changed\0"
    "selfCheckFlag6Changed\0pitchLosAngVelChanged\0"
    "yawLosAngVelChanged\0targetBackgroundTypeChanged\0"
    "targetBackgroundType1Changed\0"
    "targetBackgroundType2Changed\0"
    "targetBackgroundType3Changed\0"
    "targetBackgroundType4Changed\0"
    "opticalWorkStateChanged\0pitchFrameAngleChanged\0"
    "yawFrameAngleChanged\0correctionCmdStatusChanged\0"
    "correctionCmdCountChanged\0pitchGyroChanged\0"
    "yawGyroChanged\0trackingStateChanged\0"
    "trackerStateChanged\0azimuthDeviationPixelChanged\0"
    "pitchDeviationPixelChanged\0"
    "azimuthGyroOutputChanged\0"
    "pitchGyroOutputChanged\0azimuthMasterCmdChanged\0"
    "pitchMasterCmdChanged\0platformSelfCheckChanged\0"
    "servoRunningTimeChanged\0servoStepChanged\0"
    "infraredFrameNumChanged\0cbhTv4405Changed\0"
    "infraredFrameRateChanged\0tvFrameRateChanged\0"
    "gateSizeChanged\0softwareVersion1Changed\0"
    "softwareVersion2Changed\0softwareVersion3Changed\0"
    "portOpenChanged\0availablePortsChanged\0"
    "errorStringChanged\0requestOpenPort\0"
    "portName\0baudRate\0requestClosePort\0"
    "requestScanPorts\0setPortOpen\0open\0"
    "setPortList\0ports\0setError\0msg\0"
    "updateFromFrame\0frame\0openPort\0closePort\0"
    "scanPorts\0frameLength\0bFrameSequence\0"
    "aFrameSequenceReply\0aFrameValidFlag\0"
    "seekerCtrlReply\0opticalParamReply\0"
    "currentWorkChannel\0selfCheckFlag\0"
    "selfCheckFlag1\0selfCheckFlag2\0"
    "selfCheckFlag3\0selfCheckFlag4\0"
    "selfCheckFlag5\0selfCheckFlag6\0"
    "pitchLosAngVel\0yawLosAngVel\0"
    "targetBackgroundType\0targetBackgroundType1\0"
    "targetBackgroundType2\0targetBackgroundType3\0"
    "targetBackgroundType4\0opticalWorkState\0"
    "pitchFrameAngle\0yawFrameAngle\0"
    "correctionCmdStatus\0correctionCmdCount\0"
    "pitchGyro\0yawGyro\0trackingState\0"
    "trackerState\0azimuthDeviationPixel\0"
    "pitchDeviationPixel\0azimuthGyroOutput\0"
    "pitchGyroOutput\0azimuthMasterCmd\0"
    "pitchMasterCmd\0platformSelfCheck\0"
    "servoRunningTime\0servoStep\0infraredFrameNum\0"
    "cbhTv4405\0infraredFrameRate\0tvFrameRate\0"
    "gateSize\0softwareVersion1\0softwareVersion2\0"
    "softwareVersion3\0portOpen\0availablePorts\0"
    "errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      60,   14, // methods
      50,  390, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      53,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  314,    2, 0x06 /* Public */,
       3,    0,  315,    2, 0x06 /* Public */,
       4,    0,  316,    2, 0x06 /* Public */,
       5,    0,  317,    2, 0x06 /* Public */,
       6,    0,  318,    2, 0x06 /* Public */,
       7,    0,  319,    2, 0x06 /* Public */,
       8,    0,  320,    2, 0x06 /* Public */,
       9,    0,  321,    2, 0x06 /* Public */,
      10,    0,  322,    2, 0x06 /* Public */,
      11,    0,  323,    2, 0x06 /* Public */,
      12,    0,  324,    2, 0x06 /* Public */,
      13,    0,  325,    2, 0x06 /* Public */,
      14,    0,  326,    2, 0x06 /* Public */,
      15,    0,  327,    2, 0x06 /* Public */,
      16,    0,  328,    2, 0x06 /* Public */,
      17,    0,  329,    2, 0x06 /* Public */,
      18,    0,  330,    2, 0x06 /* Public */,
      19,    0,  331,    2, 0x06 /* Public */,
      20,    0,  332,    2, 0x06 /* Public */,
      21,    0,  333,    2, 0x06 /* Public */,
      22,    0,  334,    2, 0x06 /* Public */,
      23,    0,  335,    2, 0x06 /* Public */,
      24,    0,  336,    2, 0x06 /* Public */,
      25,    0,  337,    2, 0x06 /* Public */,
      26,    0,  338,    2, 0x06 /* Public */,
      27,    0,  339,    2, 0x06 /* Public */,
      28,    0,  340,    2, 0x06 /* Public */,
      29,    0,  341,    2, 0x06 /* Public */,
      30,    0,  342,    2, 0x06 /* Public */,
      31,    0,  343,    2, 0x06 /* Public */,
      32,    0,  344,    2, 0x06 /* Public */,
      33,    0,  345,    2, 0x06 /* Public */,
      34,    0,  346,    2, 0x06 /* Public */,
      35,    0,  347,    2, 0x06 /* Public */,
      36,    0,  348,    2, 0x06 /* Public */,
      37,    0,  349,    2, 0x06 /* Public */,
      38,    0,  350,    2, 0x06 /* Public */,
      39,    0,  351,    2, 0x06 /* Public */,
      40,    0,  352,    2, 0x06 /* Public */,
      41,    0,  353,    2, 0x06 /* Public */,
      42,    0,  354,    2, 0x06 /* Public */,
      43,    0,  355,    2, 0x06 /* Public */,
      44,    0,  356,    2, 0x06 /* Public */,
      45,    0,  357,    2, 0x06 /* Public */,
      46,    0,  358,    2, 0x06 /* Public */,
      47,    0,  359,    2, 0x06 /* Public */,
      48,    0,  360,    2, 0x06 /* Public */,
      49,    0,  361,    2, 0x06 /* Public */,
      50,    0,  362,    2, 0x06 /* Public */,
      51,    0,  363,    2, 0x06 /* Public */,
      52,    2,  364,    2, 0x06 /* Public */,
      55,    0,  369,    2, 0x06 /* Public */,
      56,    0,  370,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      57,    1,  371,    2, 0x0a /* Public */,
      59,    1,  374,    2, 0x0a /* Public */,
      61,    1,  377,    2, 0x0a /* Public */,
      63,    1,  380,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      65,    2,  383,    2, 0x02 /* Public */,
      66,    0,  388,    2, 0x02 /* Public */,
      67,    0,  389,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   53,   54,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   58,
    QMetaType::Void, QMetaType::QStringList,   60,
    QMetaType::Void, QMetaType::QString,   62,
    QMetaType::Void, QMetaType::QByteArray,   64,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   53,   54,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      68, QMetaType::Int, 0x00495001,
      69, QMetaType::Int, 0x00495001,
      70, QMetaType::Int, 0x00495001,
      71, QMetaType::Int, 0x00495001,
      72, QMetaType::Int, 0x00495001,
      73, QMetaType::Int, 0x00495001,
      74, QMetaType::Int, 0x00495001,
      75, QMetaType::Int, 0x00495001,
      76, QMetaType::Int, 0x00495001,
      77, QMetaType::Int, 0x00495001,
      78, QMetaType::Int, 0x00495001,
      79, QMetaType::Int, 0x00495001,
      80, QMetaType::Int, 0x00495001,
      81, QMetaType::Int, 0x00495001,
      82, QMetaType::Float, 0x00495001,
      83, QMetaType::Float, 0x00495001,
      84, QMetaType::Int, 0x00495001,
      85, QMetaType::Int, 0x00495001,
      86, QMetaType::Int, 0x00495001,
      87, QMetaType::Int, 0x00495001,
      88, QMetaType::Int, 0x00495001,
      89, QMetaType::Int, 0x00495001,
      90, QMetaType::Float, 0x00495001,
      91, QMetaType::Float, 0x00495001,
      92, QMetaType::Int, 0x00495001,
      93, QMetaType::Int, 0x00495001,
      94, QMetaType::Float, 0x00495001,
      95, QMetaType::Float, 0x00495001,
      96, QMetaType::Int, 0x00495001,
      97, QMetaType::Int, 0x00495001,
      98, QMetaType::Int, 0x00495001,
      99, QMetaType::Int, 0x00495001,
     100, QMetaType::Float, 0x00495001,
     101, QMetaType::Float, 0x00495001,
     102, QMetaType::Float, 0x00495001,
     103, QMetaType::Float, 0x00495001,
     104, QMetaType::Int, 0x00495001,
     105, QMetaType::Int, 0x00495001,
     106, QMetaType::Int, 0x00495001,
     107, QMetaType::Int, 0x00495001,
     108, QMetaType::Int, 0x00495001,
     109, QMetaType::Int, 0x00495001,
     110, QMetaType::Int, 0x00495001,
     111, QMetaType::Int, 0x00495001,
     112, QMetaType::Int, 0x00495001,
     113, QMetaType::Int, 0x00495001,
     114, QMetaType::Int, 0x00495001,
     115, QMetaType::Bool, 0x00495001,
     116, QMetaType::QStringList, 0x00495001,
     117, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,
       8,
       9,
      10,
      11,
      12,
      13,
      14,
      15,
      16,
      17,
      18,
      19,
      20,
      21,
      22,
      23,
      24,
      25,
      26,
      27,
      28,
      29,
      30,
      31,
      32,
      33,
      34,
      35,
      36,
      37,
      38,
      39,
      40,
      41,
      42,
      43,
      44,
      45,
      46,
      47,
      48,
      49,

       0        // eod
};

void ImageData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameLengthChanged(); break;
        case 1: _t->bFrameSequenceChanged(); break;
        case 2: _t->aFrameSequenceReplyChanged(); break;
        case 3: _t->aFrameValidFlagChanged(); break;
        case 4: _t->seekerCtrlReplyChanged(); break;
        case 5: _t->opticalParamReplyChanged(); break;
        case 6: _t->currentWorkChannelChanged(); break;
        case 7: _t->selfCheckFlagChanged(); break;
        case 8: _t->selfCheckFlag1Changed(); break;
        case 9: _t->selfCheckFlag2Changed(); break;
        case 10: _t->selfCheckFlag3Changed(); break;
        case 11: _t->selfCheckFlag4Changed(); break;
        case 12: _t->selfCheckFlag5Changed(); break;
        case 13: _t->selfCheckFlag6Changed(); break;
        case 14: _t->pitchLosAngVelChanged(); break;
        case 15: _t->yawLosAngVelChanged(); break;
        case 16: _t->targetBackgroundTypeChanged(); break;
        case 17: _t->targetBackgroundType1Changed(); break;
        case 18: _t->targetBackgroundType2Changed(); break;
        case 19: _t->targetBackgroundType3Changed(); break;
        case 20: _t->targetBackgroundType4Changed(); break;
        case 21: _t->opticalWorkStateChanged(); break;
        case 22: _t->pitchFrameAngleChanged(); break;
        case 23: _t->yawFrameAngleChanged(); break;
        case 24: _t->correctionCmdStatusChanged(); break;
        case 25: _t->correctionCmdCountChanged(); break;
        case 26: _t->pitchGyroChanged(); break;
        case 27: _t->yawGyroChanged(); break;
        case 28: _t->trackingStateChanged(); break;
        case 29: _t->trackerStateChanged(); break;
        case 30: _t->azimuthDeviationPixelChanged(); break;
        case 31: _t->pitchDeviationPixelChanged(); break;
        case 32: _t->azimuthGyroOutputChanged(); break;
        case 33: _t->pitchGyroOutputChanged(); break;
        case 34: _t->azimuthMasterCmdChanged(); break;
        case 35: _t->pitchMasterCmdChanged(); break;
        case 36: _t->platformSelfCheckChanged(); break;
        case 37: _t->servoRunningTimeChanged(); break;
        case 38: _t->servoStepChanged(); break;
        case 39: _t->infraredFrameNumChanged(); break;
        case 40: _t->cbhTv4405Changed(); break;
        case 41: _t->infraredFrameRateChanged(); break;
        case 42: _t->tvFrameRateChanged(); break;
        case 43: _t->gateSizeChanged(); break;
        case 44: _t->softwareVersion1Changed(); break;
        case 45: _t->softwareVersion2Changed(); break;
        case 46: _t->softwareVersion3Changed(); break;
        case 47: _t->portOpenChanged(); break;
        case 48: _t->availablePortsChanged(); break;
        case 49: _t->errorStringChanged(); break;
        case 50: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 51: _t->requestClosePort(); break;
        case 52: _t->requestScanPorts(); break;
        case 53: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 55: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 56: _t->updateFromFrame((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 57: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 58: _t->closePort(); break;
        case 59: _t->scanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::frameLengthChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::bFrameSequenceChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::aFrameSequenceReplyChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::aFrameValidFlagChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::seekerCtrlReplyChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::opticalParamReplyChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::currentWorkChannelChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlagChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag1Changed)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag2Changed)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag3Changed)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag4Changed)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag5Changed)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::selfCheckFlag6Changed)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchLosAngVelChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawLosAngVelChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundTypeChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundType1Changed)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundType2Changed)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundType3Changed)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundType4Changed)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::opticalWorkStateChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchFrameAngleChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawFrameAngleChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::correctionCmdStatusChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::correctionCmdCountChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchGyroChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawGyroChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::trackingStateChanged)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::trackerStateChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthDeviationPixelChanged)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchDeviationPixelChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthGyroOutputChanged)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchGyroOutputChanged)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthMasterCmdChanged)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchMasterCmdChanged)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::platformSelfCheckChanged)) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::servoRunningTimeChanged)) {
                *result = 37;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::servoStepChanged)) {
                *result = 38;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::infraredFrameNumChanged)) {
                *result = 39;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::cbhTv4405Changed)) {
                *result = 40;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::infraredFrameRateChanged)) {
                *result = 41;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::tvFrameRateChanged)) {
                *result = 42;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::gateSizeChanged)) {
                *result = 43;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion1Changed)) {
                *result = 44;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion2Changed)) {
                *result = 45;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion3Changed)) {
                *result = 46;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::portOpenChanged)) {
                *result = 47;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::availablePortsChanged)) {
                *result = 48;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::errorStringChanged)) {
                *result = 49;
                return;
            }
        }
        {
            using _t = void (ImageData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::requestOpenPort)) {
                *result = 50;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::requestClosePort)) {
                *result = 51;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::requestScanPorts)) {
                *result = 52;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ImageData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->frameLength(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->bFrameSequence(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->aFrameSequenceReply(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->aFrameValidFlag(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->seekerCtrlReply(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->opticalParamReply(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->currentWorkChannel(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag1(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag2(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag3(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag4(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag5(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->selfCheckFlag6(); break;
        case 14: *reinterpret_cast< float*>(_v) = _t->pitchLosAngVel(); break;
        case 15: *reinterpret_cast< float*>(_v) = _t->yawLosAngVel(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType(); break;
        case 17: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType1(); break;
        case 18: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType2(); break;
        case 19: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType3(); break;
        case 20: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType4(); break;
        case 21: *reinterpret_cast< int*>(_v) = _t->opticalWorkState(); break;
        case 22: *reinterpret_cast< float*>(_v) = _t->pitchFrameAngle(); break;
        case 23: *reinterpret_cast< float*>(_v) = _t->yawFrameAngle(); break;
        case 24: *reinterpret_cast< int*>(_v) = _t->correctionCmdStatus(); break;
        case 25: *reinterpret_cast< int*>(_v) = _t->correctionCmdCount(); break;
        case 26: *reinterpret_cast< float*>(_v) = _t->pitchGyro(); break;
        case 27: *reinterpret_cast< float*>(_v) = _t->yawGyro(); break;
        case 28: *reinterpret_cast< int*>(_v) = _t->trackingState(); break;
        case 29: *reinterpret_cast< int*>(_v) = _t->trackerState(); break;
        case 30: *reinterpret_cast< int*>(_v) = _t->azimuthDeviationPixel(); break;
        case 31: *reinterpret_cast< int*>(_v) = _t->pitchDeviationPixel(); break;
        case 32: *reinterpret_cast< float*>(_v) = _t->azimuthGyroOutput(); break;
        case 33: *reinterpret_cast< float*>(_v) = _t->pitchGyroOutput(); break;
        case 34: *reinterpret_cast< float*>(_v) = _t->azimuthMasterCmd(); break;
        case 35: *reinterpret_cast< float*>(_v) = _t->pitchMasterCmd(); break;
        case 36: *reinterpret_cast< int*>(_v) = _t->platformSelfCheck(); break;
        case 37: *reinterpret_cast< int*>(_v) = _t->servoRunningTime(); break;
        case 38: *reinterpret_cast< int*>(_v) = _t->servoStep(); break;
        case 39: *reinterpret_cast< int*>(_v) = _t->infraredFrameNum(); break;
        case 40: *reinterpret_cast< int*>(_v) = _t->cbhTv4405(); break;
        case 41: *reinterpret_cast< int*>(_v) = _t->infraredFrameRate(); break;
        case 42: *reinterpret_cast< int*>(_v) = _t->tvFrameRate(); break;
        case 43: *reinterpret_cast< int*>(_v) = _t->gateSize(); break;
        case 44: *reinterpret_cast< int*>(_v) = _t->softwareVersion1(); break;
        case 45: *reinterpret_cast< int*>(_v) = _t->softwareVersion2(); break;
        case 46: *reinterpret_cast< int*>(_v) = _t->softwareVersion3(); break;
        case 47: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 48: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 49: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ImageData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageData.data,
    qt_meta_data_ImageData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 60)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 60;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 60)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 60;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 50;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ImageData::frameLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ImageData::bFrameSequenceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ImageData::aFrameSequenceReplyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ImageData::aFrameValidFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ImageData::seekerCtrlReplyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ImageData::opticalParamReplyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ImageData::currentWorkChannelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ImageData::selfCheckFlagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ImageData::selfCheckFlag1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ImageData::selfCheckFlag2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ImageData::selfCheckFlag3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ImageData::selfCheckFlag4Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ImageData::selfCheckFlag5Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ImageData::selfCheckFlag6Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void ImageData::pitchLosAngVelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ImageData::yawLosAngVelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void ImageData::targetBackgroundTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void ImageData::targetBackgroundType1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void ImageData::targetBackgroundType2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void ImageData::targetBackgroundType3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ImageData::targetBackgroundType4Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void ImageData::opticalWorkStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void ImageData::pitchFrameAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void ImageData::yawFrameAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void ImageData::correctionCmdStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void ImageData::correctionCmdCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void ImageData::pitchGyroChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void ImageData::yawGyroChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void ImageData::trackingStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void ImageData::trackerStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void ImageData::azimuthDeviationPixelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void ImageData::pitchDeviationPixelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void ImageData::azimuthGyroOutputChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void ImageData::pitchGyroOutputChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void ImageData::azimuthMasterCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void ImageData::pitchMasterCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void ImageData::platformSelfCheckChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}

// SIGNAL 37
void ImageData::servoRunningTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void ImageData::servoStepChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void ImageData::infraredFrameNumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 39, nullptr);
}

// SIGNAL 40
void ImageData::cbhTv4405Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}

// SIGNAL 41
void ImageData::infraredFrameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 41, nullptr);
}

// SIGNAL 42
void ImageData::tvFrameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 42, nullptr);
}

// SIGNAL 43
void ImageData::gateSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 43, nullptr);
}

// SIGNAL 44
void ImageData::softwareVersion1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 44, nullptr);
}

// SIGNAL 45
void ImageData::softwareVersion2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 45, nullptr);
}

// SIGNAL 46
void ImageData::softwareVersion3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 46, nullptr);
}

// SIGNAL 47
void ImageData::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 47, nullptr);
}

// SIGNAL 48
void ImageData::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 48, nullptr);
}

// SIGNAL 49
void ImageData::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 49, nullptr);
}

// SIGNAL 50
void ImageData::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 50, _a);
}

// SIGNAL 51
void ImageData::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 51, nullptr);
}

// SIGNAL 52
void ImageData::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 52, nullptr);
}
struct qt_meta_stringdata_ImageSendData_t {
    QByteArrayData data[107];
    char stringdata0[2014];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageSendData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageSendData_t qt_meta_stringdata_ImageSendData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ImageSendData"
QT_MOC_LITERAL(1, 14, 18), // "frameLengthChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 21), // "aFrameSequenceChanged"
QT_MOC_LITERAL(4, 56, 21), // "seekerCtrlWordChanged"
QT_MOC_LITERAL(5, 78, 23), // "opticalParamCtrlChanged"
QT_MOC_LITERAL(6, 102, 20), // "templateIndexChanged"
QT_MOC_LITERAL(7, 123, 27), // "targetBackgroundTypeChanged"
QT_MOC_LITERAL(8, 151, 28), // "targetBackgroundType1Changed"
QT_MOC_LITERAL(9, 180, 28), // "targetBackgroundType2Changed"
QT_MOC_LITERAL(10, 209, 28), // "targetBackgroundType3Changed"
QT_MOC_LITERAL(11, 238, 28), // "targetBackgroundType4Changed"
QT_MOC_LITERAL(12, 267, 28), // "missileTargetDistanceChanged"
QT_MOC_LITERAL(13, 296, 19), // "missileSpeedChanged"
QT_MOC_LITERAL(14, 316, 21), // "bodyPitchAngleChanged"
QT_MOC_LITERAL(15, 338, 19), // "bodyYawAngleChanged"
QT_MOC_LITERAL(16, 358, 20), // "bodyRollAngleChanged"
QT_MOC_LITERAL(17, 379, 20), // "bodyPitchRateChanged"
QT_MOC_LITERAL(18, 400, 18), // "bodyYawRateChanged"
QT_MOC_LITERAL(19, 419, 19), // "bodyRollRateChanged"
QT_MOC_LITERAL(20, 439, 15), // "bodyVelXChanged"
QT_MOC_LITERAL(21, 455, 15), // "bodyVelYChanged"
QT_MOC_LITERAL(22, 471, 15), // "bodyVelZChanged"
QT_MOC_LITERAL(23, 487, 15), // "bodyPosXChanged"
QT_MOC_LITERAL(24, 503, 15), // "bodyPosYChanged"
QT_MOC_LITERAL(25, 519, 15), // "bodyPosZChanged"
QT_MOC_LITERAL(26, 535, 24), // "pitchGimbalPresetChanged"
QT_MOC_LITERAL(27, 560, 22), // "yawGimbalPresetChanged"
QT_MOC_LITERAL(28, 583, 24), // "irIntegrationTimeChanged"
QT_MOC_LITERAL(29, 608, 28), // "trackingCorrectionCmdChanged"
QT_MOC_LITERAL(30, 637, 25), // "correctionFrameNumChanged"
QT_MOC_LITERAL(31, 663, 24), // "correctedPitchPosChanged"
QT_MOC_LITERAL(32, 688, 22), // "correctedYawPosChanged"
QT_MOC_LITERAL(33, 711, 22), // "searchPitchRateChanged"
QT_MOC_LITERAL(34, 734, 20), // "searchYawRateChanged"
QT_MOC_LITERAL(35, 755, 15), // "gateSizeChanged"
QT_MOC_LITERAL(36, 771, 16), // "osdSwitchChanged"
QT_MOC_LITERAL(37, 788, 23), // "captureRefImgCmdChanged"
QT_MOC_LITERAL(38, 812, 21), // "targetAltitudeChanged"
QT_MOC_LITERAL(39, 834, 20), // "aircraftPitchChanged"
QT_MOC_LITERAL(40, 855, 18), // "aircraftYawChanged"
QT_MOC_LITERAL(41, 874, 19), // "aircraftRollChanged"
QT_MOC_LITERAL(42, 894, 18), // "focalLengthChanged"
QT_MOC_LITERAL(43, 913, 20), // "podPitchAngleChanged"
QT_MOC_LITERAL(44, 934, 18), // "podYawAngleChanged"
QT_MOC_LITERAL(45, 953, 24), // "satelliteMapScaleChanged"
QT_MOC_LITERAL(46, 978, 14), // "podTypeChanged"
QT_MOC_LITERAL(47, 993, 22), // "targetLongitudeChanged"
QT_MOC_LITERAL(48, 1016, 21), // "targetLatitudeChanged"
QT_MOC_LITERAL(49, 1038, 24), // "aircraftLongitudeChanged"
QT_MOC_LITERAL(50, 1063, 23), // "aircraftLatitudeChanged"
QT_MOC_LITERAL(51, 1087, 23), // "aircraftAltitudeChanged"
QT_MOC_LITERAL(52, 1111, 16), // "pixelSizeChanged"
QT_MOC_LITERAL(53, 1128, 15), // "requestSendData"
QT_MOC_LITERAL(54, 1144, 17), // "image_send_frame&"
QT_MOC_LITERAL(55, 1162, 5), // "frame"
QT_MOC_LITERAL(56, 1168, 10), // "buildFrame"
QT_MOC_LITERAL(57, 1179, 13), // "m_frameLength"
QT_MOC_LITERAL(58, 1193, 16), // "m_aFrameSequence"
QT_MOC_LITERAL(59, 1210, 16), // "m_seekerCtrlWord"
QT_MOC_LITERAL(60, 1227, 18), // "m_opticalParamCtrl"
QT_MOC_LITERAL(61, 1246, 15), // "m_templateIndex"
QT_MOC_LITERAL(62, 1262, 23), // "m_targetBackgroundType1"
QT_MOC_LITERAL(63, 1286, 23), // "m_targetBackgroundType2"
QT_MOC_LITERAL(64, 1310, 23), // "m_targetBackgroundType3"
QT_MOC_LITERAL(65, 1334, 23), // "m_targetBackgroundType4"
QT_MOC_LITERAL(66, 1358, 23), // "m_missileTargetDistance"
QT_MOC_LITERAL(67, 1382, 14), // "m_missileSpeed"
QT_MOC_LITERAL(68, 1397, 16), // "m_bodyPitchAngle"
QT_MOC_LITERAL(69, 1414, 14), // "m_bodyYawAngle"
QT_MOC_LITERAL(70, 1429, 15), // "m_bodyRollAngle"
QT_MOC_LITERAL(71, 1445, 15), // "m_bodyPitchRate"
QT_MOC_LITERAL(72, 1461, 13), // "m_bodyYawRate"
QT_MOC_LITERAL(73, 1475, 14), // "m_bodyRollRate"
QT_MOC_LITERAL(74, 1490, 10), // "m_bodyVelX"
QT_MOC_LITERAL(75, 1501, 10), // "m_bodyVelY"
QT_MOC_LITERAL(76, 1512, 10), // "m_bodyVelZ"
QT_MOC_LITERAL(77, 1523, 10), // "m_bodyPosX"
QT_MOC_LITERAL(78, 1534, 10), // "m_bodyPosY"
QT_MOC_LITERAL(79, 1545, 10), // "m_bodyPosZ"
QT_MOC_LITERAL(80, 1556, 19), // "m_pitchGimbalPreset"
QT_MOC_LITERAL(81, 1576, 17), // "m_yawGimbalPreset"
QT_MOC_LITERAL(82, 1594, 19), // "m_irIntegrationTime"
QT_MOC_LITERAL(83, 1614, 23), // "m_trackingCorrectionCmd"
QT_MOC_LITERAL(84, 1638, 20), // "m_correctionFrameNum"
QT_MOC_LITERAL(85, 1659, 19), // "m_correctedPitchPos"
QT_MOC_LITERAL(86, 1679, 17), // "m_correctedYawPos"
QT_MOC_LITERAL(87, 1697, 17), // "m_searchPitchRate"
QT_MOC_LITERAL(88, 1715, 15), // "m_searchYawRate"
QT_MOC_LITERAL(89, 1731, 10), // "m_gateSize"
QT_MOC_LITERAL(90, 1742, 11), // "m_osdSwitch"
QT_MOC_LITERAL(91, 1754, 18), // "m_captureRefImgCmd"
QT_MOC_LITERAL(92, 1773, 16), // "m_targetAltitude"
QT_MOC_LITERAL(93, 1790, 15), // "m_aircraftPitch"
QT_MOC_LITERAL(94, 1806, 13), // "m_aircraftYaw"
QT_MOC_LITERAL(95, 1820, 14), // "m_aircraftRoll"
QT_MOC_LITERAL(96, 1835, 13), // "m_focalLength"
QT_MOC_LITERAL(97, 1849, 15), // "m_podPitchAngle"
QT_MOC_LITERAL(98, 1865, 13), // "m_podYawAngle"
QT_MOC_LITERAL(99, 1879, 19), // "m_satelliteMapScale"
QT_MOC_LITERAL(100, 1899, 9), // "m_podType"
QT_MOC_LITERAL(101, 1909, 17), // "m_targetLongitude"
QT_MOC_LITERAL(102, 1927, 16), // "m_targetLatitude"
QT_MOC_LITERAL(103, 1944, 19), // "m_aircraftLongitude"
QT_MOC_LITERAL(104, 1964, 18), // "m_aircraftLatitude"
QT_MOC_LITERAL(105, 1983, 18), // "m_aircraftAltitude"
QT_MOC_LITERAL(106, 2002, 11) // "m_pixelSize"

    },
    "ImageSendData\0frameLengthChanged\0\0"
    "aFrameSequenceChanged\0seekerCtrlWordChanged\0"
    "opticalParamCtrlChanged\0templateIndexChanged\0"
    "targetBackgroundTypeChanged\0"
    "targetBackgroundType1Changed\0"
    "targetBackgroundType2Changed\0"
    "targetBackgroundType3Changed\0"
    "targetBackgroundType4Changed\0"
    "missileTargetDistanceChanged\0"
    "missileSpeedChanged\0bodyPitchAngleChanged\0"
    "bodyYawAngleChanged\0bodyRollAngleChanged\0"
    "bodyPitchRateChanged\0bodyYawRateChanged\0"
    "bodyRollRateChanged\0bodyVelXChanged\0"
    "bodyVelYChanged\0bodyVelZChanged\0"
    "bodyPosXChanged\0bodyPosYChanged\0"
    "bodyPosZChanged\0pitchGimbalPresetChanged\0"
    "yawGimbalPresetChanged\0irIntegrationTimeChanged\0"
    "trackingCorrectionCmdChanged\0"
    "correctionFrameNumChanged\0"
    "correctedPitchPosChanged\0"
    "correctedYawPosChanged\0searchPitchRateChanged\0"
    "searchYawRateChanged\0gateSizeChanged\0"
    "osdSwitchChanged\0captureRefImgCmdChanged\0"
    "targetAltitudeChanged\0aircraftPitchChanged\0"
    "aircraftYawChanged\0aircraftRollChanged\0"
    "focalLengthChanged\0podPitchAngleChanged\0"
    "podYawAngleChanged\0satelliteMapScaleChanged\0"
    "podTypeChanged\0targetLongitudeChanged\0"
    "targetLatitudeChanged\0aircraftLongitudeChanged\0"
    "aircraftLatitudeChanged\0aircraftAltitudeChanged\0"
    "pixelSizeChanged\0requestSendData\0"
    "image_send_frame&\0frame\0buildFrame\0"
    "m_frameLength\0m_aFrameSequence\0"
    "m_seekerCtrlWord\0m_opticalParamCtrl\0"
    "m_templateIndex\0m_targetBackgroundType1\0"
    "m_targetBackgroundType2\0m_targetBackgroundType3\0"
    "m_targetBackgroundType4\0m_missileTargetDistance\0"
    "m_missileSpeed\0m_bodyPitchAngle\0"
    "m_bodyYawAngle\0m_bodyRollAngle\0"
    "m_bodyPitchRate\0m_bodyYawRate\0"
    "m_bodyRollRate\0m_bodyVelX\0m_bodyVelY\0"
    "m_bodyVelZ\0m_bodyPosX\0m_bodyPosY\0"
    "m_bodyPosZ\0m_pitchGimbalPreset\0"
    "m_yawGimbalPreset\0m_irIntegrationTime\0"
    "m_trackingCorrectionCmd\0m_correctionFrameNum\0"
    "m_correctedPitchPos\0m_correctedYawPos\0"
    "m_searchPitchRate\0m_searchYawRate\0"
    "m_gateSize\0m_osdSwitch\0m_captureRefImgCmd\0"
    "m_targetAltitude\0m_aircraftPitch\0"
    "m_aircraftYaw\0m_aircraftRoll\0m_focalLength\0"
    "m_podPitchAngle\0m_podYawAngle\0"
    "m_satelliteMapScale\0m_podType\0"
    "m_targetLongitude\0m_targetLatitude\0"
    "m_aircraftLongitude\0m_aircraftLatitude\0"
    "m_aircraftAltitude\0m_pixelSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageSendData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      53,   14, // methods
      50,  334, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      52,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  279,    2, 0x06 /* Public */,
       3,    0,  280,    2, 0x06 /* Public */,
       4,    0,  281,    2, 0x06 /* Public */,
       5,    0,  282,    2, 0x06 /* Public */,
       6,    0,  283,    2, 0x06 /* Public */,
       7,    0,  284,    2, 0x06 /* Public */,
       8,    0,  285,    2, 0x06 /* Public */,
       9,    0,  286,    2, 0x06 /* Public */,
      10,    0,  287,    2, 0x06 /* Public */,
      11,    0,  288,    2, 0x06 /* Public */,
      12,    0,  289,    2, 0x06 /* Public */,
      13,    0,  290,    2, 0x06 /* Public */,
      14,    0,  291,    2, 0x06 /* Public */,
      15,    0,  292,    2, 0x06 /* Public */,
      16,    0,  293,    2, 0x06 /* Public */,
      17,    0,  294,    2, 0x06 /* Public */,
      18,    0,  295,    2, 0x06 /* Public */,
      19,    0,  296,    2, 0x06 /* Public */,
      20,    0,  297,    2, 0x06 /* Public */,
      21,    0,  298,    2, 0x06 /* Public */,
      22,    0,  299,    2, 0x06 /* Public */,
      23,    0,  300,    2, 0x06 /* Public */,
      24,    0,  301,    2, 0x06 /* Public */,
      25,    0,  302,    2, 0x06 /* Public */,
      26,    0,  303,    2, 0x06 /* Public */,
      27,    0,  304,    2, 0x06 /* Public */,
      28,    0,  305,    2, 0x06 /* Public */,
      29,    0,  306,    2, 0x06 /* Public */,
      30,    0,  307,    2, 0x06 /* Public */,
      31,    0,  308,    2, 0x06 /* Public */,
      32,    0,  309,    2, 0x06 /* Public */,
      33,    0,  310,    2, 0x06 /* Public */,
      34,    0,  311,    2, 0x06 /* Public */,
      35,    0,  312,    2, 0x06 /* Public */,
      36,    0,  313,    2, 0x06 /* Public */,
      37,    0,  314,    2, 0x06 /* Public */,
      38,    0,  315,    2, 0x06 /* Public */,
      39,    0,  316,    2, 0x06 /* Public */,
      40,    0,  317,    2, 0x06 /* Public */,
      41,    0,  318,    2, 0x06 /* Public */,
      42,    0,  319,    2, 0x06 /* Public */,
      43,    0,  320,    2, 0x06 /* Public */,
      44,    0,  321,    2, 0x06 /* Public */,
      45,    0,  322,    2, 0x06 /* Public */,
      46,    0,  323,    2, 0x06 /* Public */,
      47,    0,  324,    2, 0x06 /* Public */,
      48,    0,  325,    2, 0x06 /* Public */,
      49,    0,  326,    2, 0x06 /* Public */,
      50,    0,  327,    2, 0x06 /* Public */,
      51,    0,  328,    2, 0x06 /* Public */,
      52,    0,  329,    2, 0x06 /* Public */,
      53,    1,  330,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      56,    0,  333,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 54,   55,

 // methods: parameters
    QMetaType::Void,

 // properties: name, type, flags
      57, QMetaType::Int, 0x00495003,
      58, QMetaType::Int, 0x00495003,
      59, QMetaType::Int, 0x00495003,
      60, QMetaType::Int, 0x00495003,
      61, QMetaType::Int, 0x00495003,
      62, QMetaType::Int, 0x00495003,
      63, QMetaType::Int, 0x00495003,
      64, QMetaType::Int, 0x00495003,
      65, QMetaType::Int, 0x00495003,
      66, QMetaType::Int, 0x00495003,
      67, QMetaType::Float, 0x00495003,
      68, QMetaType::Float, 0x00495003,
      69, QMetaType::Float, 0x00495003,
      70, QMetaType::Float, 0x00495003,
      71, QMetaType::Float, 0x00495003,
      72, QMetaType::Float, 0x00495003,
      73, QMetaType::Float, 0x00495003,
      74, QMetaType::Float, 0x00495003,
      75, QMetaType::Float, 0x00495003,
      76, QMetaType::Float, 0x00495003,
      77, QMetaType::Int, 0x00495003,
      78, QMetaType::Int, 0x00495003,
      79, QMetaType::Int, 0x00495003,
      80, QMetaType::Float, 0x00495003,
      81, QMetaType::Float, 0x00495003,
      82, QMetaType::Int, 0x00495003,
      83, QMetaType::Int, 0x00495003,
      84, QMetaType::Int, 0x00495003,
      85, QMetaType::Int, 0x00495003,
      86, QMetaType::Int, 0x00495003,
      87, QMetaType::Float, 0x00495003,
      88, QMetaType::Float, 0x00495003,
      89, QMetaType::Int, 0x00495003,
      90, QMetaType::Int, 0x00495003,
      91, QMetaType::Int, 0x00495003,
      92, QMetaType::Int, 0x00495003,
      93, QMetaType::Float, 0x00495003,
      94, QMetaType::Float, 0x00495003,
      95, QMetaType::Float, 0x00495003,
      96, QMetaType::Int, 0x00495003,
      97, QMetaType::Float, 0x00495003,
      98, QMetaType::Float, 0x00495003,
      99, QMetaType::Int, 0x00495003,
     100, QMetaType::Int, 0x00495003,
     101, QMetaType::Double, 0x00495003,
     102, QMetaType::Double, 0x00495003,
     103, QMetaType::Double, 0x00495003,
     104, QMetaType::Double, 0x00495003,
     105, QMetaType::Int, 0x00495003,
     106, QMetaType::Int, 0x00495003,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       6,
       7,
       8,
       9,
      10,
      11,
      12,
      13,
      14,
      15,
      16,
      17,
      18,
      19,
      20,
      21,
      22,
      23,
      24,
      25,
      26,
      27,
      28,
      29,
      30,
      31,
      32,
      33,
      34,
      35,
      36,
      37,
      38,
      39,
      40,
      41,
      42,
      43,
      44,
      45,
      46,
      47,
      48,
      49,
      50,

       0        // eod
};

void ImageSendData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageSendData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameLengthChanged(); break;
        case 1: _t->aFrameSequenceChanged(); break;
        case 2: _t->seekerCtrlWordChanged(); break;
        case 3: _t->opticalParamCtrlChanged(); break;
        case 4: _t->templateIndexChanged(); break;
        case 5: _t->targetBackgroundTypeChanged(); break;
        case 6: _t->targetBackgroundType1Changed(); break;
        case 7: _t->targetBackgroundType2Changed(); break;
        case 8: _t->targetBackgroundType3Changed(); break;
        case 9: _t->targetBackgroundType4Changed(); break;
        case 10: _t->missileTargetDistanceChanged(); break;
        case 11: _t->missileSpeedChanged(); break;
        case 12: _t->bodyPitchAngleChanged(); break;
        case 13: _t->bodyYawAngleChanged(); break;
        case 14: _t->bodyRollAngleChanged(); break;
        case 15: _t->bodyPitchRateChanged(); break;
        case 16: _t->bodyYawRateChanged(); break;
        case 17: _t->bodyRollRateChanged(); break;
        case 18: _t->bodyVelXChanged(); break;
        case 19: _t->bodyVelYChanged(); break;
        case 20: _t->bodyVelZChanged(); break;
        case 21: _t->bodyPosXChanged(); break;
        case 22: _t->bodyPosYChanged(); break;
        case 23: _t->bodyPosZChanged(); break;
        case 24: _t->pitchGimbalPresetChanged(); break;
        case 25: _t->yawGimbalPresetChanged(); break;
        case 26: _t->irIntegrationTimeChanged(); break;
        case 27: _t->trackingCorrectionCmdChanged(); break;
        case 28: _t->correctionFrameNumChanged(); break;
        case 29: _t->correctedPitchPosChanged(); break;
        case 30: _t->correctedYawPosChanged(); break;
        case 31: _t->searchPitchRateChanged(); break;
        case 32: _t->searchYawRateChanged(); break;
        case 33: _t->gateSizeChanged(); break;
        case 34: _t->osdSwitchChanged(); break;
        case 35: _t->captureRefImgCmdChanged(); break;
        case 36: _t->targetAltitudeChanged(); break;
        case 37: _t->aircraftPitchChanged(); break;
        case 38: _t->aircraftYawChanged(); break;
        case 39: _t->aircraftRollChanged(); break;
        case 40: _t->focalLengthChanged(); break;
        case 41: _t->podPitchAngleChanged(); break;
        case 42: _t->podYawAngleChanged(); break;
        case 43: _t->satelliteMapScaleChanged(); break;
        case 44: _t->podTypeChanged(); break;
        case 45: _t->targetLongitudeChanged(); break;
        case 46: _t->targetLatitudeChanged(); break;
        case 47: _t->aircraftLongitudeChanged(); break;
        case 48: _t->aircraftLatitudeChanged(); break;
        case 49: _t->aircraftAltitudeChanged(); break;
        case 50: _t->pixelSizeChanged(); break;
        case 51: _t->requestSendData((*reinterpret_cast< image_send_frame(*)>(_a[1]))); break;
        case 52: _t->buildFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::frameLengthChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aFrameSequenceChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::seekerCtrlWordChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::opticalParamCtrlChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::templateIndexChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetBackgroundTypeChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetBackgroundType1Changed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetBackgroundType2Changed)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetBackgroundType3Changed)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetBackgroundType4Changed)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::missileTargetDistanceChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::missileSpeedChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPitchAngleChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyYawAngleChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyRollAngleChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPitchRateChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyYawRateChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyRollRateChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelXChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelYChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelZChanged)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosXChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosYChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosZChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::pitchGimbalPresetChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::yawGimbalPresetChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::irIntegrationTimeChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::trackingCorrectionCmdChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctionFrameNumChanged)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctedPitchPosChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctedYawPosChanged)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::searchPitchRateChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::searchYawRateChanged)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::gateSizeChanged)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::osdSwitchChanged)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::captureRefImgCmdChanged)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetAltitudeChanged)) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftPitchChanged)) {
                *result = 37;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftYawChanged)) {
                *result = 38;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftRollChanged)) {
                *result = 39;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::focalLengthChanged)) {
                *result = 40;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podPitchAngleChanged)) {
                *result = 41;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podYawAngleChanged)) {
                *result = 42;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::satelliteMapScaleChanged)) {
                *result = 43;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podTypeChanged)) {
                *result = 44;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetLongitudeChanged)) {
                *result = 45;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetLatitudeChanged)) {
                *result = 46;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftLongitudeChanged)) {
                *result = 47;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftLatitudeChanged)) {
                *result = 48;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftAltitudeChanged)) {
                *result = 49;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::pixelSizeChanged)) {
                *result = 50;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)(image_send_frame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::requestSendData)) {
                *result = 51;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ImageSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->m_frameLength; break;
        case 1: *reinterpret_cast< int*>(_v) = _t->m_aFrameSequence; break;
        case 2: *reinterpret_cast< int*>(_v) = _t->m_seekerCtrlWord; break;
        case 3: *reinterpret_cast< int*>(_v) = _t->m_opticalParamCtrl; break;
        case 4: *reinterpret_cast< int*>(_v) = _t->m_templateIndex; break;
        case 5: *reinterpret_cast< int*>(_v) = _t->m_targetBackgroundType1; break;
        case 6: *reinterpret_cast< int*>(_v) = _t->m_targetBackgroundType2; break;
        case 7: *reinterpret_cast< int*>(_v) = _t->m_targetBackgroundType3; break;
        case 8: *reinterpret_cast< int*>(_v) = _t->m_targetBackgroundType4; break;
        case 9: *reinterpret_cast< int*>(_v) = _t->m_missileTargetDistance; break;
        case 10: *reinterpret_cast< float*>(_v) = _t->m_missileSpeed; break;
        case 11: *reinterpret_cast< float*>(_v) = _t->m_bodyPitchAngle; break;
        case 12: *reinterpret_cast< float*>(_v) = _t->m_bodyYawAngle; break;
        case 13: *reinterpret_cast< float*>(_v) = _t->m_bodyRollAngle; break;
        case 14: *reinterpret_cast< float*>(_v) = _t->m_bodyPitchRate; break;
        case 15: *reinterpret_cast< float*>(_v) = _t->m_bodyYawRate; break;
        case 16: *reinterpret_cast< float*>(_v) = _t->m_bodyRollRate; break;
        case 17: *reinterpret_cast< float*>(_v) = _t->m_bodyVelX; break;
        case 18: *reinterpret_cast< float*>(_v) = _t->m_bodyVelY; break;
        case 19: *reinterpret_cast< float*>(_v) = _t->m_bodyVelZ; break;
        case 20: *reinterpret_cast< int*>(_v) = _t->m_bodyPosX; break;
        case 21: *reinterpret_cast< int*>(_v) = _t->m_bodyPosY; break;
        case 22: *reinterpret_cast< int*>(_v) = _t->m_bodyPosZ; break;
        case 23: *reinterpret_cast< float*>(_v) = _t->m_pitchGimbalPreset; break;
        case 24: *reinterpret_cast< float*>(_v) = _t->m_yawGimbalPreset; break;
        case 25: *reinterpret_cast< int*>(_v) = _t->m_irIntegrationTime; break;
        case 26: *reinterpret_cast< int*>(_v) = _t->m_trackingCorrectionCmd; break;
        case 27: *reinterpret_cast< int*>(_v) = _t->m_correctionFrameNum; break;
        case 28: *reinterpret_cast< int*>(_v) = _t->m_correctedPitchPos; break;
        case 29: *reinterpret_cast< int*>(_v) = _t->m_correctedYawPos; break;
        case 30: *reinterpret_cast< float*>(_v) = _t->m_searchPitchRate; break;
        case 31: *reinterpret_cast< float*>(_v) = _t->m_searchYawRate; break;
        case 32: *reinterpret_cast< int*>(_v) = _t->m_gateSize; break;
        case 33: *reinterpret_cast< int*>(_v) = _t->m_osdSwitch; break;
        case 34: *reinterpret_cast< int*>(_v) = _t->m_captureRefImgCmd; break;
        case 35: *reinterpret_cast< int*>(_v) = _t->m_targetAltitude; break;
        case 36: *reinterpret_cast< float*>(_v) = _t->m_aircraftPitch; break;
        case 37: *reinterpret_cast< float*>(_v) = _t->m_aircraftYaw; break;
        case 38: *reinterpret_cast< float*>(_v) = _t->m_aircraftRoll; break;
        case 39: *reinterpret_cast< int*>(_v) = _t->m_focalLength; break;
        case 40: *reinterpret_cast< float*>(_v) = _t->m_podPitchAngle; break;
        case 41: *reinterpret_cast< float*>(_v) = _t->m_podYawAngle; break;
        case 42: *reinterpret_cast< int*>(_v) = _t->m_satelliteMapScale; break;
        case 43: *reinterpret_cast< int*>(_v) = _t->m_podType; break;
        case 44: *reinterpret_cast< double*>(_v) = _t->m_targetLongitude; break;
        case 45: *reinterpret_cast< double*>(_v) = _t->m_targetLatitude; break;
        case 46: *reinterpret_cast< double*>(_v) = _t->m_aircraftLongitude; break;
        case 47: *reinterpret_cast< double*>(_v) = _t->m_aircraftLatitude; break;
        case 48: *reinterpret_cast< int*>(_v) = _t->m_aircraftAltitude; break;
        case 49: *reinterpret_cast< int*>(_v) = _t->m_pixelSize; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ImageSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_frameLength != *reinterpret_cast< int*>(_v)) {
                _t->m_frameLength = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->frameLengthChanged();
            }
            break;
        case 1:
            if (_t->m_aFrameSequence != *reinterpret_cast< int*>(_v)) {
                _t->m_aFrameSequence = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->aFrameSequenceChanged();
            }
            break;
        case 2:
            if (_t->m_seekerCtrlWord != *reinterpret_cast< int*>(_v)) {
                _t->m_seekerCtrlWord = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->seekerCtrlWordChanged();
            }
            break;
        case 3:
            if (_t->m_opticalParamCtrl != *reinterpret_cast< int*>(_v)) {
                _t->m_opticalParamCtrl = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->opticalParamCtrlChanged();
            }
            break;
        case 4:
            if (_t->m_templateIndex != *reinterpret_cast< int*>(_v)) {
                _t->m_templateIndex = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->templateIndexChanged();
            }
            break;
        case 5:
            if (_t->m_targetBackgroundType1 != *reinterpret_cast< int*>(_v)) {
                _t->m_targetBackgroundType1 = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetBackgroundType1Changed();
            }
            break;
        case 6:
            if (_t->m_targetBackgroundType2 != *reinterpret_cast< int*>(_v)) {
                _t->m_targetBackgroundType2 = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetBackgroundType2Changed();
            }
            break;
        case 7:
            if (_t->m_targetBackgroundType3 != *reinterpret_cast< int*>(_v)) {
                _t->m_targetBackgroundType3 = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetBackgroundType3Changed();
            }
            break;
        case 8:
            if (_t->m_targetBackgroundType4 != *reinterpret_cast< int*>(_v)) {
                _t->m_targetBackgroundType4 = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetBackgroundType4Changed();
            }
            break;
        case 9:
            if (_t->m_missileTargetDistance != *reinterpret_cast< int*>(_v)) {
                _t->m_missileTargetDistance = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->missileTargetDistanceChanged();
            }
            break;
        case 10:
            if (_t->m_missileSpeed != *reinterpret_cast< float*>(_v)) {
                _t->m_missileSpeed = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->missileSpeedChanged();
            }
            break;
        case 11:
            if (_t->m_bodyPitchAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyPitchAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyPitchAngleChanged();
            }
            break;
        case 12:
            if (_t->m_bodyYawAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyYawAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyYawAngleChanged();
            }
            break;
        case 13:
            if (_t->m_bodyRollAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyRollAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyRollAngleChanged();
            }
            break;
        case 14:
            if (_t->m_bodyPitchRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyPitchRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyPitchRateChanged();
            }
            break;
        case 15:
            if (_t->m_bodyYawRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyYawRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyYawRateChanged();
            }
            break;
        case 16:
            if (_t->m_bodyRollRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyRollRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyRollRateChanged();
            }
            break;
        case 17:
            if (_t->m_bodyVelX != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelX = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelXChanged();
            }
            break;
        case 18:
            if (_t->m_bodyVelY != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelY = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelYChanged();
            }
            break;
        case 19:
            if (_t->m_bodyVelZ != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelZ = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelZChanged();
            }
            break;
        case 20:
            if (_t->m_bodyPosX != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosX = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosXChanged();
            }
            break;
        case 21:
            if (_t->m_bodyPosY != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosY = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosYChanged();
            }
            break;
        case 22:
            if (_t->m_bodyPosZ != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosZ = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosZChanged();
            }
            break;
        case 23:
            if (_t->m_pitchGimbalPreset != *reinterpret_cast< float*>(_v)) {
                _t->m_pitchGimbalPreset = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->pitchGimbalPresetChanged();
            }
            break;
        case 24:
            if (_t->m_yawGimbalPreset != *reinterpret_cast< float*>(_v)) {
                _t->m_yawGimbalPreset = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->yawGimbalPresetChanged();
            }
            break;
        case 25:
            if (_t->m_irIntegrationTime != *reinterpret_cast< int*>(_v)) {
                _t->m_irIntegrationTime = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->irIntegrationTimeChanged();
            }
            break;
        case 26:
            if (_t->m_trackingCorrectionCmd != *reinterpret_cast< int*>(_v)) {
                _t->m_trackingCorrectionCmd = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->trackingCorrectionCmdChanged();
            }
            break;
        case 27:
            if (_t->m_correctionFrameNum != *reinterpret_cast< int*>(_v)) {
                _t->m_correctionFrameNum = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctionFrameNumChanged();
            }
            break;
        case 28:
            if (_t->m_correctedPitchPos != *reinterpret_cast< int*>(_v)) {
                _t->m_correctedPitchPos = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctedPitchPosChanged();
            }
            break;
        case 29:
            if (_t->m_correctedYawPos != *reinterpret_cast< int*>(_v)) {
                _t->m_correctedYawPos = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctedYawPosChanged();
            }
            break;
        case 30:
            if (_t->m_searchPitchRate != *reinterpret_cast< float*>(_v)) {
                _t->m_searchPitchRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchPitchRateChanged();
            }
            break;
        case 31:
            if (_t->m_searchYawRate != *reinterpret_cast< float*>(_v)) {
                _t->m_searchYawRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchYawRateChanged();
            }
            break;
        case 32:
            if (_t->m_gateSize != *reinterpret_cast< int*>(_v)) {
                _t->m_gateSize = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->gateSizeChanged();
            }
            break;
        case 33:
            if (_t->m_osdSwitch != *reinterpret_cast< int*>(_v)) {
                _t->m_osdSwitch = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->osdSwitchChanged();
            }
            break;
        case 34:
            if (_t->m_captureRefImgCmd != *reinterpret_cast< int*>(_v)) {
                _t->m_captureRefImgCmd = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->captureRefImgCmdChanged();
            }
            break;
        case 35:
            if (_t->m_targetAltitude != *reinterpret_cast< int*>(_v)) {
                _t->m_targetAltitude = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetAltitudeChanged();
            }
            break;
        case 36:
            if (_t->m_aircraftPitch != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftPitch = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftPitchChanged();
            }
            break;
        case 37:
            if (_t->m_aircraftYaw != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftYaw = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftYawChanged();
            }
            break;
        case 38:
            if (_t->m_aircraftRoll != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftRoll = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftRollChanged();
            }
            break;
        case 39:
            if (_t->m_focalLength != *reinterpret_cast< int*>(_v)) {
                _t->m_focalLength = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->focalLengthChanged();
            }
            break;
        case 40:
            if (_t->m_podPitchAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_podPitchAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->podPitchAngleChanged();
            }
            break;
        case 41:
            if (_t->m_podYawAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_podYawAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->podYawAngleChanged();
            }
            break;
        case 42:
            if (_t->m_satelliteMapScale != *reinterpret_cast< int*>(_v)) {
                _t->m_satelliteMapScale = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->satelliteMapScaleChanged();
            }
            break;
        case 43:
            if (_t->m_podType != *reinterpret_cast< int*>(_v)) {
                _t->m_podType = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->podTypeChanged();
            }
            break;
        case 44:
            if (_t->m_targetLongitude != *reinterpret_cast< double*>(_v)) {
                _t->m_targetLongitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->targetLongitudeChanged();
            }
            break;
        case 45:
            if (_t->m_targetLatitude != *reinterpret_cast< double*>(_v)) {
                _t->m_targetLatitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->targetLatitudeChanged();
            }
            break;
        case 46:
            if (_t->m_aircraftLongitude != *reinterpret_cast< double*>(_v)) {
                _t->m_aircraftLongitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->aircraftLongitudeChanged();
            }
            break;
        case 47:
            if (_t->m_aircraftLatitude != *reinterpret_cast< double*>(_v)) {
                _t->m_aircraftLatitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->aircraftLatitudeChanged();
            }
            break;
        case 48:
            if (_t->m_aircraftAltitude != *reinterpret_cast< int*>(_v)) {
                _t->m_aircraftAltitude = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->aircraftAltitudeChanged();
            }
            break;
        case 49:
            if (_t->m_pixelSize != *reinterpret_cast< int*>(_v)) {
                _t->m_pixelSize = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->pixelSizeChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ImageSendData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageSendData.data,
    qt_meta_data_ImageSendData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageSendData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageSendData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageSendData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageSendData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 53)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 53;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 50;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 50;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ImageSendData::frameLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ImageSendData::aFrameSequenceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ImageSendData::seekerCtrlWordChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ImageSendData::opticalParamCtrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ImageSendData::templateIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ImageSendData::targetBackgroundTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ImageSendData::targetBackgroundType1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ImageSendData::targetBackgroundType2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ImageSendData::targetBackgroundType3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ImageSendData::targetBackgroundType4Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ImageSendData::missileTargetDistanceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ImageSendData::missileSpeedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ImageSendData::bodyPitchAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ImageSendData::bodyYawAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void ImageSendData::bodyRollAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ImageSendData::bodyPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void ImageSendData::bodyYawRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void ImageSendData::bodyRollRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void ImageSendData::bodyVelXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void ImageSendData::bodyVelYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ImageSendData::bodyVelZChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void ImageSendData::bodyPosXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void ImageSendData::bodyPosYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void ImageSendData::bodyPosZChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void ImageSendData::pitchGimbalPresetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void ImageSendData::yawGimbalPresetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void ImageSendData::irIntegrationTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void ImageSendData::trackingCorrectionCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void ImageSendData::correctionFrameNumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void ImageSendData::correctedPitchPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void ImageSendData::correctedYawPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void ImageSendData::searchPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void ImageSendData::searchYawRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void ImageSendData::gateSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void ImageSendData::osdSwitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void ImageSendData::captureRefImgCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void ImageSendData::targetAltitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}

// SIGNAL 37
void ImageSendData::aircraftPitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void ImageSendData::aircraftYawChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void ImageSendData::aircraftRollChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 39, nullptr);
}

// SIGNAL 40
void ImageSendData::focalLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}

// SIGNAL 41
void ImageSendData::podPitchAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 41, nullptr);
}

// SIGNAL 42
void ImageSendData::podYawAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 42, nullptr);
}

// SIGNAL 43
void ImageSendData::satelliteMapScaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 43, nullptr);
}

// SIGNAL 44
void ImageSendData::podTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 44, nullptr);
}

// SIGNAL 45
void ImageSendData::targetLongitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 45, nullptr);
}

// SIGNAL 46
void ImageSendData::targetLatitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 46, nullptr);
}

// SIGNAL 47
void ImageSendData::aircraftLongitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 47, nullptr);
}

// SIGNAL 48
void ImageSendData::aircraftLatitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 48, nullptr);
}

// SIGNAL 49
void ImageSendData::aircraftAltitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 49, nullptr);
}

// SIGNAL 50
void ImageSendData::pixelSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 50, nullptr);
}

// SIGNAL 51
void ImageSendData::requestSendData(image_send_frame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 51, _a);
}
struct qt_meta_stringdata_SerialPortImage_t {
    QByteArrayData data[29];
    char stringdata0[286];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortImage_t qt_meta_stringdata_SerialPortImage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SerialPortImage"
QT_MOC_LITERAL(1, 16, 10), // "portOpened"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "success"
QT_MOC_LITERAL(4, 36, 10), // "portClosed"
QT_MOC_LITERAL(5, 47, 9), // "portError"
QT_MOC_LITERAL(6, 57, 3), // "msg"
QT_MOC_LITERAL(7, 61, 12), // "portsChanged"
QT_MOC_LITERAL(8, 74, 5), // "ports"
QT_MOC_LITERAL(9, 80, 18), // "imageFrameReceived"
QT_MOC_LITERAL(10, 99, 7), // "rawData"
QT_MOC_LITERAL(11, 107, 6), // "dowork"
QT_MOC_LITERAL(12, 114, 16), // "init_crc16_table"
QT_MOC_LITERAL(13, 131, 8), // "uint16_t"
QT_MOC_LITERAL(14, 140, 4), // "poly"
QT_MOC_LITERAL(15, 145, 16), // "crc16_ccitt_fast"
QT_MOC_LITERAL(16, 162, 14), // "const uint8_t*"
QT_MOC_LITERAL(17, 177, 4), // "data"
QT_MOC_LITERAL(18, 182, 6), // "size_t"
QT_MOC_LITERAL(19, 189, 3), // "len"
QT_MOC_LITERAL(20, 193, 4), // "init"
QT_MOC_LITERAL(21, 198, 10), // "onOpenPort"
QT_MOC_LITERAL(22, 209, 8), // "portName"
QT_MOC_LITERAL(23, 218, 8), // "baudRate"
QT_MOC_LITERAL(24, 227, 11), // "onClosePort"
QT_MOC_LITERAL(25, 239, 11), // "onScanPorts"
QT_MOC_LITERAL(26, 251, 10), // "onSendData"
QT_MOC_LITERAL(27, 262, 17), // "image_send_frame&"
QT_MOC_LITERAL(28, 280, 5) // "frame"

    },
    "SerialPortImage\0portOpened\0\0success\0"
    "portClosed\0portError\0msg\0portsChanged\0"
    "ports\0imageFrameReceived\0rawData\0"
    "dowork\0init_crc16_table\0uint16_t\0poly\0"
    "crc16_ccitt_fast\0const uint8_t*\0data\0"
    "size_t\0len\0init\0onOpenPort\0portName\0"
    "baudRate\0onClosePort\0onScanPorts\0"
    "onSendData\0image_send_frame&\0frame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortImage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    0,   87,    2, 0x06 /* Public */,
       5,    1,   88,    2, 0x06 /* Public */,
       7,    1,   91,    2, 0x06 /* Public */,
       9,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   97,    2, 0x0a /* Public */,
      12,    1,   98,    2, 0x0a /* Public */,
      12,    0,  101,    2, 0x2a /* Public | MethodCloned */,
      15,    3,  102,    2, 0x0a /* Public */,
      15,    2,  109,    2, 0x2a /* Public | MethodCloned */,
      21,    2,  114,    2, 0x0a /* Public */,
      24,    0,  119,    2, 0x0a /* Public */,
      25,    0,  120,    2, 0x0a /* Public */,
      26,    1,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QStringList,    8,
    QMetaType::Void, QMetaType::QByteArray,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    0x80000000 | 13, 0x80000000 | 16, 0x80000000 | 18, 0x80000000 | 13,   17,   19,   20,
    0x80000000 | 13, 0x80000000 | 16, 0x80000000 | 18,   17,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 27,   28,

       0        // eod
};

void SerialPortImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortImage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->portClosed(); break;
        case 2: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->imageFrameReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->dowork(); break;
        case 6: _t->init_crc16_table((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 7: _t->init_crc16_table(); break;
        case 8: { uint16_t _r = _t->crc16_ccitt_fast((*reinterpret_cast< const uint8_t*(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< uint16_t*>(_a[0]) = std::move(_r); }  break;
        case 9: { uint16_t _r = _t->crc16_ccitt_fast((*reinterpret_cast< const uint8_t*(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< uint16_t*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->onClosePort(); break;
        case 12: _t->onScanPorts(); break;
        case 13: _t->onSendData((*reinterpret_cast< image_send_frame(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortImage::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortImage::portOpened)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortImage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortImage::portClosed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortImage::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortImage::portError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortImage::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortImage::portsChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortImage::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortImage::imageFrameReceived)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortImage::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortImage.data,
    qt_meta_data_SerialPortImage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortImage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortImage.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SerialPortImage::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortImage::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPortImage::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialPortImage::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortImage::imageFrameReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
