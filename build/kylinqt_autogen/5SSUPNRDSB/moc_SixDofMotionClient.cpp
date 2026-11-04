/****************************************************************************
** Meta object code from reading C++ file 'SixDofMotionClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../network/SixDofMotionClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SixDofMotionClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SixDofMotionClient_t {
    QByteArrayData data[65];
    char stringdata0[980];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SixDofMotionClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SixDofMotionClient_t qt_meta_stringdata_SixDofMotionClient = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SixDofMotionClient"
QT_MOC_LITERAL(1, 19, 16), // "localPortChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 20), // "targetAddressChanged"
QT_MOC_LITERAL(4, 58, 17), // "targetPortChanged"
QT_MOC_LITERAL(5, 76, 25), // "statusQueryAddressChanged"
QT_MOC_LITERAL(6, 102, 22), // "statusQueryPortChanged"
QT_MOC_LITERAL(7, 125, 25), // "controlCardAddressChanged"
QT_MOC_LITERAL(8, 151, 28), // "autoGoMiddleOnConnectChanged"
QT_MOC_LITERAL(9, 180, 16), // "connectedChanged"
QT_MOC_LITERAL(10, 197, 16), // "linkAliveChanged"
QT_MOC_LITERAL(11, 214, 20), // "statusMessageChanged"
QT_MOC_LITERAL(12, 235, 17), // "statusCodeChanged"
QT_MOC_LITERAL(13, 253, 16), // "lastErrorChanged"
QT_MOC_LITERAL(14, 270, 20), // "executeTimeMsChanged"
QT_MOC_LITERAL(15, 291, 11), // "tiltChanged"
QT_MOC_LITERAL(16, 303, 17), // "targetPoseChanged"
QT_MOC_LITERAL(17, 321, 20), // "currentHeightChanged"
QT_MOC_LITERAL(18, 342, 23), // "connectionCheckFinished"
QT_MOC_LITERAL(19, 366, 7), // "success"
QT_MOC_LITERAL(20, 374, 7), // "message"
QT_MOC_LITERAL(21, 382, 11), // "commandSent"
QT_MOC_LITERAL(22, 394, 11), // "description"
QT_MOC_LITERAL(23, 406, 17), // "onSocketReadyRead"
QT_MOC_LITERAL(24, 424, 20), // "onStatusTimerTimeout"
QT_MOC_LITERAL(25, 445, 19), // "onCheckTimerTimeout"
QT_MOC_LITERAL(26, 465, 14), // "openConnection"
QT_MOC_LITERAL(27, 480, 15), // "closeConnection"
QT_MOC_LITERAL(28, 496, 11), // "isConnected"
QT_MOC_LITERAL(29, 508, 15), // "checkConnection"
QT_MOC_LITERAL(30, 524, 20), // "startConnectionCheck"
QT_MOC_LITERAL(31, 545, 9), // "timeoutMs"
QT_MOC_LITERAL(32, 555, 10), // "sendTiltXY"
QT_MOC_LITERAL(33, 566, 4), // "xDeg"
QT_MOC_LITERAL(34, 571, 4), // "yDeg"
QT_MOC_LITERAL(35, 576, 18), // "sendTiltXYWithTime"
QT_MOC_LITERAL(36, 595, 6), // "timeMs"
QT_MOC_LITERAL(37, 602, 14), // "sendTiltHeight"
QT_MOC_LITERAL(38, 617, 3), // "zMm"
QT_MOC_LITERAL(39, 621, 22), // "sendTiltHeightWithTime"
QT_MOC_LITERAL(40, 644, 14), // "setExecuteTime"
QT_MOC_LITERAL(41, 659, 4), // "msec"
QT_MOC_LITERAL(42, 664, 8), // "goMiddle"
QT_MOC_LITERAL(43, 673, 16), // "goMiddleWithTime"
QT_MOC_LITERAL(44, 690, 8), // "goBottom"
QT_MOC_LITERAL(45, 699, 16), // "goBottomWithTime"
QT_MOC_LITERAL(46, 716, 13), // "emergencyStop"
QT_MOC_LITERAL(47, 730, 9), // "localPort"
QT_MOC_LITERAL(48, 740, 13), // "targetAddress"
QT_MOC_LITERAL(49, 754, 10), // "targetPort"
QT_MOC_LITERAL(50, 765, 18), // "statusQueryAddress"
QT_MOC_LITERAL(51, 784, 15), // "statusQueryPort"
QT_MOC_LITERAL(52, 800, 18), // "controlCardAddress"
QT_MOC_LITERAL(53, 819, 21), // "autoGoMiddleOnConnect"
QT_MOC_LITERAL(54, 841, 9), // "connected"
QT_MOC_LITERAL(55, 851, 9), // "linkAlive"
QT_MOC_LITERAL(56, 861, 13), // "statusMessage"
QT_MOC_LITERAL(57, 875, 10), // "statusCode"
QT_MOC_LITERAL(58, 886, 9), // "lastError"
QT_MOC_LITERAL(59, 896, 13), // "executeTimeMs"
QT_MOC_LITERAL(60, 910, 8), // "tiltXDeg"
QT_MOC_LITERAL(61, 919, 8), // "tiltYDeg"
QT_MOC_LITERAL(62, 928, 15), // "targetHeightZMm"
QT_MOC_LITERAL(63, 944, 16), // "currentHeightZMm"
QT_MOC_LITERAL(64, 961, 18) // "currentHeightValid"

    },
    "SixDofMotionClient\0localPortChanged\0"
    "\0targetAddressChanged\0targetPortChanged\0"
    "statusQueryAddressChanged\0"
    "statusQueryPortChanged\0controlCardAddressChanged\0"
    "autoGoMiddleOnConnectChanged\0"
    "connectedChanged\0linkAliveChanged\0"
    "statusMessageChanged\0statusCodeChanged\0"
    "lastErrorChanged\0executeTimeMsChanged\0"
    "tiltChanged\0targetPoseChanged\0"
    "currentHeightChanged\0connectionCheckFinished\0"
    "success\0message\0commandSent\0description\0"
    "onSocketReadyRead\0onStatusTimerTimeout\0"
    "onCheckTimerTimeout\0openConnection\0"
    "closeConnection\0isConnected\0checkConnection\0"
    "startConnectionCheck\0timeoutMs\0"
    "sendTiltXY\0xDeg\0yDeg\0sendTiltXYWithTime\0"
    "timeMs\0sendTiltHeight\0zMm\0"
    "sendTiltHeightWithTime\0setExecuteTime\0"
    "msec\0goMiddle\0goMiddleWithTime\0goBottom\0"
    "goBottomWithTime\0emergencyStop\0localPort\0"
    "targetAddress\0targetPort\0statusQueryAddress\0"
    "statusQueryPort\0controlCardAddress\0"
    "autoGoMiddleOnConnect\0connected\0"
    "linkAlive\0statusMessage\0statusCode\0"
    "lastError\0executeTimeMs\0tiltXDeg\0"
    "tiltYDeg\0targetHeightZMm\0currentHeightZMm\0"
    "currentHeightValid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SixDofMotionClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
      18,  274, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

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
      18,    2,  215,    2, 0x06 /* Public */,
      21,    1,  220,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      23,    0,  223,    2, 0x08 /* Private */,
      24,    0,  224,    2, 0x08 /* Private */,
      25,    0,  225,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      26,    0,  226,    2, 0x02 /* Public */,
      27,    0,  227,    2, 0x02 /* Public */,
      28,    0,  228,    2, 0x02 /* Public */,
      29,    0,  229,    2, 0x02 /* Public */,
      30,    1,  230,    2, 0x02 /* Public */,
      30,    0,  233,    2, 0x22 /* Public | MethodCloned */,
      32,    2,  234,    2, 0x02 /* Public */,
      35,    3,  239,    2, 0x02 /* Public */,
      37,    3,  246,    2, 0x02 /* Public */,
      39,    4,  253,    2, 0x02 /* Public */,
      40,    1,  262,    2, 0x02 /* Public */,
      42,    0,  265,    2, 0x02 /* Public */,
      43,    1,  266,    2, 0x02 /* Public */,
      44,    0,  269,    2, 0x02 /* Public */,
      45,    1,  270,    2, 0x02 /* Public */,
      46,    0,  273,    2, 0x02 /* Public */,

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
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   19,   20,
    QMetaType::Void, QMetaType::QString,   22,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Double, QMetaType::Double,   33,   34,
    QMetaType::Bool, QMetaType::Double, QMetaType::Double, QMetaType::Int,   33,   34,   36,
    QMetaType::Bool, QMetaType::Double, QMetaType::Double, QMetaType::Double,   33,   34,   38,
    QMetaType::Bool, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Int,   33,   34,   38,   36,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::Int,   36,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::Int,   36,
    QMetaType::Bool,

 // properties: name, type, flags
      47, QMetaType::Int, 0x00495103,
      48, QMetaType::QString, 0x00495103,
      49, QMetaType::Int, 0x00495103,
      50, QMetaType::QString, 0x00495103,
      51, QMetaType::Int, 0x00495103,
      52, QMetaType::QString, 0x00495103,
      53, QMetaType::Bool, 0x00495103,
      54, QMetaType::Bool, 0x00495001,
      55, QMetaType::Bool, 0x00495001,
      56, QMetaType::QString, 0x00495001,
      57, QMetaType::Int, 0x00495001,
      58, QMetaType::QString, 0x00495001,
      59, QMetaType::Int, 0x00495003,
      60, QMetaType::Double, 0x00495001,
      61, QMetaType::Double, 0x00495001,
      62, QMetaType::Double, 0x00495001,
      63, QMetaType::Double, 0x00495001,
      64, QMetaType::Bool, 0x00495001,

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
      13,
      14,
      15,
      15,

       0        // eod
};

void SixDofMotionClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SixDofMotionClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->localPortChanged(); break;
        case 1: _t->targetAddressChanged(); break;
        case 2: _t->targetPortChanged(); break;
        case 3: _t->statusQueryAddressChanged(); break;
        case 4: _t->statusQueryPortChanged(); break;
        case 5: _t->controlCardAddressChanged(); break;
        case 6: _t->autoGoMiddleOnConnectChanged(); break;
        case 7: _t->connectedChanged(); break;
        case 8: _t->linkAliveChanged(); break;
        case 9: _t->statusMessageChanged(); break;
        case 10: _t->statusCodeChanged(); break;
        case 11: _t->lastErrorChanged(); break;
        case 12: _t->executeTimeMsChanged(); break;
        case 13: _t->tiltChanged(); break;
        case 14: _t->targetPoseChanged(); break;
        case 15: _t->currentHeightChanged(); break;
        case 16: _t->connectionCheckFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 17: _t->commandSent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->onSocketReadyRead(); break;
        case 19: _t->onStatusTimerTimeout(); break;
        case 20: _t->onCheckTimerTimeout(); break;
        case 21: { bool _r = _t->openConnection();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->closeConnection(); break;
        case 23: { bool _r = _t->isConnected();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 24: { bool _r = _t->checkConnection();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 25: _t->startConnectionCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->startConnectionCheck(); break;
        case 27: { bool _r = _t->sendTiltXY((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 28: { bool _r = _t->sendTiltXYWithTime((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 29: { bool _r = _t->sendTiltHeight((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 30: { bool _r = _t->sendTiltHeightWithTime((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 31: _t->setExecuteTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: { bool _r = _t->goMiddle();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 33: { bool _r = _t->goMiddleWithTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 34: { bool _r = _t->goBottom();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 35: { bool _r = _t->goBottomWithTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 36: { bool _r = _t->emergencyStop();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::localPortChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::targetAddressChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::targetPortChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::statusQueryAddressChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::statusQueryPortChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::controlCardAddressChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::autoGoMiddleOnConnectChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::connectedChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::linkAliveChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::statusMessageChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::statusCodeChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::lastErrorChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::executeTimeMsChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::tiltChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::targetPoseChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::currentHeightChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::connectionCheckFinished)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (SixDofMotionClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SixDofMotionClient::commandSent)) {
                *result = 17;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SixDofMotionClient *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->localPort(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->targetAddress(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->targetPort(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->statusQueryAddress(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->statusQueryPort(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->controlCardAddress(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->autoGoMiddleOnConnect(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->connected(); break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->linkAlive(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->statusMessage(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->statusCode(); break;
        case 11: *reinterpret_cast< QString*>(_v) = _t->lastError(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->executeTimeMs(); break;
        case 13: *reinterpret_cast< double*>(_v) = _t->tiltXDeg(); break;
        case 14: *reinterpret_cast< double*>(_v) = _t->tiltYDeg(); break;
        case 15: *reinterpret_cast< double*>(_v) = _t->targetHeightZMm(); break;
        case 16: *reinterpret_cast< double*>(_v) = _t->currentHeightZMm(); break;
        case 17: *reinterpret_cast< bool*>(_v) = _t->currentHeightValid(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<SixDofMotionClient *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setLocalPort(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setTargetAddress(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setTargetPort(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setStatusQueryAddress(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setStatusQueryPort(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setControlCardAddress(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setAutoGoMiddleOnConnect(*reinterpret_cast< bool*>(_v)); break;
        case 12: _t->setExecuteTime(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SixDofMotionClient::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_SixDofMotionClient.data,
    qt_meta_data_SixDofMotionClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SixDofMotionClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SixDofMotionClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SixDofMotionClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SixDofMotionClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 18;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SixDofMotionClient::localPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SixDofMotionClient::targetAddressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SixDofMotionClient::targetPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SixDofMotionClient::statusQueryAddressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SixDofMotionClient::statusQueryPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SixDofMotionClient::controlCardAddressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SixDofMotionClient::autoGoMiddleOnConnectChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SixDofMotionClient::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SixDofMotionClient::linkAliveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void SixDofMotionClient::statusMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void SixDofMotionClient::statusCodeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void SixDofMotionClient::lastErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void SixDofMotionClient::executeTimeMsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void SixDofMotionClient::tiltChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void SixDofMotionClient::targetPoseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void SixDofMotionClient::currentHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void SixDofMotionClient::connectionCheckFinished(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void SixDofMotionClient::commandSent(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
