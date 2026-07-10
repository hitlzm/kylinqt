/****************************************************************************
** Meta object code from reading C++ file 'serialport_BD.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_BD.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_BD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BDData_t {
    QByteArrayData data[39];
    char stringdata0[461];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BDData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BDData_t qt_meta_stringdata_BDData = {
    {
QT_MOC_LITERAL(0, 0, 6), // "BDData"
QT_MOC_LITERAL(1, 7, 17), // "bjDateTimeChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 17), // "isPosValidChanged"
QT_MOC_LITERAL(4, 44, 15), // "latitudeChanged"
QT_MOC_LITERAL(5, 60, 14), // "isnorthChanged"
QT_MOC_LITERAL(6, 75, 16), // "longitudeChanged"
QT_MOC_LITERAL(7, 92, 13), // "iseastChanged"
QT_MOC_LITERAL(8, 106, 11), // "modeChanged"
QT_MOC_LITERAL(9, 118, 15), // "portOpenChanged"
QT_MOC_LITERAL(10, 134, 21), // "availablePortsChanged"
QT_MOC_LITERAL(11, 156, 18), // "errorStringChanged"
QT_MOC_LITERAL(12, 175, 15), // "requestOpenPort"
QT_MOC_LITERAL(13, 191, 8), // "portName"
QT_MOC_LITERAL(14, 200, 8), // "baudRate"
QT_MOC_LITERAL(15, 209, 16), // "requestClosePort"
QT_MOC_LITERAL(16, 226, 16), // "requestScanPorts"
QT_MOC_LITERAL(17, 243, 11), // "setPortOpen"
QT_MOC_LITERAL(18, 255, 4), // "open"
QT_MOC_LITERAL(19, 260, 11), // "setPortList"
QT_MOC_LITERAL(20, 272, 5), // "ports"
QT_MOC_LITERAL(21, 278, 8), // "setError"
QT_MOC_LITERAL(22, 287, 3), // "msg"
QT_MOC_LITERAL(23, 291, 15), // "updateFromFrame"
QT_MOC_LITERAL(24, 307, 7), // "RMCData"
QT_MOC_LITERAL(25, 315, 5), // "frame"
QT_MOC_LITERAL(26, 321, 8), // "openPort"
QT_MOC_LITERAL(27, 330, 9), // "closePort"
QT_MOC_LITERAL(28, 340, 9), // "scanPorts"
QT_MOC_LITERAL(29, 350, 12), // "m_bjDateTime"
QT_MOC_LITERAL(30, 363, 12), // "m_isPosValid"
QT_MOC_LITERAL(31, 376, 10), // "m_latitude"
QT_MOC_LITERAL(32, 387, 9), // "m_isnorth"
QT_MOC_LITERAL(33, 397, 11), // "m_longitude"
QT_MOC_LITERAL(34, 409, 8), // "m_iseast"
QT_MOC_LITERAL(35, 418, 6), // "m_mode"
QT_MOC_LITERAL(36, 425, 8), // "portOpen"
QT_MOC_LITERAL(37, 434, 14), // "availablePorts"
QT_MOC_LITERAL(38, 449, 11) // "errorString"

    },
    "BDData\0bjDateTimeChanged\0\0isPosValidChanged\0"
    "latitudeChanged\0isnorthChanged\0"
    "longitudeChanged\0iseastChanged\0"
    "modeChanged\0portOpenChanged\0"
    "availablePortsChanged\0errorStringChanged\0"
    "requestOpenPort\0portName\0baudRate\0"
    "requestClosePort\0requestScanPorts\0"
    "setPortOpen\0open\0setPortList\0ports\0"
    "setError\0msg\0updateFromFrame\0RMCData\0"
    "frame\0openPort\0closePort\0scanPorts\0"
    "m_bjDateTime\0m_isPosValid\0m_latitude\0"
    "m_isnorth\0m_longitude\0m_iseast\0m_mode\0"
    "portOpen\0availablePorts\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BDData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
      10,  150, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x06 /* Public */,
       3,    0,  115,    2, 0x06 /* Public */,
       4,    0,  116,    2, 0x06 /* Public */,
       5,    0,  117,    2, 0x06 /* Public */,
       6,    0,  118,    2, 0x06 /* Public */,
       7,    0,  119,    2, 0x06 /* Public */,
       8,    0,  120,    2, 0x06 /* Public */,
       9,    0,  121,    2, 0x06 /* Public */,
      10,    0,  122,    2, 0x06 /* Public */,
      11,    0,  123,    2, 0x06 /* Public */,
      12,    2,  124,    2, 0x06 /* Public */,
      15,    0,  129,    2, 0x06 /* Public */,
      16,    0,  130,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  131,    2, 0x0a /* Public */,
      19,    1,  134,    2, 0x0a /* Public */,
      21,    1,  137,    2, 0x0a /* Public */,
      23,    1,  140,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      26,    2,  143,    2, 0x02 /* Public */,
      27,    0,  148,    2, 0x02 /* Public */,
      28,    0,  149,    2, 0x02 /* Public */,

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

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::QStringList,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, 0x80000000 | 24,   25,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   14,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      29, QMetaType::QDateTime, 0x00495001,
      30, QMetaType::Bool, 0x00495001,
      31, QMetaType::Double, 0x00495001,
      32, QMetaType::Bool, 0x00495001,
      33, QMetaType::Double, 0x00495001,
      34, QMetaType::Bool, 0x00495001,
      35, QMetaType::QString, 0x00495001,
      36, QMetaType::Bool, 0x00495001,
      37, QMetaType::QStringList, 0x00495001,
      38, QMetaType::QString, 0x00495001,

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

       0        // eod
};

