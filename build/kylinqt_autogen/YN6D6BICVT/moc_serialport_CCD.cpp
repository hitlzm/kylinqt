/****************************************************************************
** Meta object code from reading C++ file 'serialport_CCD.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_CCD.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_CCD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CCDData_t {
    QByteArrayData data[91];
    char stringdata0[1306];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCDData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCDData_t qt_meta_stringdata_CCDData = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CCDData"
QT_MOC_LITERAL(1, 8, 15), // "portOpenChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 21), // "availablePortsChanged"
QT_MOC_LITERAL(4, 47, 18), // "errorStringChanged"
QT_MOC_LITERAL(5, 66, 15), // "requestOpenPort"
QT_MOC_LITERAL(6, 82, 8), // "portName"
QT_MOC_LITERAL(7, 91, 8), // "baudRate"
QT_MOC_LITERAL(8, 100, 16), // "requestClosePort"
QT_MOC_LITERAL(9, 117, 16), // "requestScanPorts"
QT_MOC_LITERAL(10, 134, 11), // "req30XFocus"
QT_MOC_LITERAL(11, 146, 10), // "req1XFocus"
QT_MOC_LITERAL(12, 157, 14), // "reqdigZoomopen"
QT_MOC_LITERAL(13, 172, 15), // "reqdigZoomclose"
QT_MOC_LITERAL(14, 188, 16), // "reqBacklightopen"
QT_MOC_LITERAL(15, 205, 17), // "reqBacklightclose"
QT_MOC_LITERAL(16, 223, 19), // "reqResolutionchange"
QT_MOC_LITERAL(17, 243, 5), // "index"
QT_MOC_LITERAL(18, 249, 11), // "reqZoomMove"
QT_MOC_LITERAL(19, 261, 9), // "direction"
QT_MOC_LITERAL(20, 271, 11), // "reqZoomStop"
QT_MOC_LITERAL(21, 283, 12), // "reqFocusMode"
QT_MOC_LITERAL(22, 296, 4), // "mode"
QT_MOC_LITERAL(23, 301, 12), // "reqFocusMove"
QT_MOC_LITERAL(24, 314, 12), // "reqFocusStop"
QT_MOC_LITERAL(25, 327, 15), // "reqDefogSetting"
QT_MOC_LITERAL(26, 343, 7), // "enabled"
QT_MOC_LITERAL(27, 351, 5), // "level"
QT_MOC_LITERAL(28, 357, 16), // "reqHeatHazeLevel"
QT_MOC_LITERAL(29, 374, 24), // "reqNoiseReductionSetting"
QT_MOC_LITERAL(30, 399, 23), // "reqStabilizationSetting"
QT_MOC_LITERAL(31, 423, 19), // "reqWhiteBalanceMode"
QT_MOC_LITERAL(32, 443, 19), // "reqWhiteBalanceGain"
QT_MOC_LITERAL(33, 463, 3), // "red"
QT_MOC_LITERAL(34, 467, 4), // "gain"
QT_MOC_LITERAL(35, 472, 16), // "focusModeChanged"
QT_MOC_LITERAL(36, 489, 19), // "defogEnabledChanged"
QT_MOC_LITERAL(37, 509, 17), // "defogLevelChanged"
QT_MOC_LITERAL(38, 527, 20), // "heatHazeLevelChanged"
QT_MOC_LITERAL(39, 548, 28), // "noiseReductionEnabledChanged"
QT_MOC_LITERAL(40, 577, 26), // "noiseReductionLevelChanged"
QT_MOC_LITERAL(41, 604, 27), // "stabilizationEnabledChanged"
QT_MOC_LITERAL(42, 632, 25), // "stabilizationLevelChanged"
QT_MOC_LITERAL(43, 658, 22), // "resolutionIndexChanged"
QT_MOC_LITERAL(44, 681, 23), // "whiteBalanceModeChanged"
QT_MOC_LITERAL(45, 705, 14), // "redGainChanged"
QT_MOC_LITERAL(46, 720, 15), // "blueGainChanged"
QT_MOC_LITERAL(47, 736, 18), // "backlightOnChanged"
QT_MOC_LITERAL(48, 755, 11), // "setPortOpen"
QT_MOC_LITERAL(49, 767, 4), // "open"
QT_MOC_LITERAL(50, 772, 11), // "setPortList"
QT_MOC_LITERAL(51, 784, 5), // "ports"
QT_MOC_LITERAL(52, 790, 8), // "setError"
QT_MOC_LITERAL(53, 799, 3), // "msg"
QT_MOC_LITERAL(54, 803, 8), // "openPort"
QT_MOC_LITERAL(55, 812, 9), // "closePort"
QT_MOC_LITERAL(56, 822, 9), // "scanPorts"
QT_MOC_LITERAL(57, 832, 9), // "startZoom"
QT_MOC_LITERAL(58, 842, 8), // "stopZoom"
QT_MOC_LITERAL(59, 851, 12), // "setFocusMode"
QT_MOC_LITERAL(60, 864, 10), // "startFocus"
QT_MOC_LITERAL(61, 875, 9), // "stopFocus"
QT_MOC_LITERAL(62, 885, 8), // "setDefog"
QT_MOC_LITERAL(63, 894, 13), // "setDefogLevel"
QT_MOC_LITERAL(64, 908, 16), // "setHeatHazeLevel"
QT_MOC_LITERAL(65, 925, 17), // "setNoiseReduction"
QT_MOC_LITERAL(66, 943, 22), // "setNoiseReductionLevel"
QT_MOC_LITERAL(67, 966, 16), // "setStabilization"
QT_MOC_LITERAL(68, 983, 21), // "setStabilizationLevel"
QT_MOC_LITERAL(69, 1005, 13), // "setResolution"
QT_MOC_LITERAL(70, 1019, 19), // "setWhiteBalanceMode"
QT_MOC_LITERAL(71, 1039, 10), // "setRedGain"
QT_MOC_LITERAL(72, 1050, 11), // "setBlueGain"
QT_MOC_LITERAL(73, 1062, 12), // "setBacklight"
QT_MOC_LITERAL(74, 1075, 2), // "on"
QT_MOC_LITERAL(75, 1078, 8), // "portOpen"
QT_MOC_LITERAL(76, 1087, 14), // "availablePorts"
QT_MOC_LITERAL(77, 1102, 11), // "errorString"
QT_MOC_LITERAL(78, 1114, 9), // "focusMode"
QT_MOC_LITERAL(79, 1124, 12), // "defogEnabled"
QT_MOC_LITERAL(80, 1137, 10), // "defogLevel"
QT_MOC_LITERAL(81, 1148, 13), // "heatHazeLevel"
QT_MOC_LITERAL(82, 1162, 21), // "noiseReductionEnabled"
QT_MOC_LITERAL(83, 1184, 19), // "noiseReductionLevel"
QT_MOC_LITERAL(84, 1204, 20), // "stabilizationEnabled"
QT_MOC_LITERAL(85, 1225, 18), // "stabilizationLevel"
QT_MOC_LITERAL(86, 1244, 15), // "resolutionIndex"
QT_MOC_LITERAL(87, 1260, 16), // "whiteBalanceMode"
QT_MOC_LITERAL(88, 1277, 7), // "redGain"
QT_MOC_LITERAL(89, 1285, 8), // "blueGain"
QT_MOC_LITERAL(90, 1294, 11) // "backlightOn"

    },
    "CCDData\0portOpenChanged\0\0availablePortsChanged\0"
    "errorStringChanged\0requestOpenPort\0"
    "portName\0baudRate\0requestClosePort\0"
    "requestScanPorts\0req30XFocus\0req1XFocus\0"
    "reqdigZoomopen\0reqdigZoomclose\0"
    "reqBacklightopen\0reqBacklightclose\0"
    "reqResolutionchange\0index\0reqZoomMove\0"
    "direction\0reqZoomStop\0reqFocusMode\0"
    "mode\0reqFocusMove\0reqFocusStop\0"
    "reqDefogSetting\0enabled\0level\0"
    "reqHeatHazeLevel\0reqNoiseReductionSetting\0"
    "reqStabilizationSetting\0reqWhiteBalanceMode\0"
    "reqWhiteBalanceGain\0red\0gain\0"
    "focusModeChanged\0defogEnabledChanged\0"
    "defogLevelChanged\0heatHazeLevelChanged\0"
    "noiseReductionEnabledChanged\0"
    "noiseReductionLevelChanged\0"
    "stabilizationEnabledChanged\0"
    "stabilizationLevelChanged\0"
    "resolutionIndexChanged\0whiteBalanceModeChanged\0"
    "redGainChanged\0blueGainChanged\0"
    "backlightOnChanged\0setPortOpen\0open\0"
    "setPortList\0ports\0setError\0msg\0openPort\0"
    "closePort\0scanPorts\0startZoom\0stopZoom\0"
    "setFocusMode\0startFocus\0stopFocus\0"
    "setDefog\0setDefogLevel\0setHeatHazeLevel\0"
    "setNoiseReduction\0setNoiseReductionLevel\0"
    "setStabilization\0setStabilizationLevel\0"
    "setResolution\0setWhiteBalanceMode\0"
    "setRedGain\0setBlueGain\0setBacklight\0"
    "on\0portOpen\0availablePorts\0errorString\0"
    "focusMode\0defogEnabled\0defogLevel\0"
    "heatHazeLevel\0noiseReductionEnabled\0"
    "noiseReductionLevel\0stabilizationEnabled\0"
    "stabilizationLevel\0resolutionIndex\0"
    "whiteBalanceMode\0redGain\0blueGain\0"
    "backlightOn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCDData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      60,   14, // methods
      16,  446, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      37,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  314,    2, 0x06 /* Public */,
       3,    0,  315,    2, 0x06 /* Public */,
       4,    0,  316,    2, 0x06 /* Public */,
       5,    2,  317,    2, 0x06 /* Public */,
       8,    0,  322,    2, 0x06 /* Public */,
       9,    0,  323,    2, 0x06 /* Public */,
      10,    0,  324,    2, 0x06 /* Public */,
      11,    0,  325,    2, 0x06 /* Public */,
      12,    0,  326,    2, 0x06 /* Public */,
      13,    0,  327,    2, 0x06 /* Public */,
      14,    0,  328,    2, 0x06 /* Public */,
      15,    0,  329,    2, 0x06 /* Public */,
      16,    1,  330,    2, 0x06 /* Public */,
      18,    1,  333,    2, 0x06 /* Public */,
      20,    0,  336,    2, 0x06 /* Public */,
      21,    1,  337,    2, 0x06 /* Public */,
      23,    1,  340,    2, 0x06 /* Public */,
      24,    0,  343,    2, 0x06 /* Public */,
      25,    2,  344,    2, 0x06 /* Public */,
      28,    1,  349,    2, 0x06 /* Public */,
      29,    2,  352,    2, 0x06 /* Public */,
      30,    2,  357,    2, 0x06 /* Public */,
      31,    1,  362,    2, 0x06 /* Public */,
      32,    2,  365,    2, 0x06 /* Public */,
      35,    0,  370,    2, 0x06 /* Public */,
      36,    0,  371,    2, 0x06 /* Public */,
      37,    0,  372,    2, 0x06 /* Public */,
      38,    0,  373,    2, 0x06 /* Public */,
      39,    0,  374,    2, 0x06 /* Public */,
      40,    0,  375,    2, 0x06 /* Public */,
      41,    0,  376,    2, 0x06 /* Public */,
      42,    0,  377,    2, 0x06 /* Public */,
      43,    0,  378,    2, 0x06 /* Public */,
      44,    0,  379,    2, 0x06 /* Public */,
      45,    0,  380,    2, 0x06 /* Public */,
      46,    0,  381,    2, 0x06 /* Public */,
      47,    0,  382,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      48,    1,  383,    2, 0x0a /* Public */,
      50,    1,  386,    2, 0x0a /* Public */,
      52,    1,  389,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      54,    2,  392,    2, 0x02 /* Public */,
      55,    0,  397,    2, 0x02 /* Public */,
      56,    0,  398,    2, 0x02 /* Public */,
      57,    1,  399,    2, 0x02 /* Public */,
      58,    0,  402,    2, 0x02 /* Public */,
      59,    1,  403,    2, 0x02 /* Public */,
      60,    1,  406,    2, 0x02 /* Public */,
      61,    0,  409,    2, 0x02 /* Public */,
      62,    1,  410,    2, 0x02 /* Public */,
      63,    1,  413,    2, 0x02 /* Public */,
      64,    1,  416,    2, 0x02 /* Public */,
      65,    1,  419,    2, 0x02 /* Public */,
      66,    1,  422,    2, 0x02 /* Public */,
      67,    1,  425,    2, 0x02 /* Public */,
      68,    1,  428,    2, 0x02 /* Public */,
      69,    1,  431,    2, 0x02 /* Public */,
      70,    1,  434,    2, 0x02 /* Public */,
      71,    1,  437,    2, 0x02 /* Public */,
      72,    1,  440,    2, 0x02 /* Public */,
      73,    1,  443,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   33,   34,
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

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::QStringList,   51,
    QMetaType::Void, QMetaType::QString,   53,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void, QMetaType::Bool,   74,

 // properties: name, type, flags
      75, QMetaType::Bool, 0x00495001,
      76, QMetaType::QStringList, 0x00495001,
      77, QMetaType::QString, 0x00495001,
      78, QMetaType::Int, 0x00495001,
      79, QMetaType::Bool, 0x00495001,
      80, QMetaType::Int, 0x00495001,
      81, QMetaType::Int, 0x00495001,
      82, QMetaType::Bool, 0x00495001,
      83, QMetaType::Int, 0x00495001,
      84, QMetaType::Bool, 0x00495001,
      85, QMetaType::Int, 0x00495001,
      86, QMetaType::Int, 0x00495001,
      87, QMetaType::Int, 0x00495001,
      88, QMetaType::Int, 0x00495001,
      89, QMetaType::Int, 0x00495001,
      90, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
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

