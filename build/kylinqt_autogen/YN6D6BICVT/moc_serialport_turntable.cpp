/****************************************************************************
** Meta object code from reading C++ file 'serialport_turntable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_turntable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_turntable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TurntableData_t {
    QByteArrayData data[50];
    char stringdata0[773];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TurntableData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TurntableData_t qt_meta_stringdata_TurntableData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TurntableData"
QT_MOC_LITERAL(1, 14, 11), // "timeChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 16), // "ctlnumberChanged"
QT_MOC_LITERAL(4, 44, 25), // "inner_statusnumberChanged"
QT_MOC_LITERAL(5, 70, 18), // "inner_angleChanged"
QT_MOC_LITERAL(6, 89, 25), // "inner_ctlDeviationChanged"
QT_MOC_LITERAL(7, 115, 26), // "middle_statusnumberChanged"
QT_MOC_LITERAL(8, 142, 19), // "middle_angleChanged"
QT_MOC_LITERAL(9, 162, 26), // "middle_ctlDeviationChanged"
QT_MOC_LITERAL(10, 189, 26), // "outter_statusnumberChanged"
QT_MOC_LITERAL(11, 216, 19), // "outter_angleChanged"
QT_MOC_LITERAL(12, 236, 26), // "outter_ctlDeviationChanged"
QT_MOC_LITERAL(13, 263, 20), // "myinner_angleChanged"
QT_MOC_LITERAL(14, 284, 11), // "inner_angle"
QT_MOC_LITERAL(15, 296, 21), // "mymiddle_angleChanged"
QT_MOC_LITERAL(16, 318, 12), // "middle_angle"
QT_MOC_LITERAL(17, 331, 21), // "myoutter_angleChanged"
QT_MOC_LITERAL(18, 353, 12), // "outter_angle"
QT_MOC_LITERAL(19, 366, 15), // "portOpenChanged"
QT_MOC_LITERAL(20, 382, 21), // "availablePortsChanged"
QT_MOC_LITERAL(21, 404, 18), // "errorStringChanged"
QT_MOC_LITERAL(22, 423, 15), // "requestOpenPort"
QT_MOC_LITERAL(23, 439, 8), // "portName"
QT_MOC_LITERAL(24, 448, 8), // "baudRate"
QT_MOC_LITERAL(25, 457, 16), // "requestClosePort"
QT_MOC_LITERAL(26, 474, 16), // "requestScanPorts"
QT_MOC_LITERAL(27, 491, 11), // "updateframe"
QT_MOC_LITERAL(28, 503, 14), // "StatusFeedback"
QT_MOC_LITERAL(29, 518, 8), // "recvdata"
QT_MOC_LITERAL(30, 527, 11), // "setPortOpen"
QT_MOC_LITERAL(31, 539, 4), // "open"
QT_MOC_LITERAL(32, 544, 11), // "setPortList"
QT_MOC_LITERAL(33, 556, 5), // "ports"
QT_MOC_LITERAL(34, 562, 8), // "setError"
QT_MOC_LITERAL(35, 571, 3), // "msg"
QT_MOC_LITERAL(36, 575, 8), // "openPort"
QT_MOC_LITERAL(37, 584, 9), // "closePort"
QT_MOC_LITERAL(38, 594, 9), // "scanPorts"
QT_MOC_LITERAL(39, 604, 4), // "time"
QT_MOC_LITERAL(40, 609, 9), // "ctlnumber"
QT_MOC_LITERAL(41, 619, 18), // "inner_statusnumber"
QT_MOC_LITERAL(42, 638, 18), // "inner_ctlDeviation"
QT_MOC_LITERAL(43, 657, 19), // "middle_statusnumber"
QT_MOC_LITERAL(44, 677, 19), // "middle_ctlDeviation"
QT_MOC_LITERAL(45, 697, 19), // "outter_statusnumber"
QT_MOC_LITERAL(46, 717, 19), // "outter_ctlDeviation"
QT_MOC_LITERAL(47, 737, 8), // "portOpen"
QT_MOC_LITERAL(48, 746, 14), // "availablePorts"
QT_MOC_LITERAL(49, 761, 11) // "errorString"

    },
    "TurntableData\0timeChanged\0\0ctlnumberChanged\0"
    "inner_statusnumberChanged\0inner_angleChanged\0"
    "inner_ctlDeviationChanged\0"
    "middle_statusnumberChanged\0"
    "middle_angleChanged\0middle_ctlDeviationChanged\0"
    "outter_statusnumberChanged\0"
    "outter_angleChanged\0outter_ctlDeviationChanged\0"
    "myinner_angleChanged\0inner_angle\0"
    "mymiddle_angleChanged\0middle_angle\0"
    "myoutter_angleChanged\0outter_angle\0"
    "portOpenChanged\0availablePortsChanged\0"
    "errorStringChanged\0requestOpenPort\0"
    "portName\0baudRate\0requestClosePort\0"
    "requestScanPorts\0updateframe\0"
    "StatusFeedback\0recvdata\0setPortOpen\0"
    "open\0setPortList\0ports\0setError\0msg\0"
    "openPort\0closePort\0scanPorts\0time\0"
    "ctlnumber\0inner_statusnumber\0"
    "inner_ctlDeviation\0middle_statusnumber\0"
    "middle_ctlDeviation\0outter_statusnumber\0"
    "outter_ctlDeviation\0portOpen\0"
    "availablePorts\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
      14,  198, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      20,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  149,    2, 0x06 /* Public */,
       3,    0,  150,    2, 0x06 /* Public */,
       4,    0,  151,    2, 0x06 /* Public */,
       5,    0,  152,    2, 0x06 /* Public */,
       6,    0,  153,    2, 0x06 /* Public */,
       7,    0,  154,    2, 0x06 /* Public */,
       8,    0,  155,    2, 0x06 /* Public */,
       9,    0,  156,    2, 0x06 /* Public */,
      10,    0,  157,    2, 0x06 /* Public */,
      11,    0,  158,    2, 0x06 /* Public */,
      12,    0,  159,    2, 0x06 /* Public */,
      13,    1,  160,    2, 0x06 /* Public */,
      15,    1,  163,    2, 0x06 /* Public */,
      17,    1,  166,    2, 0x06 /* Public */,
      19,    0,  169,    2, 0x06 /* Public */,
      20,    0,  170,    2, 0x06 /* Public */,
      21,    0,  171,    2, 0x06 /* Public */,
      22,    2,  172,    2, 0x06 /* Public */,
      25,    0,  177,    2, 0x06 /* Public */,
      26,    0,  178,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    1,  179,    2, 0x0a /* Public */,
      30,    1,  182,    2, 0x0a /* Public */,
      32,    1,  185,    2, 0x0a /* Public */,
      34,    1,  188,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      36,    2,  191,    2, 0x02 /* Public */,
      37,    0,  196,    2, 0x02 /* Public */,
      38,    0,  197,    2, 0x02 /* Public */,

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
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   23,   24,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, QMetaType::Bool,   31,
    QMetaType::Void, QMetaType::QStringList,   33,
    QMetaType::Void, QMetaType::QString,   35,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   23,   24,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      39, QMetaType::Int, 0x00495003,
      40, QMetaType::Int, 0x00495003,
      41, QMetaType::Int, 0x00495003,
      14, QMetaType::Float, 0x00495003,
      42, QMetaType::Float, 0x00495003,
      43, QMetaType::Int, 0x00495003,
      16, QMetaType::Float, 0x00495003,
      44, QMetaType::Float, 0x00495003,
      45, QMetaType::Int, 0x00495003,
      18, QMetaType::Float, 0x00495003,
      46, QMetaType::Float, 0x00495003,
      47, QMetaType::Bool, 0x00495001,
      48, QMetaType::QStringList, 0x00495001,
      49, QMetaType::QString, 0x00495001,

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
      14,
      15,
      16,

       0        // eod
};

