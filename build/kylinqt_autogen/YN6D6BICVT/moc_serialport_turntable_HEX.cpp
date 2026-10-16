/****************************************************************************
** Meta object code from reading C++ file 'serialport_turntable_HEX.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_turntable_HEX.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_turntable_HEX.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TurntableDataHex_t {
    QByteArrayData data[56];
    char stringdata0[874];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TurntableDataHex_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TurntableDataHex_t qt_meta_stringdata_TurntableDataHex = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TurntableDataHex"
QT_MOC_LITERAL(1, 17, 11), // "timeChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 25), // "inner_statusnumberChanged"
QT_MOC_LITERAL(4, 56, 18), // "inner_angleChanged"
QT_MOC_LITERAL(5, 75, 25), // "inner_ctlDeviationChanged"
QT_MOC_LITERAL(6, 101, 26), // "middle_statusnumberChanged"
QT_MOC_LITERAL(7, 128, 19), // "middle_angleChanged"
QT_MOC_LITERAL(8, 148, 26), // "middle_ctlDeviationChanged"
QT_MOC_LITERAL(9, 175, 26), // "outter_statusnumberChanged"
QT_MOC_LITERAL(10, 202, 19), // "outter_angleChanged"
QT_MOC_LITERAL(11, 222, 26), // "outter_ctlDeviationChanged"
QT_MOC_LITERAL(12, 249, 18), // "hasSecPulseChanged"
QT_MOC_LITERAL(13, 268, 14), // "cmdHintChanged"
QT_MOC_LITERAL(14, 283, 20), // "myinner_angleChanged"
QT_MOC_LITERAL(15, 304, 11), // "inner_angle"
QT_MOC_LITERAL(16, 316, 21), // "mymiddle_angleChanged"
QT_MOC_LITERAL(17, 338, 12), // "middle_angle"
QT_MOC_LITERAL(18, 351, 21), // "myoutter_angleChanged"
QT_MOC_LITERAL(19, 373, 12), // "outter_angle"
QT_MOC_LITERAL(20, 386, 15), // "portOpenChanged"
QT_MOC_LITERAL(21, 402, 21), // "availablePortsChanged"
QT_MOC_LITERAL(22, 424, 18), // "errorStringChanged"
QT_MOC_LITERAL(23, 443, 15), // "requestOpenPort"
QT_MOC_LITERAL(24, 459, 8), // "portName"
QT_MOC_LITERAL(25, 468, 8), // "baudRate"
QT_MOC_LITERAL(26, 477, 16), // "requestClosePort"
QT_MOC_LITERAL(27, 494, 16), // "requestScanPorts"
QT_MOC_LITERAL(28, 511, 16), // "stepAngleChanged"
QT_MOC_LITERAL(29, 528, 10), // "step_angle"
QT_MOC_LITERAL(30, 539, 22), // "speedMultiplierChanged"
QT_MOC_LITERAL(31, 562, 16), // "speed_multiplier"
QT_MOC_LITERAL(32, 579, 11), // "updateframe"
QT_MOC_LITERAL(33, 591, 17), // "StatusFeedbackHex"
QT_MOC_LITERAL(34, 609, 8), // "recvdata"
QT_MOC_LITERAL(35, 618, 11), // "setPortOpen"
QT_MOC_LITERAL(36, 630, 4), // "open"
QT_MOC_LITERAL(37, 635, 11), // "setPortList"
QT_MOC_LITERAL(38, 647, 5), // "ports"
QT_MOC_LITERAL(39, 653, 8), // "setError"
QT_MOC_LITERAL(40, 662, 3), // "msg"
QT_MOC_LITERAL(41, 666, 8), // "openPort"
QT_MOC_LITERAL(42, 675, 9), // "closePort"
QT_MOC_LITERAL(43, 685, 9), // "scanPorts"
QT_MOC_LITERAL(44, 695, 4), // "time"
QT_MOC_LITERAL(45, 700, 18), // "inner_statusnumber"
QT_MOC_LITERAL(46, 719, 18), // "inner_ctlDeviation"
QT_MOC_LITERAL(47, 738, 19), // "middle_statusnumber"
QT_MOC_LITERAL(48, 758, 19), // "middle_ctlDeviation"
QT_MOC_LITERAL(49, 778, 19), // "outter_statusnumber"
QT_MOC_LITERAL(50, 798, 19), // "outter_ctlDeviation"
QT_MOC_LITERAL(51, 818, 11), // "hasSecPulse"
QT_MOC_LITERAL(52, 830, 7), // "cmdHint"
QT_MOC_LITERAL(53, 838, 8), // "portOpen"
QT_MOC_LITERAL(54, 847, 14), // "availablePorts"
QT_MOC_LITERAL(55, 862, 11) // "errorString"

    },
    "TurntableDataHex\0timeChanged\0\0"
    "inner_statusnumberChanged\0inner_angleChanged\0"
    "inner_ctlDeviationChanged\0"
    "middle_statusnumberChanged\0"
    "middle_angleChanged\0middle_ctlDeviationChanged\0"
    "outter_statusnumberChanged\0"
    "outter_angleChanged\0outter_ctlDeviationChanged\0"
    "hasSecPulseChanged\0cmdHintChanged\0"
    "myinner_angleChanged\0inner_angle\0"
    "mymiddle_angleChanged\0middle_angle\0"
    "myoutter_angleChanged\0outter_angle\0"
    "portOpenChanged\0availablePortsChanged\0"
    "errorStringChanged\0requestOpenPort\0"
    "portName\0baudRate\0requestClosePort\0"
    "requestScanPorts\0stepAngleChanged\0"
    "step_angle\0speedMultiplierChanged\0"
    "speed_multiplier\0updateframe\0"
    "StatusFeedbackHex\0recvdata\0setPortOpen\0"
    "open\0setPortList\0ports\0setError\0msg\0"
    "openPort\0closePort\0scanPorts\0time\0"
    "inner_statusnumber\0inner_ctlDeviation\0"
    "middle_statusnumber\0middle_ctlDeviation\0"
    "outter_statusnumber\0outter_ctlDeviation\0"
    "hasSecPulse\0cmdHint\0portOpen\0"
    "availablePorts\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableDataHex[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
      15,  220, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      23,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  164,    2, 0x06 /* Public */,
       3,    0,  165,    2, 0x06 /* Public */,
       4,    0,  166,    2, 0x06 /* Public */,
       5,    0,  167,    2, 0x06 /* Public */,
       6,    0,  168,    2, 0x06 /* Public */,
       7,    0,  169,    2, 0x06 /* Public */,
       8,    0,  170,    2, 0x06 /* Public */,
       9,    0,  171,    2, 0x06 /* Public */,
      10,    0,  172,    2, 0x06 /* Public */,
      11,    0,  173,    2, 0x06 /* Public */,
      12,    0,  174,    2, 0x06 /* Public */,
      13,    0,  175,    2, 0x06 /* Public */,
      14,    1,  176,    2, 0x06 /* Public */,
      16,    1,  179,    2, 0x06 /* Public */,
      18,    1,  182,    2, 0x06 /* Public */,
      20,    0,  185,    2, 0x06 /* Public */,
      21,    0,  186,    2, 0x06 /* Public */,
      22,    0,  187,    2, 0x06 /* Public */,
      23,    2,  188,    2, 0x06 /* Public */,
      26,    0,  193,    2, 0x06 /* Public */,
      27,    0,  194,    2, 0x06 /* Public */,
      28,    1,  195,    2, 0x06 /* Public */,
      30,    1,  198,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      32,    1,  201,    2, 0x0a /* Public */,
      35,    1,  204,    2, 0x0a /* Public */,
      37,    1,  207,    2, 0x0a /* Public */,
      39,    1,  210,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      41,    2,  213,    2, 0x02 /* Public */,
      42,    0,  218,    2, 0x02 /* Public */,
      43,    0,  219,    2, 0x02 /* Public */,

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
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void, QMetaType::Float,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   24,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   29,
    QMetaType::Void, QMetaType::Float,   31,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void, QMetaType::QStringList,   38,
    QMetaType::Void, QMetaType::QString,   40,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   24,   25,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      44, QMetaType::Int, 0x00495003,
      45, QMetaType::Int, 0x00495003,
      15, QMetaType::Float, 0x00495003,
      46, QMetaType::Float, 0x00495003,
      47, QMetaType::Int, 0x00495003,
      17, QMetaType::Float, 0x00495003,
      48, QMetaType::Float, 0x00495003,
      49, QMetaType::Int, 0x00495003,
      19, QMetaType::Float, 0x00495003,
      50, QMetaType::Float, 0x00495003,
      51, QMetaType::Int, 0x00495003,
      52, QMetaType::Int, 0x00495003,
      53, QMetaType::Bool, 0x00495001,
      54, QMetaType::QStringList, 0x00495001,
      55, QMetaType::QString, 0x00495001,

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
      15,
      16,
      17,

       0        // eod
};