void CCDData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CCDData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->portOpenChanged(); break;
        case 1: _t->availablePortsChanged(); break;
        case 2: _t->errorStringChanged(); break;
        case 3: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->requestClosePort(); break;
        case 5: _t->requestScanPorts(); break;
        case 6: _t->req30XFocus(); break;
        case 7: _t->req1XFocus(); break;
        case 8: _t->reqdigZoomopen(); break;
        case 9: _t->reqdigZoomclose(); break;
        case 10: _t->reqBacklightopen(); break;
        case 11: _t->reqBacklightclose(); break;
        case 12: _t->reqResolutionchange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->reqZoomMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->reqZoomStop(); break;
        case 15: _t->reqFocusMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->reqFocusMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->reqFocusStop(); break;
        case 18: _t->reqDefogSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: _t->reqHeatHazeLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->reqNoiseReductionSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->reqStabilizationSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->reqWhiteBalanceMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->reqWhiteBalanceGain((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->focusModeChanged(); break;
        case 25: _t->defogEnabledChanged(); break;
        case 26: _t->defogLevelChanged(); break;
        case 27: _t->heatHazeLevelChanged(); break;
        case 28: _t->noiseReductionEnabledChanged(); break;
        case 29: _t->noiseReductionLevelChanged(); break;
        case 30: _t->stabilizationEnabledChanged(); break;
        case 31: _t->stabilizationLevelChanged(); break;
        case 32: _t->resolutionIndexChanged(); break;
        case 33: _t->whiteBalanceModeChanged(); break;
        case 34: _t->redGainChanged(); break;
        case 35: _t->blueGainChanged(); break;
        case 36: _t->backlightOnChanged(); break;
        case 37: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 39: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 40: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 41: _t->closePort(); break;
        case 42: _t->scanPorts(); break;
        case 43: _t->startZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->stopZoom(); break;
        case 45: _t->setFocusMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->startFocus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->stopFocus(); break;
        case 48: _t->setDefog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 49: _t->setDefogLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->setHeatHazeLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->setNoiseReduction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->setNoiseReductionLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->setStabilization((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->setStabilizationLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 55: _t->setResolution((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 56: _t->setWhiteBalanceMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 57: _t->setRedGain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 58: _t->setBlueGain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 59: _t->setBacklight((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::portOpenChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::availablePortsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::errorStringChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::requestOpenPort)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::requestClosePort)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::requestScanPorts)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::req30XFocus)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::req1XFocus)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqdigZoomopen)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqdigZoomclose)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqBacklightopen)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqBacklightclose)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqResolutionchange)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqZoomMove)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqZoomStop)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqFocusMode)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqFocusMove)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqFocusStop)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqDefogSetting)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqHeatHazeLevel)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqNoiseReductionSetting)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqStabilizationSetting)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqWhiteBalanceMode)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (CCDData::*)(bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqWhiteBalanceGain)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::focusModeChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::defogEnabledChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::defogLevelChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::heatHazeLevelChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::noiseReductionEnabledChanged)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::noiseReductionLevelChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::stabilizationEnabledChanged)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::stabilizationLevelChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::resolutionIndexChanged)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::whiteBalanceModeChanged)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::redGainChanged)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::blueGainChanged)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::backlightOnChanged)) {
                *result = 36;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CCDData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->focusMode(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->defogEnabled(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->defogLevel(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->heatHazeLevel(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->noiseReductionEnabled(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->noiseReductionLevel(); break;
        case 9: *reinterpret_cast< bool*>(_v) = _t->stabilizationEnabled(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->stabilizationLevel(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->resolutionIndex(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->whiteBalanceMode(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->redGain(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->blueGain(); break;
        case 15: *reinterpret_cast< bool*>(_v) = _t->backlightOn(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CCDData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CCDData.data,
    qt_meta_data_CCDData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CCDData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCDData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CCDData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CCDData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 16;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CCDData::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CCDData::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CCDData::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CCDData::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CCDData::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CCDData::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CCDData::req30XFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CCDData::req1XFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CCDData::reqdigZoomopen()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CCDData::reqdigZoomclose()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void CCDData::reqBacklightopen()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void CCDData::reqBacklightclose()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void CCDData::reqResolutionchange(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CCDData::reqZoomMove(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CCDData::reqZoomStop()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void CCDData::reqFocusMode(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CCDData::reqFocusMove(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CCDData::reqFocusStop()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void CCDData::reqDefogSetting(bool _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CCDData::reqHeatHazeLevel(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CCDData::reqNoiseReductionSetting(bool _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CCDData::reqStabilizationSetting(bool _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CCDData::reqWhiteBalanceMode(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CCDData::reqWhiteBalanceGain(bool _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CCDData::focusModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void CCDData::defogEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void CCDData::defogLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void CCDData::heatHazeLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void CCDData::noiseReductionEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void CCDData::noiseReductionLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void CCDData::stabilizationEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void CCDData::stabilizationLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void CCDData::resolutionIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void CCDData::whiteBalanceModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void CCDData::redGainChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 34, nullptr);
}

// SIGNAL 35
void CCDData::blueGainChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void CCDData::backlightOnChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}
struct qt_meta_stringdata_SerialPortCCD_t {
    QByteArrayData data[39];
    char stringdata0[487];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortCCD_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortCCD_t qt_meta_stringdata_SerialPortCCD = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SerialPortCCD"
QT_MOC_LITERAL(1, 14, 10), // "portOpened"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "success"
QT_MOC_LITERAL(4, 34, 10), // "portClosed"
QT_MOC_LITERAL(5, 45, 9), // "portError"
QT_MOC_LITERAL(6, 55, 3), // "msg"
QT_MOC_LITERAL(7, 59, 12), // "portsChanged"
QT_MOC_LITERAL(8, 72, 5), // "ports"
QT_MOC_LITERAL(9, 78, 10), // "onOpenPort"
QT_MOC_LITERAL(10, 89, 8), // "portName"
QT_MOC_LITERAL(11, 98, 8), // "baudRate"
QT_MOC_LITERAL(12, 107, 11), // "onClosePort"
QT_MOC_LITERAL(13, 119, 11), // "onScanPorts"
QT_MOC_LITERAL(14, 131, 12), // "send30XFocus"
QT_MOC_LITERAL(15, 144, 11), // "send1XFocus"
QT_MOC_LITERAL(16, 156, 15), // "senddigZoomopen"
QT_MOC_LITERAL(17, 172, 16), // "senddigZoomclose"
QT_MOC_LITERAL(18, 189, 17), // "sendBacklightopen"
QT_MOC_LITERAL(19, 207, 18), // "sendBacklightclose"
QT_MOC_LITERAL(20, 226, 20), // "sendResolutionchange"
QT_MOC_LITERAL(21, 247, 5), // "index"
QT_MOC_LITERAL(22, 253, 12), // "sendZoomMove"
QT_MOC_LITERAL(23, 266, 9), // "direction"
QT_MOC_LITERAL(24, 276, 12), // "sendZoomStop"
QT_MOC_LITERAL(25, 289, 13), // "sendFocusMode"
QT_MOC_LITERAL(26, 303, 4), // "mode"
QT_MOC_LITERAL(27, 308, 13), // "sendFocusMove"
QT_MOC_LITERAL(28, 322, 13), // "sendFocusStop"
QT_MOC_LITERAL(29, 336, 16), // "sendDefogSetting"
QT_MOC_LITERAL(30, 353, 7), // "enabled"
QT_MOC_LITERAL(31, 361, 5), // "level"
QT_MOC_LITERAL(32, 367, 17), // "sendHeatHazeLevel"
QT_MOC_LITERAL(33, 385, 25), // "sendNoiseReductionSetting"
QT_MOC_LITERAL(34, 411, 24), // "sendStabilizationSetting"
QT_MOC_LITERAL(35, 436, 20), // "sendWhiteBalanceMode"
QT_MOC_LITERAL(36, 457, 20), // "sendWhiteBalanceGain"
QT_MOC_LITERAL(37, 478, 3), // "red"
QT_MOC_LITERAL(38, 482, 4) // "gain"

    },
    "SerialPortCCD\0portOpened\0\0success\0"
    "portClosed\0portError\0msg\0portsChanged\0"
    "ports\0onOpenPort\0portName\0baudRate\0"
    "onClosePort\0onScanPorts\0send30XFocus\0"
    "send1XFocus\0senddigZoomopen\0"
    "senddigZoomclose\0sendBacklightopen\0"
    "sendBacklightclose\0sendResolutionchange\0"
    "index\0sendZoomMove\0direction\0sendZoomStop\0"
    "sendFocusMode\0mode\0sendFocusMove\0"
    "sendFocusStop\0sendDefogSetting\0enabled\0"
    "level\0sendHeatHazeLevel\0"
    "sendNoiseReductionSetting\0"
    "sendStabilizationSetting\0sendWhiteBalanceMode\0"
    "sendWhiteBalanceGain\0red\0gain"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortCCD[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  139,    2, 0x06 /* Public */,
       4,    0,  142,    2, 0x06 /* Public */,
       5,    1,  143,    2, 0x06 /* Public */,
       7,    1,  146,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,  149,    2, 0x0a /* Public */,
      12,    0,  154,    2, 0x0a /* Public */,
      13,    0,  155,    2, 0x0a /* Public */,
      14,    0,  156,    2, 0x0a /* Public */,
      15,    0,  157,    2, 0x0a /* Public */,
      16,    0,  158,    2, 0x0a /* Public */,
      17,    0,  159,    2, 0x0a /* Public */,
      18,    0,  160,    2, 0x0a /* Public */,
      19,    0,  161,    2, 0x0a /* Public */,
      20,    1,  162,    2, 0x0a /* Public */,
      22,    1,  165,    2, 0x0a /* Public */,
      24,    0,  168,    2, 0x0a /* Public */,
      25,    1,  169,    2, 0x0a /* Public */,
      27,    1,  172,    2, 0x0a /* Public */,
      28,    0,  175,    2, 0x0a /* Public */,
      29,    2,  176,    2, 0x0a /* Public */,
      32,    1,  181,    2, 0x0a /* Public */,
      33,    2,  184,    2, 0x0a /* Public */,
      34,    2,  189,    2, 0x0a /* Public */,
      35,    1,  194,    2, 0x0a /* Public */,
      36,    2,  197,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QStringList,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   26,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   30,   31,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   30,   31,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   30,   31,
    QMetaType::Void, QMetaType::Int,   26,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   37,   38,

       0        // eod
};

void SerialPortCCD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortCCD *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->portClosed(); break;
        case 2: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->onClosePort(); break;
        case 6: _t->onScanPorts(); break;
        case 7: _t->send30XFocus(); break;
        case 8: _t->send1XFocus(); break;
        case 9: _t->senddigZoomopen(); break;
        case 10: _t->senddigZoomclose(); break;
        case 11: _t->sendBacklightopen(); break;
        case 12: _t->sendBacklightclose(); break;
        case 13: _t->sendResolutionchange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->sendZoomMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->sendZoomStop(); break;
        case 16: _t->sendFocusMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->sendFocusMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->sendFocusStop(); break;
        case 19: _t->sendDefogSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->sendHeatHazeLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->sendNoiseReductionSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->sendStabilizationSetting((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: _t->sendWhiteBalanceMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->sendWhiteBalanceGain((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortCCD::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortCCD::portOpened)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortCCD::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortCCD::portClosed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortCCD::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortCCD::portError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortCCD::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortCCD::portsChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortCCD::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortCCD.data,
    qt_meta_data_SerialPortCCD,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortCCD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortCCD::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortCCD.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortCCD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void SerialPortCCD::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortCCD::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPortCCD::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialPortCCD::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