void TurntableData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TurntableData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timeChanged(); break;
        case 1: _t->ctlnumberChanged(); break;
        case 2: _t->inner_statusnumberChanged(); break;
        case 3: _t->inner_angleChanged(); break;
        case 4: _t->inner_ctlDeviationChanged(); break;
        case 5: _t->middle_statusnumberChanged(); break;
        case 6: _t->middle_angleChanged(); break;
        case 7: _t->middle_ctlDeviationChanged(); break;
        case 8: _t->outter_statusnumberChanged(); break;
        case 9: _t->outter_angleChanged(); break;
        case 10: _t->outter_ctlDeviationChanged(); break;
        case 11: _t->myinner_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 12: _t->mymiddle_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 13: _t->myoutter_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->portOpenChanged(); break;
        case 15: _t->availablePortsChanged(); break;
        case 16: _t->errorStringChanged(); break;
        case 17: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->requestClosePort(); break;
        case 19: _t->requestScanPorts(); break;
        case 20: _t->updateframe((*reinterpret_cast< StatusFeedback(*)>(_a[1]))); break;
        case 21: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 23: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->closePort(); break;
        case 26: _t->scanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::timeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::ctlnumberChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::inner_statusnumberChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::inner_angleChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::inner_ctlDeviationChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::middle_statusnumberChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::middle_angleChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::middle_ctlDeviationChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::outter_statusnumberChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::outter_angleChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::outter_ctlDeviationChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::myinner_angleChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::mymiddle_angleChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::myoutter_angleChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::portOpenChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::availablePortsChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::errorStringChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::requestOpenPort)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::requestClosePort)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::requestScanPorts)) {
                *result = 19;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TurntableData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->m_time; break;
        case 1: *reinterpret_cast< int*>(_v) = _t->m_ctlnumber; break;
        case 2: *reinterpret_cast< int*>(_v) = _t->m_inner_statusnumber; break;
        case 3: *reinterpret_cast< float*>(_v) = _t->m_inner_angle; break;
        case 4: *reinterpret_cast< float*>(_v) = _t->m_inner_ctlDeviation; break;
        case 5: *reinterpret_cast< int*>(_v) = _t->m_middle_statusnumber; break;
        case 6: *reinterpret_cast< float*>(_v) = _t->m_middle_angle; break;
        case 7: *reinterpret_cast< float*>(_v) = _t->m_middle_ctlDeviation; break;
        case 8: *reinterpret_cast< int*>(_v) = _t->m_outter_statusnumber; break;
        case 9: *reinterpret_cast< float*>(_v) = _t->m_outter_angle; break;
        case 10: *reinterpret_cast< float*>(_v) = _t->m_outter_ctlDeviation; break;
        case 11: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 12: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 13: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<TurntableData *>(_o);
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
            if (_t->m_ctlnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_ctlnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->ctlnumberChanged();
            }
            break;
        case 2:
            if (_t->m_inner_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_inner_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->inner_statusnumberChanged();
            }
            break;
        case 3:
            if (_t->m_inner_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_angleChanged();
            }
            break;
        case 4:
            if (_t->m_inner_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_inner_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->inner_ctlDeviationChanged();
            }
            break;
        case 5:
            if (_t->m_middle_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_middle_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->middle_statusnumberChanged();
            }
            break;
        case 6:
            if (_t->m_middle_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_angleChanged();
            }
            break;
        case 7:
            if (_t->m_middle_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_middle_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->middle_ctlDeviationChanged();
            }
            break;
        case 8:
            if (_t->m_outter_statusnumber != *reinterpret_cast< int*>(_v)) {
                _t->m_outter_statusnumber = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->outter_statusnumberChanged();
            }
            break;
        case 9:
            if (_t->m_outter_angle != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_angle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_angleChanged();
            }
            break;
        case 10:
            if (_t->m_outter_ctlDeviation != *reinterpret_cast< float*>(_v)) {
                _t->m_outter_ctlDeviation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->outter_ctlDeviationChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TurntableData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TurntableData.data,
    qt_meta_data_TurntableData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TurntableData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TurntableData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TurntableData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TurntableData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 27;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TurntableData::timeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TurntableData::ctlnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TurntableData::inner_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TurntableData::inner_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TurntableData::inner_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TurntableData::middle_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TurntableData::middle_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TurntableData::middle_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TurntableData::outter_statusnumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TurntableData::outter_angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TurntableData::outter_ctlDeviationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TurntableData::myinner_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void TurntableData::mymiddle_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void TurntableData::myoutter_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void TurntableData::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void TurntableData::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void TurntableData::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void TurntableData::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void TurntableData::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void TurntableData::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}
struct qt_meta_stringdata_TurntableSendData_t {
    QByteArrayData data[30];
    char stringdata0[504];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TurntableSendData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TurntableSendData_t qt_meta_stringdata_TurntableSendData = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TurntableSendData"
QT_MOC_LITERAL(1, 18, 20), // "requestOpenTurntable"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 21), // "requestCloseTurntable"
QT_MOC_LITERAL(4, 62, 21), // "requestResetTurntable"
QT_MOC_LITERAL(5, 84, 20), // "requestZeroTurntable"
QT_MOC_LITERAL(6, 105, 22), // "requestSendProgramMode"
QT_MOC_LITERAL(7, 128, 18), // "programSend_frame&"
QT_MOC_LITERAL(8, 147, 5), // "frame"
QT_MOC_LITERAL(9, 153, 14), // "runtimeChanged"
QT_MOC_LITERAL(10, 168, 23), // "inner_startangleChanged"
QT_MOC_LITERAL(11, 192, 21), // "inner_endangleChanged"
QT_MOC_LITERAL(12, 214, 24), // "middle_startangleChanged"
QT_MOC_LITERAL(13, 239, 22), // "middle_endangleChanged"
QT_MOC_LITERAL(14, 262, 24), // "outter_startangleChanged"
QT_MOC_LITERAL(15, 287, 22), // "outter_endangleChanged"
QT_MOC_LITERAL(16, 310, 12), // "indexChanged"
QT_MOC_LITERAL(17, 323, 15), // "recvinner_angle"
QT_MOC_LITERAL(18, 339, 5), // "angle"
QT_MOC_LITERAL(19, 345, 16), // "recvmiddle_angle"
QT_MOC_LITERAL(20, 362, 16), // "recvoutter_angle"
QT_MOC_LITERAL(21, 379, 10), // "buildFrame"
QT_MOC_LITERAL(22, 390, 7), // "runtime"
QT_MOC_LITERAL(23, 398, 16), // "inner_startangle"
QT_MOC_LITERAL(24, 415, 14), // "inner_endangle"
QT_MOC_LITERAL(25, 430, 17), // "middle_startangle"
QT_MOC_LITERAL(26, 448, 15), // "middle_endangle"
QT_MOC_LITERAL(27, 464, 17), // "outter_startangle"
QT_MOC_LITERAL(28, 482, 15), // "outter_endangle"
QT_MOC_LITERAL(29, 498, 5) // "index"

    },
    "TurntableSendData\0requestOpenTurntable\0"
    "\0requestCloseTurntable\0requestResetTurntable\0"
    "requestZeroTurntable\0requestSendProgramMode\0"
    "programSend_frame&\0frame\0runtimeChanged\0"
    "inner_startangleChanged\0inner_endangleChanged\0"
    "middle_startangleChanged\0"
    "middle_endangleChanged\0outter_startangleChanged\0"
    "outter_endangleChanged\0indexChanged\0"
    "recvinner_angle\0angle\0recvmiddle_angle\0"
    "recvoutter_angle\0buildFrame\0runtime\0"
    "inner_startangle\0inner_endangle\0"
    "middle_startangle\0middle_endangle\0"
    "outter_startangle\0outter_endangle\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableSendData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       8,  124, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    1,  103,    2, 0x06 /* Public */,
       9,    0,  106,    2, 0x06 /* Public */,
      10,    0,  107,    2, 0x06 /* Public */,
      11,    0,  108,    2, 0x06 /* Public */,
      12,    0,  109,    2, 0x06 /* Public */,
      13,    0,  110,    2, 0x06 /* Public */,
      14,    0,  111,    2, 0x06 /* Public */,
      15,    0,  112,    2, 0x06 /* Public */,
      16,    0,  113,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  114,    2, 0x0a /* Public */,
      19,    1,  117,    2, 0x0a /* Public */,
      20,    1,  120,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      21,    0,  123,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::Float,   18,

 // methods: parameters
    QMetaType::Void,

 // properties: name, type, flags
      22, QMetaType::Int, 0x00495001,
      23, QMetaType::Float, 0x00495001,
      24, QMetaType::Float, 0x00495001,
      25, QMetaType::Float, 0x00495001,
      26, QMetaType::Float, 0x00495001,
      27, QMetaType::Float, 0x00495001,
      28, QMetaType::Float, 0x00495001,
      29, QMetaType::Int, 0x00495001,

 // properties: notify_signal_id
       5,
       6,
       7,
       8,
       9,
      10,
      11,
      12,

       0        // eod
};

