/****************************************************************************
** Meta object code from reading C++ file 'serialport_tiltmeter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_tiltmeter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_tiltmeter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TiltData_t {
    QByteArrayData data[54];
    char stringdata0[649];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TiltData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TiltData_t qt_meta_stringdata_TiltData = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TiltData"
QT_MOC_LITERAL(1, 9, 11), // "rollChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "pitchChanged"
QT_MOC_LITERAL(4, 35, 12), // "validChanged"
QT_MOC_LITERAL(5, 48, 16), // "timestampChanged"
QT_MOC_LITERAL(6, 65, 19), // "pollIntervalChanged"
QT_MOC_LITERAL(7, 85, 19), // "slaveAddressChanged"
QT_MOC_LITERAL(8, 105, 13), // "targetChanged"
QT_MOC_LITERAL(9, 119, 15), // "portOpenChanged"
QT_MOC_LITERAL(10, 135, 21), // "availablePortsChanged"
QT_MOC_LITERAL(11, 157, 18), // "errorStringChanged"
QT_MOC_LITERAL(12, 176, 15), // "requestOpenPort"
QT_MOC_LITERAL(13, 192, 8), // "portName"
QT_MOC_LITERAL(14, 201, 8), // "baudRate"
QT_MOC_LITERAL(15, 210, 16), // "requestClosePort"
QT_MOC_LITERAL(16, 227, 16), // "requestScanPorts"
QT_MOC_LITERAL(17, 244, 22), // "requestSetPollInterval"
QT_MOC_LITERAL(18, 267, 4), // "msec"
QT_MOC_LITERAL(19, 272, 22), // "requestSetSlaveAddress"
QT_MOC_LITERAL(20, 295, 4), // "addr"
QT_MOC_LITERAL(21, 300, 21), // "requestSendToPlatform"
QT_MOC_LITERAL(22, 322, 7), // "rollDeg"
QT_MOC_LITERAL(23, 330, 8), // "pitchDeg"
QT_MOC_LITERAL(24, 339, 16), // "requestSetTarget"
QT_MOC_LITERAL(25, 356, 7), // "address"
QT_MOC_LITERAL(26, 364, 4), // "port"
QT_MOC_LITERAL(27, 369, 15), // "updateFromFrame"
QT_MOC_LITERAL(28, 385, 9), // "TiltFrame"
QT_MOC_LITERAL(29, 395, 5), // "frame"
QT_MOC_LITERAL(30, 401, 11), // "setPortOpen"
QT_MOC_LITERAL(31, 413, 4), // "open"
QT_MOC_LITERAL(32, 418, 11), // "setPortList"
QT_MOC_LITERAL(33, 430, 5), // "ports"
QT_MOC_LITERAL(34, 436, 8), // "setError"
QT_MOC_LITERAL(35, 445, 3), // "msg"
QT_MOC_LITERAL(36, 449, 8), // "openPort"
QT_MOC_LITERAL(37, 458, 9), // "closePort"
QT_MOC_LITERAL(38, 468, 9), // "scanPorts"
QT_MOC_LITERAL(39, 478, 15), // "setPollInterval"
QT_MOC_LITERAL(40, 494, 15), // "setSlaveAddress"
QT_MOC_LITERAL(41, 510, 14), // "sendToPlatform"
QT_MOC_LITERAL(42, 525, 9), // "setTarget"
QT_MOC_LITERAL(43, 535, 4), // "roll"
QT_MOC_LITERAL(44, 540, 5), // "pitch"
QT_MOC_LITERAL(45, 546, 5), // "valid"
QT_MOC_LITERAL(46, 552, 9), // "timestamp"
QT_MOC_LITERAL(47, 562, 12), // "pollInterval"
QT_MOC_LITERAL(48, 575, 12), // "slaveAddress"
QT_MOC_LITERAL(49, 588, 13), // "targetAddress"
QT_MOC_LITERAL(50, 602, 10), // "targetPort"
QT_MOC_LITERAL(51, 613, 8), // "portOpen"
QT_MOC_LITERAL(52, 622, 14), // "availablePorts"
QT_MOC_LITERAL(53, 637, 11) // "errorString"

    },
    "TiltData\0rollChanged\0\0pitchChanged\0"
    "validChanged\0timestampChanged\0"
    "pollIntervalChanged\0slaveAddressChanged\0"
    "targetChanged\0portOpenChanged\0"
    "availablePortsChanged\0errorStringChanged\0"
    "requestOpenPort\0portName\0baudRate\0"
    "requestClosePort\0requestScanPorts\0"
    "requestSetPollInterval\0msec\0"
    "requestSetSlaveAddress\0addr\0"
    "requestSendToPlatform\0rollDeg\0pitchDeg\0"
    "requestSetTarget\0address\0port\0"
    "updateFromFrame\0TiltFrame\0frame\0"
    "setPortOpen\0open\0setPortList\0ports\0"
    "setError\0msg\0openPort\0closePort\0"
    "scanPorts\0setPollInterval\0setSlaveAddress\0"
    "sendToPlatform\0setTarget\0roll\0pitch\0"
    "valid\0timestamp\0pollInterval\0slaveAddress\0"
    "targetAddress\0targetPort\0portOpen\0"
    "availablePorts\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TiltData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
      11,  218, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  154,    2, 0x06 /* Public */,
       3,    0,  155,    2, 0x06 /* Public */,
       4,    0,  156,    2, 0x06 /* Public */,
       5,    0,  157,    2, 0x06 /* Public */,
       6,    0,  158,    2, 0x06 /* Public */,
       7,    0,  159,    2, 0x06 /* Public */,
       8,    0,  160,    2, 0x06 /* Public */,
       9,    0,  161,    2, 0x06 /* Public */,
      10,    0,  162,    2, 0x06 /* Public */,
      11,    0,  163,    2, 0x06 /* Public */,
      12,    2,  164,    2, 0x06 /* Public */,
      15,    0,  169,    2, 0x06 /* Public */,
      16,    0,  170,    2, 0x06 /* Public */,
      17,    1,  171,    2, 0x06 /* Public */,
      19,    1,  174,    2, 0x06 /* Public */,
      21,    2,  177,    2, 0x06 /* Public */,
      24,    2,  182,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    1,  187,    2, 0x0a /* Public */,
      30,    1,  190,    2, 0x0a /* Public */,
      32,    1,  193,    2, 0x0a /* Public */,
      34,    1,  196,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      36,    2,  199,    2, 0x02 /* Public */,
      37,    0,  204,    2, 0x02 /* Public */,
      38,    0,  205,    2, 0x02 /* Public */,
      39,    1,  206,    2, 0x02 /* Public */,
      40,    1,  209,    2, 0x02 /* Public */,
      41,    0,  212,    2, 0x02 /* Public */,
      42,    2,  213,    2, 0x02 /* Public */,

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
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   25,   26,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, QMetaType::Bool,   31,
    QMetaType::Void, QMetaType::QStringList,   33,
    QMetaType::Void, QMetaType::QString,   35,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   25,   26,

 // properties: name, type, flags
      43, QMetaType::Double, 0x00495001,
      44, QMetaType::Double, 0x00495001,
      45, QMetaType::Bool, 0x00495001,
      46, QMetaType::QDateTime, 0x00495001,
      47, QMetaType::Int, 0x00495001,
      48, QMetaType::Int, 0x00495001,
      49, QMetaType::QString, 0x00495001,
      50, QMetaType::Int, 0x00495001,
      51, QMetaType::Bool, 0x00495001,
      52, QMetaType::QStringList, 0x00495001,
      53, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       6,
       7,
       8,
       9,

       0        // eod
};

