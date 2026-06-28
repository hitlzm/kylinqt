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
struct qt_meta_stringdata_SerialPortTurntable_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortTurntable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortTurntable_t qt_meta_stringdata_SerialPortTurntable = {
    {
QT_MOC_LITERAL(0, 0, 19) // "SerialPortTurntable"

    },
    "SerialPortTurntable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortTurntable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SerialPortTurntable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
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
    return _id;
}
struct qt_meta_stringdata_TurntableData_t {
    QByteArrayData data[24];
    char stringdata0[434];
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
QT_MOC_LITERAL(13, 263, 4), // "time"
QT_MOC_LITERAL(14, 268, 9), // "ctlnumber"
QT_MOC_LITERAL(15, 278, 18), // "inner_statusnumber"
QT_MOC_LITERAL(16, 297, 11), // "inner_angle"
QT_MOC_LITERAL(17, 309, 18), // "inner_ctlDeviation"
QT_MOC_LITERAL(18, 328, 19), // "middle_statusnumber"
QT_MOC_LITERAL(19, 348, 12), // "middle_angle"
QT_MOC_LITERAL(20, 361, 19), // "middle_ctlDeviation"
QT_MOC_LITERAL(21, 381, 19), // "outter_statusnumber"
QT_MOC_LITERAL(22, 401, 12), // "outter_angle"
QT_MOC_LITERAL(23, 414, 19) // "outter_ctlDeviation"

    },
    "TurntableData\0timeChanged\0\0ctlnumberChanged\0"
    "inner_statusnumberChanged\0inner_angleChanged\0"
    "inner_ctlDeviationChanged\0"
    "middle_statusnumberChanged\0"
    "middle_angleChanged\0middle_ctlDeviationChanged\0"
    "outter_statusnumberChanged\0"
    "outter_angleChanged\0outter_ctlDeviationChanged\0"
    "time\0ctlnumber\0inner_statusnumber\0"
    "inner_angle\0inner_ctlDeviation\0"
    "middle_statusnumber\0middle_angle\0"
    "middle_ctlDeviation\0outter_statusnumber\0"
    "outter_angle\0outter_ctlDeviation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
      11,   80, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,
       9,    0,   76,    2, 0x06 /* Public */,
      10,    0,   77,    2, 0x06 /* Public */,
      11,    0,   78,    2, 0x06 /* Public */,
      12,    0,   79,    2, 0x06 /* Public */,

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

 // properties: name, type, flags
      13, QMetaType::Int, 0x00495003,
      14, QMetaType::Int, 0x00495003,
      15, QMetaType::Int, 0x00495003,
      16, QMetaType::Float, 0x00495003,
      17, QMetaType::Float, 0x00495003,
      18, QMetaType::Int, 0x00495003,
      19, QMetaType::Float, 0x00495003,
      20, QMetaType::Float, 0x00495003,
      21, QMetaType::Int, 0x00495003,
      22, QMetaType::Float, 0x00495003,
      23, QMetaType::Float, 0x00495003,

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
    Q_UNUSED(_a);
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