void TurntableDataHex::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TurntableDataHex *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timeChanged(); break;
        case 1: _t->inner_statusnumberChanged(); break;
        case 2: _t->inner_angleChanged(); break;
        case 3: _t->inner_ctlDeviationChanged(); break;
        case 4: _t->middle_statusnumberChanged(); break;
        case 5: _t->middle_angleChanged(); break;
        case 6: _t->middle_ctlDeviationChanged(); break;
        case 7: _t->outter_statusnumberChanged(); break;
        case 8: _t->outter_angleChanged(); break;
        case 9: _t->outter_ctlDeviationChanged(); break;
        case 10: _t->hasSecPulseChanged(); break;
        case 11: _t->cmdHintChanged(); break;
        case 12: _t->myinner_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 13: _t->mymiddle_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->myoutter_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: _t->portOpenChanged(); break;
        case 16: _t->availablePortsChanged(); break;
        case 17: _t->errorStringChanged(); break;
        case 18: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: _t->requestClosePort(); break;
        case 20: _t->requestScanPorts(); break;
        case 21: _t->stepAngleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 22: _t->speedMultiplierChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 23: _t->updateframe((*reinterpret_cast< StatusFeedbackHex(*)>(_a[1]))); break;
        case 24: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 26: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 28: _t->closePort(); break;
        case 29: _t->scanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::timeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::inner_statusnumberChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::inner_angleChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::inner_ctlDeviationChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::middle_statusnumberChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::middle_angleChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::middle_ctlDeviationChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::outter_statusnumberChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::outter_angleChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::outter_ctlDeviationChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::hasSecPulseChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::cmdHintChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::myinner_angleChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::mymiddle_angleChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::myoutter_angleChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::portOpenChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::availablePortsChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::errorStringChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::requestOpenPort)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::requestClosePort)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::requestScanPorts)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::stepAngleChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (TurntableDataHex::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableDataHex::speedMultiplierChanged)) {
                *result = 22;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TurntableDataHex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->m_time; break;
        case 1: *reinterpret_cast< int*>(_v) = _t->m_inner_statusnumber; break;
        case 2: *reinterpret_cast< float*>(_v) = _t->m_inner_angle; break;
        case 3: *reinterpret_cast< float*>(_v) = _t->m_inner_ctlDeviation; break;
        case 4: *reinterpret_cast< int*>(_v) = _t->m_middle_statusnumber; break;
        case 5: *reinterpret_cast< float*>(_v) = _t->m_middle_angle; break;
        case 6: *reinterpret_cast< float*>(_v) = _t->m_middle_ctlDeviation; break;
        case 7: *reinterpret_cast< int*>(_v) = _t->m_outter_statusnumber; break;
        case 8: *reinterpret_cast< float*>(_v) = _t->m_outter_angle; break;
        case 9: *reinterpret_cast< float*>(_v) = _t->m_outter_ctlDeviation; break;
        case 10: *reinterpret_cast< int*>(_v) = _t->m_hasSecPulse; break;
        case 11: *reinterpret_cast< int*>(_v) = _t->m_cmdHint; break;
        case 12: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 13: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 14: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<TurntableDataHex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_time != *reinterpret_cast< int*>(_v)) {
                _t->m_time = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->timeChanged();
            }
            break;
        case 1:
            if (_t->m_inner_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_inner_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->inner_statusnumberChanged();
            }
            break;
        case 2:
            if (_t->m_inner_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_angleChanged();
            }
            break;
        case 3:
            if (_t->m_inner_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_ctlDeviationChanged();
            }
            break;
        case 4:
            if (_t->m_middle_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_middle_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->middle_statusnumberChanged();
            }
            break;
        case 5:
            if (_t->m_middle_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_angleChanged();
            }
            break;
        case 6:
            if (_t->m_middle_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_ctlDeviationChanged();
            }
            break;
        case 7:
            if (_t->m_outter_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_outter_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->outter_statusnumberChanged();
            }
            break;
        case 8:
            if (_t->m_outter_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_angleChanged();
            }
            break;
        case 9:
            if (_t->m_outter_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_ctlDeviationChanged();
            }
            break;
        case 10:
            if (_t->m_hasSecPulse != *reinterpret_cast< int*>(_v)) {
                _t->m_hasSecPulse = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->hasSecPulseChanged();
            }
            break;
        case 11:
            if (_t->m_cmdHint != *reinterpret_cast< int*>(_v)) {
                _t->m_cmdHint = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->cmdHintChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TurntableDataHex::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TurntableDataHex.data,
    qt_meta_data_TurntableDataHex,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TurntableDataHex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TurntableDataHex::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TurntableDataHex.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TurntableDataHex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 30;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 15;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TurntableDataHex::timeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TurntableDataHex::inner_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TurntableDataHex::inner_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TurntableDataHex::inner_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TurntableDataHex::middle_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TurntableDataHex::middle_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TurntableDataHex::middle_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TurntableDataHex::outter_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TurntableDataHex::outter_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TurntableDataHex::outter_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TurntableDataHex::hasSecPulseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TurntableDataHex::cmdHintChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TurntableDataHex::myinner_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void TurntableDataHex::mymiddle_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void TurntableDataHex::myoutter_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void TurntableDataHex::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void TurntableDataHex::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void TurntableDataHex::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void TurntableDataHex::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void TurntableDataHex::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void TurntableDataHex::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void TurntableDataHex::stepAngleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void TurntableDataHex::speedMultiplierChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}
