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
    QByteArrayData data[76];
    char stringdata0[1430];
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
QT_MOC_LITERAL(10, 197, 21), // "pitchLosAngVelChanged"
QT_MOC_LITERAL(11, 219, 19), // "yawLosAngVelChanged"
QT_MOC_LITERAL(12, 239, 27), // "targetBackgroundTypeChanged"
QT_MOC_LITERAL(13, 267, 23), // "opticalWorkStateChanged"
QT_MOC_LITERAL(14, 291, 22), // "pitchFrameAngleChanged"
QT_MOC_LITERAL(15, 314, 20), // "yawFrameAngleChanged"
QT_MOC_LITERAL(16, 335, 26), // "correctionCmdStatusChanged"
QT_MOC_LITERAL(17, 362, 25), // "correctionCmdCountChanged"
QT_MOC_LITERAL(18, 388, 16), // "pitchGyroChanged"
QT_MOC_LITERAL(19, 405, 14), // "yawGyroChanged"
QT_MOC_LITERAL(20, 420, 20), // "trackingStateChanged"
QT_MOC_LITERAL(21, 441, 19), // "trackerStateChanged"
QT_MOC_LITERAL(22, 461, 28), // "azimuthDeviationPixelChanged"
QT_MOC_LITERAL(23, 490, 26), // "pitchDeviationPixelChanged"
QT_MOC_LITERAL(24, 517, 24), // "azimuthGyroOutputChanged"
QT_MOC_LITERAL(25, 542, 22), // "pitchGyroOutputChanged"
QT_MOC_LITERAL(26, 565, 23), // "azimuthMasterCmdChanged"
QT_MOC_LITERAL(27, 589, 21), // "pitchMasterCmdChanged"
QT_MOC_LITERAL(28, 611, 24), // "platformSelfCheckChanged"
QT_MOC_LITERAL(29, 636, 23), // "servoRunningTimeChanged"
QT_MOC_LITERAL(30, 660, 16), // "servoStepChanged"
QT_MOC_LITERAL(31, 677, 23), // "infraredFrameNumChanged"
QT_MOC_LITERAL(32, 701, 16), // "cbhTv4405Changed"
QT_MOC_LITERAL(33, 718, 24), // "infraredFrameRateChanged"
QT_MOC_LITERAL(34, 743, 18), // "tvFrameRateChanged"
QT_MOC_LITERAL(35, 762, 15), // "gateSizeChanged"
QT_MOC_LITERAL(36, 778, 23), // "softwareVersion1Changed"
QT_MOC_LITERAL(37, 802, 23), // "softwareVersion2Changed"
QT_MOC_LITERAL(38, 826, 23), // "softwareVersion3Changed"
QT_MOC_LITERAL(39, 850, 11), // "frameLength"
QT_MOC_LITERAL(40, 862, 14), // "bFrameSequence"
QT_MOC_LITERAL(41, 877, 19), // "aFrameSequenceReply"
QT_MOC_LITERAL(42, 897, 15), // "aFrameValidFlag"
QT_MOC_LITERAL(43, 913, 15), // "seekerCtrlReply"
QT_MOC_LITERAL(44, 929, 17), // "opticalParamReply"
QT_MOC_LITERAL(45, 947, 18), // "currentWorkChannel"
QT_MOC_LITERAL(46, 966, 13), // "selfCheckFlag"
QT_MOC_LITERAL(47, 980, 14), // "pitchLosAngVel"
QT_MOC_LITERAL(48, 995, 12), // "yawLosAngVel"
QT_MOC_LITERAL(49, 1008, 20), // "targetBackgroundType"
QT_MOC_LITERAL(50, 1029, 16), // "opticalWorkState"
QT_MOC_LITERAL(51, 1046, 15), // "pitchFrameAngle"
QT_MOC_LITERAL(52, 1062, 13), // "yawFrameAngle"
QT_MOC_LITERAL(53, 1076, 19), // "correctionCmdStatus"
QT_MOC_LITERAL(54, 1096, 18), // "correctionCmdCount"
QT_MOC_LITERAL(55, 1115, 9), // "pitchGyro"
QT_MOC_LITERAL(56, 1125, 7), // "yawGyro"
QT_MOC_LITERAL(57, 1133, 13), // "trackingState"
QT_MOC_LITERAL(58, 1147, 12), // "trackerState"
QT_MOC_LITERAL(59, 1160, 21), // "azimuthDeviationPixel"
QT_MOC_LITERAL(60, 1182, 19), // "pitchDeviationPixel"
QT_MOC_LITERAL(61, 1202, 17), // "azimuthGyroOutput"
QT_MOC_LITERAL(62, 1220, 15), // "pitchGyroOutput"
QT_MOC_LITERAL(63, 1236, 16), // "azimuthMasterCmd"
QT_MOC_LITERAL(64, 1253, 14), // "pitchMasterCmd"
QT_MOC_LITERAL(65, 1268, 17), // "platformSelfCheck"
QT_MOC_LITERAL(66, 1286, 16), // "servoRunningTime"
QT_MOC_LITERAL(67, 1303, 9), // "servoStep"
QT_MOC_LITERAL(68, 1313, 16), // "infraredFrameNum"
QT_MOC_LITERAL(69, 1330, 9), // "cbhTv4405"
QT_MOC_LITERAL(70, 1340, 17), // "infraredFrameRate"
QT_MOC_LITERAL(71, 1358, 11), // "tvFrameRate"
QT_MOC_LITERAL(72, 1370, 8), // "gateSize"
QT_MOC_LITERAL(73, 1379, 16), // "softwareVersion1"
QT_MOC_LITERAL(74, 1396, 16), // "softwareVersion2"
QT_MOC_LITERAL(75, 1413, 16) // "softwareVersion3"

    },
    "ImageData\0frameLengthChanged\0\0"
    "bFrameSequenceChanged\0aFrameSequenceReplyChanged\0"
    "aFrameValidFlagChanged\0seekerCtrlReplyChanged\0"
    "opticalParamReplyChanged\0"
    "currentWorkChannelChanged\0"
    "selfCheckFlagChanged\0pitchLosAngVelChanged\0"
    "yawLosAngVelChanged\0targetBackgroundTypeChanged\0"
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
    "frameLength\0bFrameSequence\0"
    "aFrameSequenceReply\0aFrameValidFlag\0"
    "seekerCtrlReply\0opticalParamReply\0"
    "currentWorkChannel\0selfCheckFlag\0"
    "pitchLosAngVel\0yawLosAngVel\0"
    "targetBackgroundType\0opticalWorkState\0"
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
    "softwareVersion3"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
      37,  236, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      37,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  199,    2, 0x06 /* Public */,
       3,    0,  200,    2, 0x06 /* Public */,
       4,    0,  201,    2, 0x06 /* Public */,
       5,    0,  202,    2, 0x06 /* Public */,
       6,    0,  203,    2, 0x06 /* Public */,
       7,    0,  204,    2, 0x06 /* Public */,
       8,    0,  205,    2, 0x06 /* Public */,
       9,    0,  206,    2, 0x06 /* Public */,
      10,    0,  207,    2, 0x06 /* Public */,
      11,    0,  208,    2, 0x06 /* Public */,
      12,    0,  209,    2, 0x06 /* Public */,
      13,    0,  210,    2, 0x06 /* Public */,
      14,    0,  211,    2, 0x06 /* Public */,
      15,    0,  212,    2, 0x06 /* Public */,
      16,    0,  213,    2, 0x06 /* Public */,
      17,    0,  214,    2, 0x06 /* Public */,
      18,    0,  215,    2, 0x06 /* Public */,
      19,    0,  216,    2, 0x06 /* Public */,
      20,    0,  217,    2, 0x06 /* Public */,
      21,    0,  218,    2, 0x06 /* Public */,
      22,    0,  219,    2, 0x06 /* Public */,
      23,    0,  220,    2, 0x06 /* Public */,
      24,    0,  221,    2, 0x06 /* Public */,
      25,    0,  222,    2, 0x06 /* Public */,
      26,    0,  223,    2, 0x06 /* Public */,
      27,    0,  224,    2, 0x06 /* Public */,
      28,    0,  225,    2, 0x06 /* Public */,
      29,    0,  226,    2, 0x06 /* Public */,
      30,    0,  227,    2, 0x06 /* Public */,
      31,    0,  228,    2, 0x06 /* Public */,
      32,    0,  229,    2, 0x06 /* Public */,
      33,    0,  230,    2, 0x06 /* Public */,
      34,    0,  231,    2, 0x06 /* Public */,
      35,    0,  232,    2, 0x06 /* Public */,
      36,    0,  233,    2, 0x06 /* Public */,
      37,    0,  234,    2, 0x06 /* Public */,
      38,    0,  235,    2, 0x06 /* Public */,

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

 // properties: name, type, flags
      39, QMetaType::Int, 0x00495001,
      40, QMetaType::Int, 0x00495001,
      41, QMetaType::Int, 0x00495001,
      42, QMetaType::Int, 0x00495001,
      43, QMetaType::Int, 0x00495001,
      44, QMetaType::Int, 0x00495001,
      45, QMetaType::Int, 0x00495001,
      46, QMetaType::Int, 0x00495001,
      47, QMetaType::Float, 0x00495001,
      48, QMetaType::Float, 0x00495001,
      49, QMetaType::Int, 0x00495001,
      50, QMetaType::Int, 0x00495001,
      51, QMetaType::Float, 0x00495001,
      52, QMetaType::Float, 0x00495001,
      53, QMetaType::Int, 0x00495001,
      54, QMetaType::Int, 0x00495001,
      55, QMetaType::Float, 0x00495001,
      56, QMetaType::Float, 0x00495001,
      57, QMetaType::Int, 0x00495001,
      58, QMetaType::Int, 0x00495001,
      59, QMetaType::Int, 0x00495001,
      60, QMetaType::Int, 0x00495001,
      61, QMetaType::Float, 0x00495001,
      62, QMetaType::Float, 0x00495001,
      63, QMetaType::Float, 0x00495001,
      64, QMetaType::Float, 0x00495001,
      65, QMetaType::Int, 0x00495001,
      66, QMetaType::Int, 0x00495001,
      67, QMetaType::Int, 0x00495001,
      68, QMetaType::Int, 0x00495001,
      69, QMetaType::Int, 0x00495001,
      70, QMetaType::Int, 0x00495001,
      71, QMetaType::Int, 0x00495001,
      72, QMetaType::Int, 0x00495001,
      73, QMetaType::Int, 0x00495001,
      74, QMetaType::Int, 0x00495001,
      75, QMetaType::Int, 0x00495001,

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
        case 8: _t->pitchLosAngVelChanged(); break;
        case 9: _t->yawLosAngVelChanged(); break;
        case 10: _t->targetBackgroundTypeChanged(); break;
        case 11: _t->opticalWorkStateChanged(); break;
        case 12: _t->pitchFrameAngleChanged(); break;
        case 13: _t->yawFrameAngleChanged(); break;
        case 14: _t->correctionCmdStatusChanged(); break;
        case 15: _t->correctionCmdCountChanged(); break;
        case 16: _t->pitchGyroChanged(); break;
        case 17: _t->yawGyroChanged(); break;
        case 18: _t->trackingStateChanged(); break;
        case 19: _t->trackerStateChanged(); break;
        case 20: _t->azimuthDeviationPixelChanged(); break;
        case 21: _t->pitchDeviationPixelChanged(); break;
        case 22: _t->azimuthGyroOutputChanged(); break;
        case 23: _t->pitchGyroOutputChanged(); break;
        case 24: _t->azimuthMasterCmdChanged(); break;
        case 25: _t->pitchMasterCmdChanged(); break;
        case 26: _t->platformSelfCheckChanged(); break;
        case 27: _t->servoRunningTimeChanged(); break;
        case 28: _t->servoStepChanged(); break;
        case 29: _t->infraredFrameNumChanged(); break;
        case 30: _t->cbhTv4405Changed(); break;
        case 31: _t->infraredFrameRateChanged(); break;
        case 32: _t->tvFrameRateChanged(); break;
        case 33: _t->gateSizeChanged(); break;
        case 34: _t->softwareVersion1Changed(); break;
        case 35: _t->softwareVersion2Changed(); break;
        case 36: _t->softwareVersion3Changed(); break;
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchLosAngVelChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawLosAngVelChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::targetBackgroundTypeChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::opticalWorkStateChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchFrameAngleChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawFrameAngleChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::correctionCmdStatusChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::correctionCmdCountChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchGyroChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::yawGyroChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::trackingStateChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::trackerStateChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthDeviationPixelChanged)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchDeviationPixelChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthGyroOutputChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchGyroOutputChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::azimuthMasterCmdChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::pitchMasterCmdChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::platformSelfCheckChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::servoRunningTimeChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::servoStepChanged)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::infraredFrameNumChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::cbhTv4405Changed)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::infraredFrameRateChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::tvFrameRateChanged)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::gateSizeChanged)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion1Changed)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion2Changed)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (ImageData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageData::softwareVersion3Changed)) {
                *result = 36;
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
        case 8: *reinterpret_cast< float*>(_v) = _t->pitchLosAngVel(); break;
        case 9: *reinterpret_cast< float*>(_v) = _t->yawLosAngVel(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->targetBackgroundType(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->opticalWorkState(); break;
        case 12: *reinterpret_cast< float*>(_v) = _t->pitchFrameAngle(); break;
        case 13: *reinterpret_cast< float*>(_v) = _t->yawFrameAngle(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->correctionCmdStatus(); break;
        case 15: *reinterpret_cast< int*>(_v) = _t->correctionCmdCount(); break;
        case 16: *reinterpret_cast< float*>(_v) = _t->pitchGyro(); break;
        case 17: *reinterpret_cast< float*>(_v) = _t->yawGyro(); break;
        case 18: *reinterpret_cast< int*>(_v) = _t->trackingState(); break;
        case 19: *reinterpret_cast< int*>(_v) = _t->trackerState(); break;
        case 20: *reinterpret_cast< int*>(_v) = _t->azimuthDeviationPixel(); break;
        case 21: *reinterpret_cast< int*>(_v) = _t->pitchDeviationPixel(); break;
        case 22: *reinterpret_cast< float*>(_v) = _t->azimuthGyroOutput(); break;
        case 23: *reinterpret_cast< float*>(_v) = _t->pitchGyroOutput(); break;
        case 24: *reinterpret_cast< float*>(_v) = _t->azimuthMasterCmd(); break;
        case 25: *reinterpret_cast< float*>(_v) = _t->pitchMasterCmd(); break;
        case 26: *reinterpret_cast< int*>(_v) = _t->platformSelfCheck(); break;
        case 27: *reinterpret_cast< int*>(_v) = _t->servoRunningTime(); break;
        case 28: *reinterpret_cast< int*>(_v) = _t->servoStep(); break;
        case 29: *reinterpret_cast< int*>(_v) = _t->infraredFrameNum(); break;
        case 30: *reinterpret_cast< int*>(_v) = _t->cbhTv4405(); break;
        case 31: *reinterpret_cast< int*>(_v) = _t->infraredFrameRate(); break;
        case 32: *reinterpret_cast< int*>(_v) = _t->tvFrameRate(); break;
        case 33: *reinterpret_cast< int*>(_v) = _t->gateSize(); break;
        case 34: *reinterpret_cast< int*>(_v) = _t->softwareVersion1(); break;
        case 35: *reinterpret_cast< int*>(_v) = _t->softwareVersion2(); break;
        case 36: *reinterpret_cast< int*>(_v) = _t->softwareVersion3(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
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
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 37;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 37;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 37;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 37;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 37;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 37;
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
void ImageData::pitchLosAngVelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ImageData::yawLosAngVelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ImageData::targetBackgroundTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ImageData::opticalWorkStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ImageData::pitchFrameAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ImageData::yawFrameAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void ImageData::correctionCmdStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ImageData::correctionCmdCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void ImageData::pitchGyroChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void ImageData::yawGyroChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void ImageData::trackingStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void ImageData::trackerStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ImageData::azimuthDeviationPixelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void ImageData::pitchDeviationPixelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void ImageData::azimuthGyroOutputChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void ImageData::pitchGyroOutputChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void ImageData::azimuthMasterCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void ImageData::pitchMasterCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void ImageData::platformSelfCheckChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void ImageData::servoRunningTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void ImageData::servoStepChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void ImageData::infraredFrameNumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void ImageData::cbhTv4405Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void ImageData::infraredFrameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void ImageData::tvFrameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void ImageData::gateSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void ImageData::softwareVersion1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void ImageData::softwareVersion2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void ImageData::softwareVersion3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}
struct qt_meta_stringdata_ImageSendData_t {
    QByteArrayData data[97];
    char stringdata0[1785];
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
QT_MOC_LITERAL(8, 151, 28), // "missileTargetDistanceChanged"
QT_MOC_LITERAL(9, 180, 19), // "missileSpeedChanged"
QT_MOC_LITERAL(10, 200, 21), // "bodyPitchAngleChanged"
QT_MOC_LITERAL(11, 222, 19), // "bodyYawAngleChanged"
QT_MOC_LITERAL(12, 242, 20), // "bodyRollAngleChanged"
QT_MOC_LITERAL(13, 263, 20), // "bodyPitchRateChanged"
QT_MOC_LITERAL(14, 284, 18), // "bodyYawRateChanged"
QT_MOC_LITERAL(15, 303, 19), // "bodyRollRateChanged"
QT_MOC_LITERAL(16, 323, 15), // "bodyVelXChanged"
QT_MOC_LITERAL(17, 339, 15), // "bodyVelYChanged"
QT_MOC_LITERAL(18, 355, 15), // "bodyVelZChanged"
QT_MOC_LITERAL(19, 371, 15), // "bodyPosXChanged"
QT_MOC_LITERAL(20, 387, 15), // "bodyPosYChanged"
QT_MOC_LITERAL(21, 403, 15), // "bodyPosZChanged"
QT_MOC_LITERAL(22, 419, 24), // "pitchGimbalPresetChanged"
QT_MOC_LITERAL(23, 444, 22), // "yawGimbalPresetChanged"
QT_MOC_LITERAL(24, 467, 24), // "irIntegrationTimeChanged"
QT_MOC_LITERAL(25, 492, 28), // "trackingCorrectionCmdChanged"
QT_MOC_LITERAL(26, 521, 25), // "correctionFrameNumChanged"
QT_MOC_LITERAL(27, 547, 24), // "correctedPitchPosChanged"
QT_MOC_LITERAL(28, 572, 22), // "correctedYawPosChanged"
QT_MOC_LITERAL(29, 595, 22), // "searchPitchRateChanged"
QT_MOC_LITERAL(30, 618, 20), // "searchYawRateChanged"
QT_MOC_LITERAL(31, 639, 15), // "gateSizeChanged"
QT_MOC_LITERAL(32, 655, 16), // "osdSwitchChanged"
QT_MOC_LITERAL(33, 672, 23), // "captureRefImgCmdChanged"
QT_MOC_LITERAL(34, 696, 21), // "targetAltitudeChanged"
QT_MOC_LITERAL(35, 718, 20), // "aircraftPitchChanged"
QT_MOC_LITERAL(36, 739, 18), // "aircraftYawChanged"
QT_MOC_LITERAL(37, 758, 19), // "aircraftRollChanged"
QT_MOC_LITERAL(38, 778, 18), // "focalLengthChanged"
QT_MOC_LITERAL(39, 797, 20), // "podPitchAngleChanged"
QT_MOC_LITERAL(40, 818, 18), // "podYawAngleChanged"
QT_MOC_LITERAL(41, 837, 24), // "satelliteMapScaleChanged"
QT_MOC_LITERAL(42, 862, 14), // "podTypeChanged"
QT_MOC_LITERAL(43, 877, 22), // "targetLongitudeChanged"
QT_MOC_LITERAL(44, 900, 21), // "targetLatitudeChanged"
QT_MOC_LITERAL(45, 922, 24), // "aircraftLongitudeChanged"
QT_MOC_LITERAL(46, 947, 23), // "aircraftLatitudeChanged"
QT_MOC_LITERAL(47, 971, 23), // "aircraftAltitudeChanged"
QT_MOC_LITERAL(48, 995, 16), // "pixelSizeChanged"
QT_MOC_LITERAL(49, 1012, 10), // "buildFrame"
QT_MOC_LITERAL(50, 1023, 13), // "m_frameLength"
QT_MOC_LITERAL(51, 1037, 16), // "m_aFrameSequence"
QT_MOC_LITERAL(52, 1054, 16), // "m_seekerCtrlWord"
QT_MOC_LITERAL(53, 1071, 18), // "m_opticalParamCtrl"
QT_MOC_LITERAL(54, 1090, 15), // "m_templateIndex"
QT_MOC_LITERAL(55, 1106, 22), // "m_targetBackgroundType"
QT_MOC_LITERAL(56, 1129, 23), // "m_missileTargetDistance"
QT_MOC_LITERAL(57, 1153, 14), // "m_missileSpeed"
QT_MOC_LITERAL(58, 1168, 16), // "m_bodyPitchAngle"
QT_MOC_LITERAL(59, 1185, 14), // "m_bodyYawAngle"
QT_MOC_LITERAL(60, 1200, 15), // "m_bodyRollAngle"
QT_MOC_LITERAL(61, 1216, 15), // "m_bodyPitchRate"
QT_MOC_LITERAL(62, 1232, 13), // "m_bodyYawRate"
QT_MOC_LITERAL(63, 1246, 14), // "m_bodyRollRate"
QT_MOC_LITERAL(64, 1261, 10), // "m_bodyVelX"
QT_MOC_LITERAL(65, 1272, 10), // "m_bodyVelY"
QT_MOC_LITERAL(66, 1283, 10), // "m_bodyVelZ"
QT_MOC_LITERAL(67, 1294, 10), // "m_bodyPosX"
QT_MOC_LITERAL(68, 1305, 10), // "m_bodyPosY"
QT_MOC_LITERAL(69, 1316, 10), // "m_bodyPosZ"
QT_MOC_LITERAL(70, 1327, 19), // "m_pitchGimbalPreset"
QT_MOC_LITERAL(71, 1347, 17), // "m_yawGimbalPreset"
QT_MOC_LITERAL(72, 1365, 19), // "m_irIntegrationTime"
QT_MOC_LITERAL(73, 1385, 23), // "m_trackingCorrectionCmd"
QT_MOC_LITERAL(74, 1409, 20), // "m_correctionFrameNum"
QT_MOC_LITERAL(75, 1430, 19), // "m_correctedPitchPos"
QT_MOC_LITERAL(76, 1450, 17), // "m_correctedYawPos"
QT_MOC_LITERAL(77, 1468, 17), // "m_searchPitchRate"
QT_MOC_LITERAL(78, 1486, 15), // "m_searchYawRate"
QT_MOC_LITERAL(79, 1502, 10), // "m_gateSize"
QT_MOC_LITERAL(80, 1513, 11), // "m_osdSwitch"
QT_MOC_LITERAL(81, 1525, 18), // "m_captureRefImgCmd"
QT_MOC_LITERAL(82, 1544, 16), // "m_targetAltitude"
QT_MOC_LITERAL(83, 1561, 15), // "m_aircraftPitch"
QT_MOC_LITERAL(84, 1577, 13), // "m_aircraftYaw"
QT_MOC_LITERAL(85, 1591, 14), // "m_aircraftRoll"
QT_MOC_LITERAL(86, 1606, 13), // "m_focalLength"
QT_MOC_LITERAL(87, 1620, 15), // "m_podPitchAngle"
QT_MOC_LITERAL(88, 1636, 13), // "m_podYawAngle"
QT_MOC_LITERAL(89, 1650, 19), // "m_satelliteMapScale"
QT_MOC_LITERAL(90, 1670, 9), // "m_podType"
QT_MOC_LITERAL(91, 1680, 17), // "m_targetLongitude"
QT_MOC_LITERAL(92, 1698, 16), // "m_targetLatitude"
QT_MOC_LITERAL(93, 1715, 19), // "m_aircraftLongitude"
QT_MOC_LITERAL(94, 1735, 18), // "m_aircraftLatitude"
QT_MOC_LITERAL(95, 1754, 18), // "m_aircraftAltitude"
QT_MOC_LITERAL(96, 1773, 11) // "m_pixelSize"

    },
    "ImageSendData\0frameLengthChanged\0\0"
    "aFrameSequenceChanged\0seekerCtrlWordChanged\0"
    "opticalParamCtrlChanged\0templateIndexChanged\0"
    "targetBackgroundTypeChanged\0"
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
    "pixelSizeChanged\0buildFrame\0m_frameLength\0"
    "m_aFrameSequence\0m_seekerCtrlWord\0"
    "m_opticalParamCtrl\0m_templateIndex\0"
    "m_targetBackgroundType\0m_missileTargetDistance\0"
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
      48,   14, // methods
      47,  302, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      47,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  254,    2, 0x06 /* Public */,
       3,    0,  255,    2, 0x06 /* Public */,
       4,    0,  256,    2, 0x06 /* Public */,
       5,    0,  257,    2, 0x06 /* Public */,
       6,    0,  258,    2, 0x06 /* Public */,
       7,    0,  259,    2, 0x06 /* Public */,
       8,    0,  260,    2, 0x06 /* Public */,
       9,    0,  261,    2, 0x06 /* Public */,
      10,    0,  262,    2, 0x06 /* Public */,
      11,    0,  263,    2, 0x06 /* Public */,
      12,    0,  264,    2, 0x06 /* Public */,
      13,    0,  265,    2, 0x06 /* Public */,
      14,    0,  266,    2, 0x06 /* Public */,
      15,    0,  267,    2, 0x06 /* Public */,
      16,    0,  268,    2, 0x06 /* Public */,
      17,    0,  269,    2, 0x06 /* Public */,
      18,    0,  270,    2, 0x06 /* Public */,
      19,    0,  271,    2, 0x06 /* Public */,
      20,    0,  272,    2, 0x06 /* Public */,
      21,    0,  273,    2, 0x06 /* Public */,
      22,    0,  274,    2, 0x06 /* Public */,
      23,    0,  275,    2, 0x06 /* Public */,
      24,    0,  276,    2, 0x06 /* Public */,
      25,    0,  277,    2, 0x06 /* Public */,
      26,    0,  278,    2, 0x06 /* Public */,
      27,    0,  279,    2, 0x06 /* Public */,
      28,    0,  280,    2, 0x06 /* Public */,
      29,    0,  281,    2, 0x06 /* Public */,
      30,    0,  282,    2, 0x06 /* Public */,
      31,    0,  283,    2, 0x06 /* Public */,
      32,    0,  284,    2, 0x06 /* Public */,
      33,    0,  285,    2, 0x06 /* Public */,
      34,    0,  286,    2, 0x06 /* Public */,
      35,    0,  287,    2, 0x06 /* Public */,
      36,    0,  288,    2, 0x06 /* Public */,
      37,    0,  289,    2, 0x06 /* Public */,
      38,    0,  290,    2, 0x06 /* Public */,
      39,    0,  291,    2, 0x06 /* Public */,
      40,    0,  292,    2, 0x06 /* Public */,
      41,    0,  293,    2, 0x06 /* Public */,
      42,    0,  294,    2, 0x06 /* Public */,
      43,    0,  295,    2, 0x06 /* Public */,
      44,    0,  296,    2, 0x06 /* Public */,
      45,    0,  297,    2, 0x06 /* Public */,
      46,    0,  298,    2, 0x06 /* Public */,
      47,    0,  299,    2, 0x06 /* Public */,
      48,    0,  300,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      49,    0,  301,    2, 0x02 /* Public */,

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

 // methods: parameters
    QMetaType::QByteArray,

 // properties: name, type, flags
      50, QMetaType::Int, 0x00495003,
      51, QMetaType::Int, 0x00495003,
      52, QMetaType::Int, 0x00495003,
      53, QMetaType::Int, 0x00495003,
      54, QMetaType::Int, 0x00495003,
      55, QMetaType::Int, 0x00495003,
      56, QMetaType::Int, 0x00495003,
      57, QMetaType::Float, 0x00495003,
      58, QMetaType::Float, 0x00495003,
      59, QMetaType::Float, 0x00495003,
      60, QMetaType::Float, 0x00495003,
      61, QMetaType::Float, 0x00495003,
      62, QMetaType::Float, 0x00495003,
      63, QMetaType::Float, 0x00495003,
      64, QMetaType::Float, 0x00495003,
      65, QMetaType::Float, 0x00495003,
      66, QMetaType::Float, 0x00495003,
      67, QMetaType::Int, 0x00495003,
      68, QMetaType::Int, 0x00495003,
      69, QMetaType::Int, 0x00495003,
      70, QMetaType::Float, 0x00495003,
      71, QMetaType::Float, 0x00495003,
      72, QMetaType::Int, 0x00495003,
      73, QMetaType::Int, 0x00495003,
      74, QMetaType::Int, 0x00495003,
      75, QMetaType::Int, 0x00495003,
      76, QMetaType::Int, 0x00495003,
      77, QMetaType::Float, 0x00495003,
      78, QMetaType::Float, 0x00495003,
      79, QMetaType::Int, 0x00495003,
      80, QMetaType::Int, 0x00495003,
      81, QMetaType::Int, 0x00495003,
      82, QMetaType::Int, 0x00495003,
      83, QMetaType::Float, 0x00495003,
      84, QMetaType::Float, 0x00495003,
      85, QMetaType::Float, 0x00495003,
      86, QMetaType::Int, 0x00495003,
      87, QMetaType::Float, 0x00495003,
      88, QMetaType::Float, 0x00495003,
      89, QMetaType::Int, 0x00495003,
      90, QMetaType::Int, 0x00495003,
      91, QMetaType::Double, 0x00495003,
      92, QMetaType::Double, 0x00495003,
      93, QMetaType::Double, 0x00495003,
      94, QMetaType::Double, 0x00495003,
      95, QMetaType::Int, 0x00495003,
      96, QMetaType::Int, 0x00495003,

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
        case 6: _t->missileTargetDistanceChanged(); break;
        case 7: _t->missileSpeedChanged(); break;
        case 8: _t->bodyPitchAngleChanged(); break;
        case 9: _t->bodyYawAngleChanged(); break;
        case 10: _t->bodyRollAngleChanged(); break;
        case 11: _t->bodyPitchRateChanged(); break;
        case 12: _t->bodyYawRateChanged(); break;
        case 13: _t->bodyRollRateChanged(); break;
        case 14: _t->bodyVelXChanged(); break;
        case 15: _t->bodyVelYChanged(); break;
        case 16: _t->bodyVelZChanged(); break;
        case 17: _t->bodyPosXChanged(); break;
        case 18: _t->bodyPosYChanged(); break;
        case 19: _t->bodyPosZChanged(); break;
        case 20: _t->pitchGimbalPresetChanged(); break;
        case 21: _t->yawGimbalPresetChanged(); break;
        case 22: _t->irIntegrationTimeChanged(); break;
        case 23: _t->trackingCorrectionCmdChanged(); break;
        case 24: _t->correctionFrameNumChanged(); break;
        case 25: _t->correctedPitchPosChanged(); break;
        case 26: _t->correctedYawPosChanged(); break;
        case 27: _t->searchPitchRateChanged(); break;
        case 28: _t->searchYawRateChanged(); break;
        case 29: _t->gateSizeChanged(); break;
        case 30: _t->osdSwitchChanged(); break;
        case 31: _t->captureRefImgCmdChanged(); break;
        case 32: _t->targetAltitudeChanged(); break;
        case 33: _t->aircraftPitchChanged(); break;
        case 34: _t->aircraftYawChanged(); break;
        case 35: _t->aircraftRollChanged(); break;
        case 36: _t->focalLengthChanged(); break;
        case 37: _t->podPitchAngleChanged(); break;
        case 38: _t->podYawAngleChanged(); break;
        case 39: _t->satelliteMapScaleChanged(); break;
        case 40: _t->podTypeChanged(); break;
        case 41: _t->targetLongitudeChanged(); break;
        case 42: _t->targetLatitudeChanged(); break;
        case 43: _t->aircraftLongitudeChanged(); break;
        case 44: _t->aircraftLatitudeChanged(); break;
        case 45: _t->aircraftAltitudeChanged(); break;
        case 46: _t->pixelSizeChanged(); break;
        case 47: { QByteArray _r = _t->buildFrame();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::missileTargetDistanceChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::missileSpeedChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPitchAngleChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyYawAngleChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyRollAngleChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPitchRateChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyYawRateChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyRollRateChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelXChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelYChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyVelZChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosXChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosYChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::bodyPosZChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::pitchGimbalPresetChanged)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::yawGimbalPresetChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::irIntegrationTimeChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::trackingCorrectionCmdChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctionFrameNumChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctedPitchPosChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::correctedYawPosChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::searchPitchRateChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::searchYawRateChanged)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::gateSizeChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::osdSwitchChanged)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::captureRefImgCmdChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetAltitudeChanged)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftPitchChanged)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftYawChanged)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftRollChanged)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::focalLengthChanged)) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podPitchAngleChanged)) {
                *result = 37;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podYawAngleChanged)) {
                *result = 38;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::satelliteMapScaleChanged)) {
                *result = 39;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::podTypeChanged)) {
                *result = 40;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetLongitudeChanged)) {
                *result = 41;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::targetLatitudeChanged)) {
                *result = 42;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftLongitudeChanged)) {
                *result = 43;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftLatitudeChanged)) {
                *result = 44;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::aircraftAltitudeChanged)) {
                *result = 45;
                return;
            }
        }
        {
            using _t = void (ImageSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageSendData::pixelSizeChanged)) {
                *result = 46;
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
        case 5: *reinterpret_cast< int*>(_v) = _t->m_targetBackgroundType; break;
        case 6: *reinterpret_cast< int*>(_v) = _t->m_missileTargetDistance; break;
        case 7: *reinterpret_cast< float*>(_v) = _t->m_missileSpeed; break;
        case 8: *reinterpret_cast< float*>(_v) = _t->m_bodyPitchAngle; break;
        case 9: *reinterpret_cast< float*>(_v) = _t->m_bodyYawAngle; break;
        case 10: *reinterpret_cast< float*>(_v) = _t->m_bodyRollAngle; break;
        case 11: *reinterpret_cast< float*>(_v) = _t->m_bodyPitchRate; break;
        case 12: *reinterpret_cast< float*>(_v) = _t->m_bodyYawRate; break;
        case 13: *reinterpret_cast< float*>(_v) = _t->m_bodyRollRate; break;
        case 14: *reinterpret_cast< float*>(_v) = _t->m_bodyVelX; break;
        case 15: *reinterpret_cast< float*>(_v) = _t->m_bodyVelY; break;
        case 16: *reinterpret_cast< float*>(_v) = _t->m_bodyVelZ; break;
        case 17: *reinterpret_cast< int*>(_v) = _t->m_bodyPosX; break;
        case 18: *reinterpret_cast< int*>(_v) = _t->m_bodyPosY; break;
        case 19: *reinterpret_cast< int*>(_v) = _t->m_bodyPosZ; break;
        case 20: *reinterpret_cast< float*>(_v) = _t->m_pitchGimbalPreset; break;
        case 21: *reinterpret_cast< float*>(_v) = _t->m_yawGimbalPreset; break;
        case 22: *reinterpret_cast< int*>(_v) = _t->m_irIntegrationTime; break;
        case 23: *reinterpret_cast< int*>(_v) = _t->m_trackingCorrectionCmd; break;
        case 24: *reinterpret_cast< int*>(_v) = _t->m_correctionFrameNum; break;
        case 25: *reinterpret_cast< int*>(_v) = _t->m_correctedPitchPos; break;
        case 26: *reinterpret_cast< int*>(_v) = _t->m_correctedYawPos; break;
        case 27: *reinterpret_cast< float*>(_v) = _t->m_searchPitchRate; break;
        case 28: *reinterpret_cast< float*>(_v) = _t->m_searchYawRate; break;
        case 29: *reinterpret_cast< int*>(_v) = _t->m_gateSize; break;
        case 30: *reinterpret_cast< int*>(_v) = _t->m_osdSwitch; break;
        case 31: *reinterpret_cast< int*>(_v) = _t->m_captureRefImgCmd; break;
        case 32: *reinterpret_cast< int*>(_v) = _t->m_targetAltitude; break;
        case 33: *reinterpret_cast< float*>(_v) = _t->m_aircraftPitch; break;
        case 34: *reinterpret_cast< float*>(_v) = _t->m_aircraftYaw; break;
        case 35: *reinterpret_cast< float*>(_v) = _t->m_aircraftRoll; break;
        case 36: *reinterpret_cast< int*>(_v) = _t->m_focalLength; break;
        case 37: *reinterpret_cast< float*>(_v) = _t->m_podPitchAngle; break;
        case 38: *reinterpret_cast< float*>(_v) = _t->m_podYawAngle; break;
        case 39: *reinterpret_cast< int*>(_v) = _t->m_satelliteMapScale; break;
        case 40: *reinterpret_cast< int*>(_v) = _t->m_podType; break;
        case 41: *reinterpret_cast< double*>(_v) = _t->m_targetLongitude; break;
        case 42: *reinterpret_cast< double*>(_v) = _t->m_targetLatitude; break;
        case 43: *reinterpret_cast< double*>(_v) = _t->m_aircraftLongitude; break;
        case 44: *reinterpret_cast< double*>(_v) = _t->m_aircraftLatitude; break;
        case 45: *reinterpret_cast< int*>(_v) = _t->m_aircraftAltitude; break;
        case 46: *reinterpret_cast< int*>(_v) = _t->m_pixelSize; break;
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
            if (_t->m_targetBackgroundType != *reinterpret_cast< int*>(_v)) {
                _t->m_targetBackgroundType = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetBackgroundTypeChanged();
            }
            break;
        case 6:
            if (_t->m_missileTargetDistance != *reinterpret_cast< int*>(_v)) {
                _t->m_missileTargetDistance = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->missileTargetDistanceChanged();
            }
            break;
        case 7:
            if (_t->m_missileSpeed != *reinterpret_cast< float*>(_v)) {
                _t->m_missileSpeed = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->missileSpeedChanged();
            }
            break;
        case 8:
            if (_t->m_bodyPitchAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyPitchAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyPitchAngleChanged();
            }
            break;
        case 9:
            if (_t->m_bodyYawAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyYawAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyYawAngleChanged();
            }
            break;
        case 10:
            if (_t->m_bodyRollAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyRollAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyRollAngleChanged();
            }
            break;
        case 11:
            if (_t->m_bodyPitchRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyPitchRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyPitchRateChanged();
            }
            break;
        case 12:
            if (_t->m_bodyYawRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyYawRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyYawRateChanged();
            }
            break;
        case 13:
            if (_t->m_bodyRollRate != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyRollRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyRollRateChanged();
            }
            break;
        case 14:
            if (_t->m_bodyVelX != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelX = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelXChanged();
            }
            break;
        case 15:
            if (_t->m_bodyVelY != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelY = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelYChanged();
            }
            break;
        case 16:
            if (_t->m_bodyVelZ != *reinterpret_cast< float*>(_v)) {
                _t->m_bodyVelZ = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->bodyVelZChanged();
            }
            break;
        case 17:
            if (_t->m_bodyPosX != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosX = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosXChanged();
            }
            break;
        case 18:
            if (_t->m_bodyPosY != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosY = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosYChanged();
            }
            break;
        case 19:
            if (_t->m_bodyPosZ != *reinterpret_cast< int*>(_v)) {
                _t->m_bodyPosZ = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->bodyPosZChanged();
            }
            break;
        case 20:
            if (_t->m_pitchGimbalPreset != *reinterpret_cast< float*>(_v)) {
                _t->m_pitchGimbalPreset = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->pitchGimbalPresetChanged();
            }
            break;
        case 21:
            if (_t->m_yawGimbalPreset != *reinterpret_cast< float*>(_v)) {
                _t->m_yawGimbalPreset = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->yawGimbalPresetChanged();
            }
            break;
        case 22:
            if (_t->m_irIntegrationTime != *reinterpret_cast< int*>(_v)) {
                _t->m_irIntegrationTime = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->irIntegrationTimeChanged();
            }
            break;
        case 23:
            if (_t->m_trackingCorrectionCmd != *reinterpret_cast< int*>(_v)) {
                _t->m_trackingCorrectionCmd = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->trackingCorrectionCmdChanged();
            }
            break;
        case 24:
            if (_t->m_correctionFrameNum != *reinterpret_cast< int*>(_v)) {
                _t->m_correctionFrameNum = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctionFrameNumChanged();
            }
            break;
        case 25:
            if (_t->m_correctedPitchPos != *reinterpret_cast< int*>(_v)) {
                _t->m_correctedPitchPos = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctedPitchPosChanged();
            }
            break;
        case 26:
            if (_t->m_correctedYawPos != *reinterpret_cast< int*>(_v)) {
                _t->m_correctedYawPos = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->correctedYawPosChanged();
            }
            break;
        case 27:
            if (_t->m_searchPitchRate != *reinterpret_cast< float*>(_v)) {
                _t->m_searchPitchRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchPitchRateChanged();
            }
            break;
        case 28:
            if (_t->m_searchYawRate != *reinterpret_cast< float*>(_v)) {
                _t->m_searchYawRate = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchYawRateChanged();
            }
            break;
        case 29:
            if (_t->m_gateSize != *reinterpret_cast< int*>(_v)) {
                _t->m_gateSize = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->gateSizeChanged();
            }
            break;
        case 30:
            if (_t->m_osdSwitch != *reinterpret_cast< int*>(_v)) {
                _t->m_osdSwitch = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->osdSwitchChanged();
            }
            break;
        case 31:
            if (_t->m_captureRefImgCmd != *reinterpret_cast< int*>(_v)) {
                _t->m_captureRefImgCmd = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->captureRefImgCmdChanged();
            }
            break;
        case 32:
            if (_t->m_targetAltitude != *reinterpret_cast< int*>(_v)) {
                _t->m_targetAltitude = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->targetAltitudeChanged();
            }
            break;
        case 33:
            if (_t->m_aircraftPitch != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftPitch = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftPitchChanged();
            }
            break;
        case 34:
            if (_t->m_aircraftYaw != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftYaw = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftYawChanged();
            }
            break;
        case 35:
            if (_t->m_aircraftRoll != *reinterpret_cast< float*>(_v)) {
                _t->m_aircraftRoll = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->aircraftRollChanged();
            }
            break;
        case 36:
            if (_t->m_focalLength != *reinterpret_cast< int*>(_v)) {
                _t->m_focalLength = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->focalLengthChanged();
            }
            break;
        case 37:
            if (_t->m_podPitchAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_podPitchAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->podPitchAngleChanged();
            }
            break;
        case 38:
            if (_t->m_podYawAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_podYawAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->podYawAngleChanged();
            }
            break;
        case 39:
            if (_t->m_satelliteMapScale != *reinterpret_cast< int*>(_v)) {
                _t->m_satelliteMapScale = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->satelliteMapScaleChanged();
            }
            break;
        case 40:
            if (_t->m_podType != *reinterpret_cast< int*>(_v)) {
                _t->m_podType = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->podTypeChanged();
            }
            break;
        case 41:
            if (_t->m_targetLongitude != *reinterpret_cast< double*>(_v)) {
                _t->m_targetLongitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->targetLongitudeChanged();
            }
            break;
        case 42:
            if (_t->m_targetLatitude != *reinterpret_cast< double*>(_v)) {
                _t->m_targetLatitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->targetLatitudeChanged();
            }
            break;
        case 43:
            if (_t->m_aircraftLongitude != *reinterpret_cast< double*>(_v)) {
                _t->m_aircraftLongitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->aircraftLongitudeChanged();
            }
            break;
        case 44:
            if (_t->m_aircraftLatitude != *reinterpret_cast< double*>(_v)) {
                _t->m_aircraftLatitude = *reinterpret_cast< double*>(_v);
                Q_EMIT _t->aircraftLatitudeChanged();
            }
            break;
        case 45:
            if (_t->m_aircraftAltitude != *reinterpret_cast< int*>(_v)) {
                _t->m_aircraftAltitude = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->aircraftAltitudeChanged();
            }
            break;
        case 46:
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
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 48)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 48;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 47;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 47;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 47;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 47;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 47;
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
void ImageSendData::missileTargetDistanceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ImageSendData::missileSpeedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ImageSendData::bodyPitchAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ImageSendData::bodyYawAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ImageSendData::bodyRollAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ImageSendData::bodyPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ImageSendData::bodyYawRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ImageSendData::bodyRollRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void ImageSendData::bodyVelXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ImageSendData::bodyVelYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void ImageSendData::bodyVelZChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void ImageSendData::bodyPosXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void ImageSendData::bodyPosYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void ImageSendData::bodyPosZChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ImageSendData::pitchGimbalPresetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void ImageSendData::yawGimbalPresetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void ImageSendData::irIntegrationTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void ImageSendData::trackingCorrectionCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void ImageSendData::correctionFrameNumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void ImageSendData::correctedPitchPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void ImageSendData::correctedYawPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void ImageSendData::searchPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void ImageSendData::searchYawRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void ImageSendData::gateSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void ImageSendData::osdSwitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void ImageSendData::captureRefImgCmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void ImageSendData::targetAltitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void ImageSendData::aircraftPitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void ImageSendData::aircraftYawChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void ImageSendData::aircraftRollChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void ImageSendData::focalLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}

