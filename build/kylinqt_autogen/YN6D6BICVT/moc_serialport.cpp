/****************************************************************************
** Meta object code from reading C++ file 'serialport.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPort_t {
    QByteArrayData data[26];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPort_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPort_t qt_meta_stringdata_SerialPort = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SerialPort"
QT_MOC_LITERAL(1, 11, 13), // "errorOccurred"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "errorMsg"
QT_MOC_LITERAL(4, 35, 9), // "connected"
QT_MOC_LITERAL(5, 45, 12), // "disconnected"
QT_MOC_LITERAL(6, 58, 15), // "portOpenChanged"
QT_MOC_LITERAL(7, 74, 21), // "availablePortsChanged"
QT_MOC_LITERAL(8, 96, 6), // "dowork"
QT_MOC_LITERAL(9, 103, 4), // "open"
QT_MOC_LITERAL(10, 108, 8), // "portName"
QT_MOC_LITERAL(11, 117, 8), // "baudRate"
QT_MOC_LITERAL(12, 126, 5), // "close"
QT_MOC_LITERAL(13, 132, 6), // "isOpen"
QT_MOC_LITERAL(14, 139, 4), // "send"
QT_MOC_LITERAL(15, 144, 4), // "data"
QT_MOC_LITERAL(16, 149, 15), // "handleReadyRead"
QT_MOC_LITERAL(17, 165, 11), // "handleError"
QT_MOC_LITERAL(18, 177, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(19, 206, 5), // "error"
QT_MOC_LITERAL(20, 212, 8), // "openPort"
QT_MOC_LITERAL(21, 221, 9), // "closePort"
QT_MOC_LITERAL(22, 231, 9), // "scanPorts"
QT_MOC_LITERAL(23, 241, 8), // "sendData"
QT_MOC_LITERAL(24, 250, 14), // "availablePorts"
QT_MOC_LITERAL(25, 265, 8) // "portOpen"

    },
    "SerialPort\0errorOccurred\0\0errorMsg\0"
    "connected\0disconnected\0portOpenChanged\0"
    "availablePortsChanged\0dowork\0open\0"
    "portName\0baudRate\0close\0isOpen\0send\0"
    "data\0handleReadyRead\0handleError\0"
    "QSerialPort::SerialPortError\0error\0"
    "openPort\0closePort\0scanPorts\0sendData\0"
    "availablePorts\0portOpen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPort[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       2,  134, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       4,    0,  102,    2, 0x06 /* Public */,
       5,    0,  103,    2, 0x06 /* Public */,
       6,    0,  104,    2, 0x06 /* Public */,
       7,    0,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  106,    2, 0x0a /* Public */,
       9,    2,  107,    2, 0x0a /* Public */,
       9,    1,  112,    2, 0x2a /* Public | MethodCloned */,
      12,    0,  115,    2, 0x0a /* Public */,
      13,    0,  116,    2, 0x0a /* Public */,
      14,    1,  117,    2, 0x0a /* Public */,
      16,    0,  120,    2, 0x08 /* Private */,
      17,    1,  121,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      20,    2,  124,    2, 0x02 /* Public */,
      21,    0,  129,    2, 0x02 /* Public */,
      22,    0,  130,    2, 0x02 /* Public */,
      23,    1,  131,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::Int,   10,   11,
    QMetaType::Bool, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::LongLong, QMetaType::QByteArray,   15,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::Int,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QByteArray,   15,

 // properties: name, type, flags
      24, QMetaType::QStringList, 0x00495001,
      25, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       4,
       3,

       0        // eod
};

void SerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPort *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->portOpenChanged(); break;
        case 4: _t->availablePortsChanged(); break;
        case 5: _t->dowork(); break;
        case 6: { bool _r = _t->open((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint32(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->open((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->close(); break;
        case 9: { bool _r = _t->isOpen();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { qint64 _r = _t->send((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->handleReadyRead(); break;
        case 12: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 13: { bool _r = _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->closePort(); break;
        case 15: _t->scanPorts(); break;
        case 16: { bool _r = _t->sendData((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPort::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::errorOccurred)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::connected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::disconnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::portOpenChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::availablePortsChanged)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SerialPort *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isOpen(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SerialPort::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_SerialPort.data,
    qt_meta_data_SerialPort,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPort.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void SerialPort::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPort::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SerialPort::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SerialPort::portOpenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SerialPort::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
