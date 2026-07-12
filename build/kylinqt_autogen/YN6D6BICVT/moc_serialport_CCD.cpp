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
    QByteArrayData data[29];
    char stringdata0[356];
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
QT_MOC_LITERAL(17, 243, 11), // "setPortOpen"
QT_MOC_LITERAL(18, 255, 4), // "open"
QT_MOC_LITERAL(19, 260, 11), // "setPortList"
QT_MOC_LITERAL(20, 272, 5), // "ports"
QT_MOC_LITERAL(21, 278, 8), // "setError"
QT_MOC_LITERAL(22, 287, 3), // "msg"
QT_MOC_LITERAL(23, 291, 8), // "openPort"
QT_MOC_LITERAL(24, 300, 9), // "closePort"
QT_MOC_LITERAL(25, 310, 9), // "scanPorts"
QT_MOC_LITERAL(26, 320, 8), // "portOpen"
QT_MOC_LITERAL(27, 329, 14), // "availablePorts"
QT_MOC_LITERAL(28, 344, 11) // "errorString"

    },
    "CCDData\0portOpenChanged\0\0availablePortsChanged\0"
    "errorStringChanged\0requestOpenPort\0"
    "portName\0baudRate\0requestClosePort\0"
    "requestScanPorts\0req30XFocus\0req1XFocus\0"
    "reqdigZoomopen\0reqdigZoomclose\0"
    "reqBacklightopen\0reqBacklightclose\0"
    "reqResolutionchange\0setPortOpen\0open\0"
    "setPortList\0ports\0setError\0msg\0openPort\0"
    "closePort\0scanPorts\0portOpen\0"
    "availablePorts\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCDData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       3,  142, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    0,  110,    2, 0x06 /* Public */,
       4,    0,  111,    2, 0x06 /* Public */,
       5,    2,  112,    2, 0x06 /* Public */,
       8,    0,  117,    2, 0x06 /* Public */,
       9,    0,  118,    2, 0x06 /* Public */,
      10,    0,  119,    2, 0x06 /* Public */,
      11,    0,  120,    2, 0x06 /* Public */,
      12,    0,  121,    2, 0x06 /* Public */,
      13,    0,  122,    2, 0x06 /* Public */,
      14,    0,  123,    2, 0x06 /* Public */,
      15,    0,  124,    2, 0x06 /* Public */,
      16,    0,  125,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  126,    2, 0x0a /* Public */,
      19,    1,  129,    2, 0x0a /* Public */,
      21,    1,  132,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      23,    2,  135,    2, 0x02 /* Public */,
      24,    0,  140,    2, 0x02 /* Public */,
      25,    0,  141,    2, 0x02 /* Public */,

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
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::QStringList,   20,
    QMetaType::Void, QMetaType::QString,   22,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      26, QMetaType::Bool, 0x00495001,
      27, QMetaType::QStringList, 0x00495001,
      28, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

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
        case 12: _t->reqResolutionchange(); break;
        case 13: _t->setPortOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setPortList((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 15: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->closePort(); break;
        case 18: _t->scanPorts(); break;
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
            using _t = void (CCDData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CCDData::reqResolutionchange)) {
                *result = 12;
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
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
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
void CCDData::reqResolutionchange()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
struct qt_meta_stringdata_SerialPortCCD_t {
    QByteArrayData data[21];
    char stringdata0[247];
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
QT_MOC_LITERAL(20, 226, 20) // "sendResolutionchange"

    },
    "SerialPortCCD\0portOpened\0\0success\0"
    "portClosed\0portError\0msg\0portsChanged\0"
    "ports\0onOpenPort\0portName\0baudRate\0"
    "onClosePort\0onScanPorts\0send30XFocus\0"
    "send1XFocus\0senddigZoomopen\0"
    "senddigZoomclose\0sendBacklightopen\0"
    "sendBacklightclose\0sendResolutionchange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortCCD[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    0,   87,    2, 0x06 /* Public */,
       5,    1,   88,    2, 0x06 /* Public */,
       7,    1,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   94,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    0,  101,    2, 0x0a /* Public */,
      15,    0,  102,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,
      17,    0,  104,    2, 0x0a /* Public */,
      18,    0,  105,    2, 0x0a /* Public */,
      19,    0,  106,    2, 0x0a /* Public */,
      20,    0,  107,    2, 0x0a /* Public */,

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
    QMetaType::Void,

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
        case 13: _t->sendResolutionchange(); break;
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