void TiltData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TiltData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rollChanged(); break;
        case 1: _t->pitchChanged(); break;
        case 2: _t->validChanged(); break;
        case 3: _t->timestampChanged(); break;
        case 4: _t->pollIntervalChanged(); break;
        case 5: _t->slaveAddressChanged(); break;
        case 6: _t->targetChanged(); break;
        case 7: _t->portOpenChanged(); break;
        case 8: _t->availablePortsChanged(); break;
        case 9: _t->errorStringChanged(); break;
        case 10: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->requestClosePort(); break;
        case 12: _t->requestScanPorts(); break;
        case 13: _t->requestSetPollInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->requestSetSlaveAddress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->requestSendToPlatform((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 16: _t->requestSetTarget((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->updateFromFrame((*reinterpret_cast< const TiltFrame(*)>(_a[1]))); break;
        case 18: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 20: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->closePort(); break;
        case 23: _t->scanPorts(); break;
        case 24: _t->setPollInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->setSlaveAddress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->sendToPlatform(); break;
        case 27: _t->setTarget((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TiltFrame >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::rollChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::pitchChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::validChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::timestampChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::pollIntervalChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::slaveAddressChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::targetChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::portOpenChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::availablePortsChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::errorStringChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TiltData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestOpenPort)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestClosePort)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TiltData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestScanPorts)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TiltData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestSetPollInterval)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TiltData::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestSetSlaveAddress)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (TiltData::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestSendToPlatform)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (TiltData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TiltData::requestSetTarget)) {
                *result = 16;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TiltData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->roll(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->pitch(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->valid(); break;
        case 3: *reinterpret_cast< QDateTime*>(_v) = _t->timestamp(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->pollInterval(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->slaveAddress(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->targetAddress(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->targetPort(); break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 9: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TiltData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TiltData.data,
    qt_meta_data_TiltData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TiltData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TiltData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TiltData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TiltData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TiltData::rollChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TiltData::pitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TiltData::validChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TiltData::timestampChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TiltData::pollIntervalChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TiltData::slaveAddressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TiltData::targetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TiltData::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TiltData::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TiltData::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TiltData::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void TiltData::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TiltData::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void TiltData::requestSetPollInterval(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void TiltData::requestSetSlaveAddress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void TiltData::requestSendToPlatform(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void TiltData::requestSetTarget(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}
struct qt_meta_stringdata_SerialPortTiltmeter_t {
    QByteArrayData data[29];
    char stringdata0[297];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortTiltmeter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortTiltmeter_t qt_meta_stringdata_SerialPortTiltmeter = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SerialPortTiltmeter"
QT_MOC_LITERAL(1, 20, 10), // "portOpened"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "success"
QT_MOC_LITERAL(4, 40, 10), // "portClosed"
QT_MOC_LITERAL(5, 51, 9), // "portError"
QT_MOC_LITERAL(6, 61, 3), // "msg"
QT_MOC_LITERAL(7, 65, 12), // "portsChanged"
QT_MOC_LITERAL(8, 78, 5), // "ports"
QT_MOC_LITERAL(9, 84, 17), // "tiltFrameReceived"
QT_MOC_LITERAL(10, 102, 9), // "TiltFrame"
QT_MOC_LITERAL(11, 112, 5), // "frame"
QT_MOC_LITERAL(12, 118, 6), // "dowork"
QT_MOC_LITERAL(13, 125, 10), // "onOpenPort"
QT_MOC_LITERAL(14, 136, 8), // "portName"
QT_MOC_LITERAL(15, 145, 8), // "baudRate"
QT_MOC_LITERAL(16, 154, 11), // "onClosePort"
QT_MOC_LITERAL(17, 166, 11), // "onScanPorts"
QT_MOC_LITERAL(18, 178, 17), // "onSetPollInterval"
QT_MOC_LITERAL(19, 196, 4), // "msec"
QT_MOC_LITERAL(20, 201, 17), // "onSetSlaveAddress"
QT_MOC_LITERAL(21, 219, 4), // "addr"
QT_MOC_LITERAL(22, 224, 16), // "onSendToPlatform"
QT_MOC_LITERAL(23, 241, 7), // "rollDeg"
QT_MOC_LITERAL(24, 249, 8), // "pitchDeg"
QT_MOC_LITERAL(25, 258, 11), // "onSetTarget"
QT_MOC_LITERAL(26, 270, 7), // "address"
QT_MOC_LITERAL(27, 278, 4), // "port"
QT_MOC_LITERAL(28, 283, 13) // "onPollTimeout"

    },
    "SerialPortTiltmeter\0portOpened\0\0success\0"
    "portClosed\0portError\0msg\0portsChanged\0"
    "ports\0tiltFrameReceived\0TiltFrame\0"
    "frame\0dowork\0onOpenPort\0portName\0"
    "baudRate\0onClosePort\0onScanPorts\0"
    "onSetPollInterval\0msec\0onSetSlaveAddress\0"
    "addr\0onSendToPlatform\0rollDeg\0pitchDeg\0"
    "onSetTarget\0address\0port\0onPollTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortTiltmeter[] = {

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
      12,    0,   97,    2, 0x0a /* Public */,
      13,    2,   98,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,
      17,    0,  104,    2, 0x0a /* Public */,
      18,    1,  105,    2, 0x0a /* Public */,
      20,    1,  108,    2, 0x0a /* Public */,
      22,    2,  111,    2, 0x0a /* Public */,
      25,    2,  116,    2, 0x0a /* Public */,
      28,    0,  121,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QStringList,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   23,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   26,   27,
    QMetaType::Void,

       0        // eod
};

void SerialPortTiltmeter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortTiltmeter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->portClosed(); break;
        case 2: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->tiltFrameReceived((*reinterpret_cast< const TiltFrame(*)>(_a[1]))); break;
        case 5: _t->dowork(); break;
        case 6: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onClosePort(); break;
        case 8: _t->onScanPorts(); break;
        case 9: _t->onSetPollInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onSetSlaveAddress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onSendToPlatform((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: _t->onSetTarget((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->onPollTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TiltFrame >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortTiltmeter::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTiltmeter::portOpened)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortTiltmeter::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTiltmeter::portClosed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortTiltmeter::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTiltmeter::portError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortTiltmeter::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTiltmeter::portsChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortTiltmeter::*)(const TiltFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTiltmeter::tiltFrameReceived)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortTiltmeter::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortTiltmeter.data,
    qt_meta_data_SerialPortTiltmeter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortTiltmeter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortTiltmeter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortTiltmeter.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortTiltmeter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SerialPortTiltmeter::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortTiltmeter::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPortTiltmeter::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialPortTiltmeter::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortTiltmeter::tiltFrameReceived(const TiltFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