struct qt_meta_stringdata_TurntableSendDataHex_t {
    QByteArrayData data[34];
    char stringdata0[543];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TurntableSendDataHex_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TurntableSendDataHex_t qt_meta_stringdata_TurntableSendDataHex = {
    {
QT_MOC_LITERAL(0, 0, 20), // "TurntableSendDataHex"
QT_MOC_LITERAL(1, 21, 22), // "requestSendProgramMode"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 20), // "programSend_frameHex"
QT_MOC_LITERAL(4, 66, 5), // "frame"
QT_MOC_LITERAL(5, 72, 14), // "runtimeChanged"
QT_MOC_LITERAL(6, 87, 23), // "inner_startangleChanged"
QT_MOC_LITERAL(7, 111, 21), // "inner_endangleChanged"
QT_MOC_LITERAL(8, 133, 24), // "middle_startangleChanged"
QT_MOC_LITERAL(9, 158, 22), // "middle_endangleChanged"
QT_MOC_LITERAL(10, 181, 24), // "outter_startangleChanged"
QT_MOC_LITERAL(11, 206, 22), // "outter_endangleChanged"
QT_MOC_LITERAL(12, 229, 12), // "indexChanged"
QT_MOC_LITERAL(13, 242, 16), // "reqopenTurntable"
QT_MOC_LITERAL(14, 259, 19), // "reqreleaseTurntable"
QT_MOC_LITERAL(15, 279, 17), // "reqcloseTurntable"
QT_MOC_LITERAL(16, 297, 16), // "reqzeroTurntable"
QT_MOC_LITERAL(17, 314, 17), // "reqresetTurntable"
QT_MOC_LITERAL(18, 332, 7), // "sinMove"
QT_MOC_LITERAL(19, 340, 7), // "m_index"
QT_MOC_LITERAL(20, 348, 9), // "amplitude"
QT_MOC_LITERAL(21, 358, 9), // "frequency"
QT_MOC_LITERAL(22, 368, 15), // "recvinner_angle"
QT_MOC_LITERAL(23, 384, 5), // "angle"
QT_MOC_LITERAL(24, 390, 16), // "recvmiddle_angle"
QT_MOC_LITERAL(25, 407, 16), // "recvoutter_angle"
QT_MOC_LITERAL(26, 424, 10), // "buildFrame"
QT_MOC_LITERAL(27, 435, 7), // "runtime"
QT_MOC_LITERAL(28, 443, 16), // "inner_startangle"
QT_MOC_LITERAL(29, 460, 14), // "inner_endangle"
QT_MOC_LITERAL(30, 475, 17), // "middle_startangle"
QT_MOC_LITERAL(31, 493, 15), // "middle_endangle"
QT_MOC_LITERAL(32, 509, 17), // "outter_startangle"
QT_MOC_LITERAL(33, 527, 15) // "outter_endangle"

    },
    "TurntableSendDataHex\0requestSendProgramMode\0"
    "\0programSend_frameHex\0frame\0runtimeChanged\0"
    "inner_startangleChanged\0inner_endangleChanged\0"
    "middle_startangleChanged\0"
    "middle_endangleChanged\0outter_startangleChanged\0"
    "outter_endangleChanged\0indexChanged\0"
    "reqopenTurntable\0reqreleaseTurntable\0"
    "reqcloseTurntable\0reqzeroTurntable\0"
    "reqresetTurntable\0sinMove\0m_index\0"
    "amplitude\0frequency\0recvinner_angle\0"
    "angle\0recvmiddle_angle\0recvoutter_angle\0"
    "buildFrame\0runtime\0inner_startangle\0"
    "inner_endangle\0middle_startangle\0"
    "middle_endangle\0outter_startangle\0"
    "outter_endangle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableSendDataHex[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       7,  144, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       5,    0,  112,    2, 0x06 /* Public */,
       6,    0,  113,    2, 0x06 /* Public */,
       7,    0,  114,    2, 0x06 /* Public */,
       8,    0,  115,    2, 0x06 /* Public */,
       9,    0,  116,    2, 0x06 /* Public */,
      10,    0,  117,    2, 0x06 /* Public */,
      11,    0,  118,    2, 0x06 /* Public */,
      12,    0,  119,    2, 0x06 /* Public */,
      13,    0,  120,    2, 0x06 /* Public */,
      14,    0,  121,    2, 0x06 /* Public */,
      15,    0,  122,    2, 0x06 /* Public */,
      16,    0,  123,    2, 0x06 /* Public */,
      17,    0,  124,    2, 0x06 /* Public */,
      18,    3,  125,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    1,  132,    2, 0x0a /* Public */,
      24,    1,  135,    2, 0x0a /* Public */,
      25,    1,  138,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      26,    1,  141,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
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
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,   19,   20,   21,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   23,
    QMetaType::Void, QMetaType::Float,   23,
    QMetaType::Void, QMetaType::Float,   23,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,   19,

 // properties: name, type, flags
      27, QMetaType::Int, 0x00495003,
      28, QMetaType::Float, 0x00495003,
      29, QMetaType::Float, 0x00495003,
      30, QMetaType::Float, 0x00495003,
      31, QMetaType::Float, 0x00495003,
      32, QMetaType::Float, 0x00495003,
      33, QMetaType::Float, 0x00495003,

 // properties: notify_signal_id
       1,
       2,
       3,
       4,
       5,
       6,
       7,

       0        // eod
};

void TurntableSendDataHex::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TurntableSendDataHex *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestSendProgramMode((*reinterpret_cast< programSend_frameHex(*)>(_a[1]))); break;
        case 1: _t->runtimeChanged(); break;
        case 2: _t->inner_startangleChanged(); break;
        case 3: _t->inner_endangleChanged(); break;
        case 4: _t->middle_startangleChanged(); break;
        case 5: _t->middle_endangleChanged(); break;
        case 6: _t->outter_startangleChanged(); break;
        case 7: _t->outter_endangleChanged(); break;
        case 8: _t->indexChanged(); break;
        case 9: _t->reqopenTurntable(); break;
        case 10: _t->reqreleaseTurntable(); break;
        case 11: _t->reqcloseTurntable(); break;
        case 12: _t->reqzeroTurntable(); break;
        case 13: _t->reqresetTurntable(); break;
        case 14: _t->sinMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 15: _t->recvinner_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 16: _t->recvmiddle_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 17: _t->recvoutter_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 18: _t->buildFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TurntableSendDataHex::*)(programSend_frameHex );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::requestSendProgramMode)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::runtimeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::inner_startangleChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::inner_endangleChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::middle_startangleChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::middle_endangleChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::outter_startangleChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::outter_endangleChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::indexChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::reqopenTurntable)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::reqreleaseTurntable)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::reqcloseTurntable)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::reqzeroTurntable)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::reqresetTurntable)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TurntableSendDataHex::*)(int , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendDataHex::sinMove)) {
                *result = 14;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TurntableSendDataHex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->m_runtime; break;
        case 1: *reinterpret_cast< float*>(_v) = _t->m_inner_startangle; break;
        case 2: *reinterpret_cast< float*>(_v) = _t->m_inner_endangle; break;
        case 3: *reinterpret_cast< float*>(_v) = _t->m_middle_startangle; break;
        case 4: *reinterpret_cast< float*>(_v) = _t->m_middle_endangle; break;
        case 5: *reinterpret_cast< float*>(_v) = _t->m_outter_startangle; break;
        case 6: *reinterpret_cast< float*>(_v) = _t->m_outter_endangle; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<TurntableSendDataHex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_runtime != *reinterpret_cast< int*>(_v)) {
                _t->m_runtime = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->runtimeChanged();
            }
            break;
        case 1:
            if (_t->m_inner_startangle != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_startangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_startangleChanged();
            }
            break;
        case 2:
            if (_t->m_inner_endangle != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_endangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_endangleChanged();
            }
            break;
        case 3:
            if (_t->m_middle_startangle != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_startangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_startangleChanged();
            }
            break;
        case 4:
            if (_t->m_middle_endangle != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_endangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_endangleChanged();
            }
            break;
        case 5:
            if (_t->m_outter_startangle != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_startangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_startangleChanged();
            }
            break;
        case 6:
            if (_t->m_outter_endangle != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_endangle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_endangleChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TurntableSendDataHex::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TurntableSendDataHex.data,
    qt_meta_data_TurntableSendDataHex,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TurntableSendDataHex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TurntableSendDataHex::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TurntableSendDataHex.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TurntableSendDataHex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TurntableSendDataHex::requestSendProgramMode(programSend_frameHex _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TurntableSendDataHex::runtimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TurntableSendDataHex::inner_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TurntableSendDataHex::inner_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TurntableSendDataHex::middle_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TurntableSendDataHex::middle_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TurntableSendDataHex::outter_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TurntableSendDataHex::outter_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TurntableSendDataHex::indexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TurntableSendDataHex::reqopenTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TurntableSendDataHex::reqreleaseTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TurntableSendDataHex::reqcloseTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TurntableSendDataHex::reqzeroTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void TurntableSendDataHex::reqresetTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void TurntableSendDataHex::sinMove(int _t1, float _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
struct qt_meta_stringdata_SerialPortTurntableHex_t {
    QByteArrayData data[59];
    char stringdata0[682];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortTurntableHex_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortTurntableHex_t qt_meta_stringdata_SerialPortTurntableHex = {
    {
QT_MOC_LITERAL(0, 0, 22), // "SerialPortTurntableHex"
QT_MOC_LITERAL(1, 23, 14), // "requpdateframe"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 17), // "StatusFeedbackHex"
QT_MOC_LITERAL(4, 57, 8), // "recvdata"
QT_MOC_LITERAL(5, 66, 10), // "portOpened"
QT_MOC_LITERAL(6, 77, 7), // "success"
QT_MOC_LITERAL(7, 85, 10), // "portClosed"
QT_MOC_LITERAL(8, 96, 9), // "portError"
QT_MOC_LITERAL(9, 106, 3), // "msg"
QT_MOC_LITERAL(10, 110, 12), // "portsChanged"
QT_MOC_LITERAL(11, 123, 5), // "ports"
QT_MOC_LITERAL(12, 129, 13), // "openTurntable"
QT_MOC_LITERAL(13, 143, 16), // "releaseTurntable"
QT_MOC_LITERAL(14, 160, 14), // "closeTurntable"
QT_MOC_LITERAL(15, 175, 14), // "resetTurntable"
QT_MOC_LITERAL(16, 190, 13), // "zeroTurntable"
QT_MOC_LITERAL(17, 204, 15), // "sendProgramMode"
QT_MOC_LITERAL(18, 220, 20), // "programSend_frameHex"
QT_MOC_LITERAL(19, 241, 5), // "frame"
QT_MOC_LITERAL(20, 247, 14), // "sendHandleMode"
QT_MOC_LITERAL(21, 262, 9), // "axisLeftX"
QT_MOC_LITERAL(22, 272, 9), // "axisLeftY"
QT_MOC_LITERAL(23, 282, 10), // "axisRightX"
QT_MOC_LITERAL(24, 293, 8), // "buttonL2"
QT_MOC_LITERAL(25, 302, 8), // "buttonR2"
QT_MOC_LITERAL(26, 311, 7), // "buttonA"
QT_MOC_LITERAL(27, 319, 7), // "buttonB"
QT_MOC_LITERAL(28, 327, 6), // "Acount"
QT_MOC_LITERAL(29, 334, 6), // "Bcount"
QT_MOC_LITERAL(30, 341, 16), // "sendTrackMode_1s"
QT_MOC_LITERAL(31, 358, 15), // "sendExGuideData"
QT_MOC_LITERAL(32, 374, 6), // "frame1"
QT_MOC_LITERAL(33, 381, 6), // "frame2"
QT_MOC_LITERAL(34, 388, 17), // "sendTrackMode_5ms"
QT_MOC_LITERAL(35, 406, 8), // "yawangle"
QT_MOC_LITERAL(36, 415, 10), // "pitchangle"
QT_MOC_LITERAL(37, 426, 13), // "sendSwingMode"
QT_MOC_LITERAL(38, 440, 5), // "index"
QT_MOC_LITERAL(39, 446, 4), // "freq"
QT_MOC_LITERAL(40, 451, 9), // "amplitude"
QT_MOC_LITERAL(41, 461, 12), // "sendTimesync"
QT_MOC_LITERAL(42, 474, 7), // "seconds"
QT_MOC_LITERAL(43, 482, 18), // "ProgramModeChanged"
QT_MOC_LITERAL(44, 501, 4), // "mode"
QT_MOC_LITERAL(45, 506, 6), // "dowork"
QT_MOC_LITERAL(46, 513, 15), // "recvinner_angle"
QT_MOC_LITERAL(47, 529, 5), // "angle"
QT_MOC_LITERAL(48, 535, 16), // "recvmiddle_angle"
QT_MOC_LITERAL(49, 552, 16), // "recvoutter_angle"
QT_MOC_LITERAL(50, 569, 10), // "onOpenPort"
QT_MOC_LITERAL(51, 580, 8), // "portName"
QT_MOC_LITERAL(52, 589, 8), // "baudRate"
QT_MOC_LITERAL(53, 598, 11), // "onClosePort"
QT_MOC_LITERAL(54, 610, 11), // "onScanPorts"
QT_MOC_LITERAL(55, 622, 12), // "setStepAngle"
QT_MOC_LITERAL(56, 635, 10), // "step_angle"
QT_MOC_LITERAL(57, 646, 18), // "setSpeedMultiplier"
QT_MOC_LITERAL(58, 665, 16) // "speed_multiplier"

    },
    "SerialPortTurntableHex\0requpdateframe\0"
    "\0StatusFeedbackHex\0recvdata\0portOpened\0"
    "success\0portClosed\0portError\0msg\0"
    "portsChanged\0ports\0openTurntable\0"
    "releaseTurntable\0closeTurntable\0"
    "resetTurntable\0zeroTurntable\0"
    "sendProgramMode\0programSend_frameHex\0"
    "frame\0sendHandleMode\0axisLeftX\0axisLeftY\0"
    "axisRightX\0buttonL2\0buttonR2\0buttonA\0"
    "buttonB\0Acount\0Bcount\0sendTrackMode_1s\0"
    "sendExGuideData\0frame1\0frame2\0"
    "sendTrackMode_5ms\0yawangle\0pitchangle\0"
    "sendSwingMode\0index\0freq\0amplitude\0"
    "sendTimesync\0seconds\0ProgramModeChanged\0"
    "mode\0dowork\0recvinner_angle\0angle\0"
    "recvmiddle_angle\0recvoutter_angle\0"
    "onOpenPort\0portName\0baudRate\0onClosePort\0"
    "onScanPorts\0setStepAngle\0step_angle\0"
    "setSpeedMultiplier\0speed_multiplier"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortTurntableHex[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  144,    2, 0x06 /* Public */,
       5,    1,  147,    2, 0x06 /* Public */,
       7,    0,  150,    2, 0x06 /* Public */,
       8,    1,  151,    2, 0x06 /* Public */,
      10,    1,  154,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  157,    2, 0x0a /* Public */,
      13,    0,  158,    2, 0x0a /* Public */,
      14,    0,  159,    2, 0x0a /* Public */,
      15,    0,  160,    2, 0x0a /* Public */,
      16,    0,  161,    2, 0x0a /* Public */,
      17,    1,  162,    2, 0x0a /* Public */,
      20,    9,  165,    2, 0x0a /* Public */,
      30,    2,  184,    2, 0x0a /* Public */,
      34,    2,  189,    2, 0x0a /* Public */,
      37,    3,  194,    2, 0x0a /* Public */,
      41,    1,  201,    2, 0x0a /* Public */,
      43,    1,  204,    2, 0x0a /* Public */,
      45,    0,  207,    2, 0x0a /* Public */,
      46,    1,  208,    2, 0x0a /* Public */,
      48,    1,  211,    2, 0x0a /* Public */,
      49,    1,  214,    2, 0x0a /* Public */,
      50,    2,  217,    2, 0x0a /* Public */,
      53,    0,  222,    2, 0x0a /* Public */,
      54,    0,  223,    2, 0x0a /* Public */,
      55,    1,  224,    2, 0x0a /* Public */,
      57,    1,  227,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QStringList,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::Int,   21,   22,   23,   24,   25,   26,   27,   28,   29,
    QMetaType::Void, 0x80000000 | 31, 0x80000000 | 31,   32,   33,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   35,   36,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,   38,   39,   40,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   44,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   47,
    QMetaType::Void, QMetaType::Float,   47,
    QMetaType::Void, QMetaType::Float,   47,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   51,   52,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   56,
    QMetaType::Void, QMetaType::Float,   58,

       0        // eod
};

void SerialPortTurntableHex::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortTurntableHex *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requpdateframe((*reinterpret_cast< StatusFeedbackHex(*)>(_a[1]))); break;
        case 1: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->portClosed(); break;
        case 3: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->openTurntable(); break;
        case 6: _t->releaseTurntable(); break;
        case 7: _t->closeTurntable(); break;
        case 8: _t->resetTurntable(); break;
        case 9: _t->zeroTurntable(); break;
        case 10: _t->sendProgramMode((*reinterpret_cast< programSend_frameHex(*)>(_a[1]))); break;
        case 11: _t->sendHandleMode((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])),(*reinterpret_cast< int(*)>(_a[9]))); break;
        case 12: _t->sendTrackMode_1s((*reinterpret_cast< sendExGuideData(*)>(_a[1])),(*reinterpret_cast< sendExGuideData(*)>(_a[2]))); break;
        case 13: _t->sendTrackMode_5ms((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 14: _t->sendSwingMode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 15: _t->sendTimesync((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->ProgramModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->dowork(); break;
        case 18: _t->recvinner_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 19: _t->recvmiddle_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 20: _t->recvoutter_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 21: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->onClosePort(); break;
        case 23: _t->onScanPorts(); break;
        case 24: _t->setStepAngle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 25: _t->setSpeedMultiplier((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortTurntableHex::*)(StatusFeedbackHex );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntableHex::requpdateframe)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntableHex::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntableHex::portOpened)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntableHex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntableHex::portClosed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntableHex::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntableHex::portError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntableHex::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntableHex::portsChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortTurntableHex::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortTurntableHex.data,
    qt_meta_data_SerialPortTurntableHex,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortTurntableHex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortTurntableHex::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortTurntableHex.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortTurntableHex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void SerialPortTurntableHex::requpdateframe(StatusFeedbackHex _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortTurntableHex::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SerialPortTurntableHex::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SerialPortTurntableHex::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortTurntableHex::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