void BDData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BDData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->bjDateTimeChanged(); break;
        case 1: _t->isPosValidChanged(); break;
        case 2: _t->latitudeChanged(); break;
        case 3: _t->isnorthChanged(); break;
        case 4: _t->longitudeChanged(); break;
        case 5: _t->iseastChanged(); break;
        case 6: _t->modeChanged(); break;
        case 7: _t->portOpenChanged(); break;
        case 8: _t->availablePortsChanged(); break;
        case 9: _t->errorStringChanged(); break;
        case 10: _t->requestOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->requestClosePort(); break;
        case 12: _t->requestScanPorts(); break;
        case 13: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 15: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->updateFromFrame((*reinterpret_cast< const RMCData(*)>(_a[1]))); break;
        case 17: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->closePort(); break;
        case 19: _t->scanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::bjDateTimeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::isPosValidChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::latitudeChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::isnorthChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::longitudeChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::iseastChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::modeChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::portOpenChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::availablePortsChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::errorStringChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (BDData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::requestOpenPort)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::requestClosePort)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (BDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BDData::requestScanPorts)) {
                *result = 12;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<BDData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QDateTime*>(_v) = _t->bjDateTime(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isPosValid(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->latitude(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isnorth(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->longitude(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->iseast(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->mode(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->portOpen(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject BDData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_BDData.data,
    qt_meta_data_BDData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BDData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BDData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BDData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BDData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BDData::bjDateTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BDData::isPosValidChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BDData::latitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BDData::isnorthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BDData::longitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BDData::iseastChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BDData::modeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void BDData::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void BDData::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void BDData::errorStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void BDData::requestOpenPort(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void BDData::requestClosePort()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void BDData::requestScanPorts()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
struct qt_meta_stringdata_SerialPortBD_t {
    QByteArrayData data[18];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortBD_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortBD_t qt_meta_stringdata_SerialPortBD = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SerialPortBD"
QT_MOC_LITERAL(1, 13, 10), // "portOpened"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "success"
QT_MOC_LITERAL(4, 33, 10), // "portClosed"
QT_MOC_LITERAL(5, 44, 9), // "portError"
QT_MOC_LITERAL(6, 54, 3), // "msg"
QT_MOC_LITERAL(7, 58, 12), // "portsChanged"
QT_MOC_LITERAL(8, 71, 5), // "ports"
QT_MOC_LITERAL(9, 77, 15), // "bdFrameReceived"
QT_MOC_LITERAL(10, 93, 7), // "RMCData"
QT_MOC_LITERAL(11, 101, 5), // "frame"
QT_MOC_LITERAL(12, 107, 6), // "dowork"
QT_MOC_LITERAL(13, 114, 10), // "onOpenPort"
QT_MOC_LITERAL(14, 125, 8), // "portName"
QT_MOC_LITERAL(15, 134, 8), // "baudRate"
QT_MOC_LITERAL(16, 143, 11), // "onClosePort"
QT_MOC_LITERAL(17, 155, 11) // "onScanPorts"

    },
    "SerialPortBD\0portOpened\0\0success\0"
    "portClosed\0portError\0msg\0portsChanged\0"
    "ports\0bdFrameReceived\0RMCData\0frame\0"
    "dowork\0onOpenPort\0portName\0baudRate\0"
    "onClosePort\0onScanPorts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortBD[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    0,   62,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,
       7,    1,   66,    2, 0x06 /* Public */,
       9,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   72,    2, 0x0a /* Public */,
      13,    2,   73,    2, 0x0a /* Public */,
      16,    0,   78,    2, 0x0a /* Public */,
      17,    0,   79,    2, 0x0a /* Public */,

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

       0        // eod
};

void SerialPortBD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortBD *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->portOpened((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->portClosed(); break;
        case 2: _t->portError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->portsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->bdFrameReceived((*reinterpret_cast< const RMCData(*)>(_a[1]))); break;
        case 5: _t->dowork(); break;
        case 6: _t->onOpenPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onClosePort(); break;
        case 8: _t->onScanPorts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortBD::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortBD::portOpened)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPortBD::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortBD::portClosed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPortBD::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortBD::portError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPortBD::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortBD::portsChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPortBD::*)(const RMCData & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortBD::bdFrameReceived)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPortBD::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortBD.data,
    qt_meta_data_SerialPortBD,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortBD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortBD::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortBD.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortBD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SerialPortBD::portOpened(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPortBD::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPortBD::portError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialPortBD::portsChanged(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPortBD::bdFrameReceived(const RMCData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