void TurntableSendData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TurntableSendData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestOpenTurntable(); break;
        case 1: _t->requestCloseTurntable(); break;
        case 2: _t->requestResetTurntable(); break;
        case 3: _t->requestZeroTurntable(); break;
        case 4: _t->requestSendProgramMode((*reinterpret_cast< programSend_frame(*)>(_a[1]))); break;
        case 5: _t->runtimeChanged(); break;
        case 6: _t->inner_startangleChanged(); break;
        case 7: _t->inner_endangleChanged(); break;
        case 8: _t->middle_startangleChanged(); break;
        case 9: _t->middle_endangleChanged(); break;
        case 10: _t->outter_startangleChanged(); break;
        case 11: _t->outter_endangleChanged(); break;
        case 12: _t->indexChanged(); break;
        case 13: _t->recvinner_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->recvmiddle_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: _t->recvoutter_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 16: _t->buildFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestOpenTurntable)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestCloseTurntable)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestResetTurntable)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestZeroTurntable)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)(programSend_frame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestSendProgramMode)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::runtimeChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::inner_startangleChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::inner_endangleChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::middle_startangleChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::middle_endangleChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::outter_startangleChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::outter_endangleChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::indexChanged)) {
                *result = 12;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TurntableSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->runtime(); break;
        case 1: *reinterpret_cast< float*>(_v) = _t->inner_startangle(); break;
        case 2: *reinterpret_cast< float*>(_v) = _t->inner_endangle(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->middle_startangle(); break;
        case 4: *reinterpret_cast< float*>(_v) = _t->middle_endangle(); break;
        case 5: *reinterpret_cast< float*>(_v) = _t->outter_startangle(); break;
        case 6: *reinterpret_cast< float*>(_v) = _t->outter_endangle(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->index(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TurntableSendData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TurntableSendData.data,
    qt_meta_data_TurntableSendData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TurntableSendData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TurntableSendData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TurntableSendData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TurntableSendData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TurntableSendData::requestOpenTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TurntableSendData::requestCloseTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TurntableSendData::requestResetTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TurntableSendData::requestZeroTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TurntableSendData::requestSendProgramMode(programSend_frame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TurntableSendData::runtimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TurntableSendData::inner_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TurntableSendData::inner_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TurntableSendData::middle_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TurntableSendData::middle_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TurntableSendData::outter_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TurntableSendData::outter_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TurntableSendData::indexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
struct qt_meta_stringdata_SerialPortTurntable_t {
    QByteArrayData data[40];
    char stringdata0[456];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortTurntable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortTurntable_t qt_meta_stringdata_SerialPortTurntable = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SerialPortTurntable"
QT_MOC_LITERAL(1, 20, 14), // "requpdateframe"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "StatusFeedback"
QT_MOC_LITERAL(4, 51, 8), // "recvdata"
QT_MOC_LITERAL(5, 60, 10), // "portOpened"
QT_MOC_LITERAL(6, 71, 7), // "success"
QT_MOC_LITERAL(7, 79, 10), // "portClosed"
QT_MOC_LITERAL(8, 90, 9), // "portError"
QT_MOC_LITERAL(9, 100, 3), // "msg"
QT_MOC_LITERAL(10, 104, 12), // "portsChanged"
QT_MOC_LITERAL(11, 117, 5), // "ports"
QT_MOC_LITERAL(12, 123, 13), // "openTurntable"
QT_MOC_LITERAL(13, 137, 14), // "closeTurntable"
QT_MOC_LITERAL(14, 152, 14), // "resetTurntable"
QT_MOC_LITERAL(15, 167, 13), // "zeroTurntable"
QT_MOC_LITERAL(16, 181, 15), // "sendProgramMode"
QT_MOC_LITERAL(17, 197, 18), // "programSend_frame&"
QT_MOC_LITERAL(18, 216, 5), // "frame"
QT_MOC_LITERAL(19, 222, 14), // "sendHandleMode"
QT_MOC_LITERAL(20, 237, 9), // "axisLeftX"
QT_MOC_LITERAL(21, 247, 9), // "axisLeftY"
QT_MOC_LITERAL(22, 257, 10), // "axisRightX"
QT_MOC_LITERAL(23, 268, 8), // "buttonL2"
QT_MOC_LITERAL(24, 277, 8), // "buttonR2"
QT_MOC_LITERAL(25, 286, 7), // "buttonA"
QT_MOC_LITERAL(26, 294, 7), // "buttonB"
QT_MOC_LITERAL(27, 302, 13), // "sendTrackMode"
QT_MOC_LITERAL(28, 316, 18), // "ProgramModeChanged"
QT_MOC_LITERAL(29, 335, 4), // "mode"
QT_MOC_LITERAL(30, 340, 6), // "dowork"
QT_MOC_LITERAL(31, 347, 15), // "recvinner_angle"
QT_MOC_LITERAL(32, 363, 5), // "angle"
QT_MOC_LITERAL(33, 369, 16), // "recvmiddle_angle"
QT_MOC_LITERAL(34, 386, 16), // "recvoutter_angle"
QT_MOC_LITERAL(35, 403, 10), // "onOpenPort"
QT_MOC_LITERAL(36, 414, 8), // "portName"
QT_MOC_LITERAL(37, 423, 8), // "baudRate"
QT_MOC_LITERAL(38, 432, 11), // "onClosePort"
QT_MOC_LITERAL(39, 444, 11) // "onScanPorts"

    },
    "SerialPortTurntable\0requpdateframe\0\0"
    "StatusFeedback\0recvdata\0portOpened\0"
    "success\0portClosed\0portError\0msg\0"
    "portsChanged\0ports\0openTurntable\0"
    "closeTurntable\0resetTurntable\0"
    "zeroTurntable\0sendProgramMode\0"
    "programSend_frame&\0frame\0sendHandleMode\0"
    "axisLeftX\0axisLeftY\0axisRightX\0buttonL2\0"
    "buttonR2\0buttonA\0buttonB\0sendTrackMode\0"
    "ProgramModeChanged\0mode\0dowork\0"
    "recvinner_angle\0angle\0recvmiddle_angle\0"
    "recvoutter_angle\0onOpenPort\0portName\0"
    "baudRate\0onClosePort\0onScanPorts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortTurntable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       5,    1,  117,    2, 0x06 /* Public */,
       7,    0,  120,    2, 0x06 /* Public */,
       8,    1,  121,    2, 0x06 /* Public */,
      10,    1,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  127,    2, 0x0a /* Public */,
      13,    0,  128,    2, 0x0a /* Public */,
      14,    0,  129,    2, 0x0a /* Public */,
      15,    0,  130,    2, 0x0a /* Public */,
      16,    1,  131,    2, 0x0a /* Public */,
      19,    7,  134,    2, 0x0a /* Public */,
      27,    0,  149,    2, 0x0a /* Public */,
      28,    1,  150,    2, 0x0a /* Public */,
      30,    0,  153,    2, 0x0a /* Public */,
      31,    1,  154,    2, 0x0a /* Public */,
      33,    1,  157,    2, 0x0a /* Public */,
      34,    1,  160,    2, 0x0a /* Public */,
      35,    2,  163,    2, 0x0a /* Public */,
      38,    0,  168,    2, 0x0a /* Public */,
      39,    0,  169,    2, 0x0a /* Public */,

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
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::Bool,   20,   21,   22,   23,   24,   25,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   32,
    QMetaType::Void, QMetaType::Float,   32,
    QMetaType::Void, QMetaType::Float,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   36,   37,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SerialPortTurntable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortTurntable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requpdateframe((*reinterpret_cast< StatusFeedback(*)>(_a[1]))); break;
        case 1: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->portClosed(); break;
        case 3: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->openTurntable(); break;
        case 6: _t->closeTurntable(); break;
        case 7: _t->resetTurntable(); break;
        case 8: _t->zeroTurntable(); break;
        case 9: _t->sendProgramMode((*reinterpret_cast< programSend_frame(*)>(_a[1]))); break;
        case 10: _t->sendHandleMode((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 11: _t->sendTrackMode(); break;
        case 12: _t->ProgramModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->dowork(); break;
        case 14: _t->recvinner_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: _t->recvmiddle_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 16: _t->recvoutter_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 17: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->onClosePort(); break;
        case 19: _t->onScanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortTurntable::*)(StatusFeedback );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::requpdateframe)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntable::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::portOpened)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntable::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::portClosed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntable::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::portError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortTurntable::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::portsChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortTurntable::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortTurntable.data,
    qt_meta_data_SerialPortTurntable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortTurntable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortTurntable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortTurntable.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortTurntable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void SerialPortTurntable::requpdateframe(StatusFeedback _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortTurntable::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SerialPortTurntable::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SerialPortTurntable::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortTurntable::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