// SIGNAL 37
void ImageSendData::podPitchAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void ImageSendData::podYawAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void ImageSendData::satelliteMapScaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 39, nullptr);
}

// SIGNAL 40
void ImageSendData::podTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}

// SIGNAL 41
void ImageSendData::targetLongitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 41, nullptr);
}

// SIGNAL 42
void ImageSendData::targetLatitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 42, nullptr);
}

// SIGNAL 43
void ImageSendData::aircraftLongitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 43, nullptr);
}

// SIGNAL 44
void ImageSendData::aircraftLatitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 44, nullptr);
}

// SIGNAL 45
void ImageSendData::aircraftAltitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 45, nullptr);
}

// SIGNAL 46
void ImageSendData::pixelSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 46, nullptr);
}
struct qt_meta_stringdata_SerialPortImage_t {
    QByteArrayData data[5];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortImage_t qt_meta_stringdata_SerialPortImage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SerialPortImage"
QT_MOC_LITERAL(1, 16, 9), // "imageData"
QT_MOC_LITERAL(2, 26, 10), // "ImageData*"
QT_MOC_LITERAL(3, 37, 13), // "imageSendData"
QT_MOC_LITERAL(4, 51, 14) // "ImageSendData*"

    },
    "SerialPortImage\0imageData\0ImageData*\0"
    "imageSendData\0ImageSendData*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortImage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, 0x80000000 | 2, 0x00095409,
       3, 0x80000000 | 4, 0x00095409,

       0        // eod
};

void SerialPortImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ImageData* >(); break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ImageSendData* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SerialPortImage *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< ImageData**>(_v) = _t->imageData(); break;
        case 1: *reinterpret_cast< ImageSendData**>(_v) = _t->imageSendData(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
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
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
