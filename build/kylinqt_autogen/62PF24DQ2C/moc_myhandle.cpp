/****************************************************************************
** Meta object code from reading C++ file 'myhandle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../handle/myhandle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myhandle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Myhandle_t {
    QByteArrayData data[20];
    char stringdata0[238];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Myhandle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Myhandle_t qt_meta_stringdata_Myhandle = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Myhandle"
QT_MOC_LITERAL(1, 9, 16), // "reqturntablesend"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 16), // "axisLeftXChanged"
QT_MOC_LITERAL(4, 44, 5), // "value"
QT_MOC_LITERAL(5, 50, 16), // "axisLeftYChanged"
QT_MOC_LITERAL(6, 67, 17), // "axisRightXChanged"
QT_MOC_LITERAL(7, 85, 17), // "axisRightYChanged"
QT_MOC_LITERAL(8, 103, 15), // "buttonL2Changed"
QT_MOC_LITERAL(9, 119, 15), // "buttonR2Changed"
QT_MOC_LITERAL(10, 135, 14), // "buttonAChanged"
QT_MOC_LITERAL(11, 150, 7), // "pressed"
QT_MOC_LITERAL(12, 158, 14), // "buttonBChanged"
QT_MOC_LITERAL(13, 173, 9), // "axisLeftX"
QT_MOC_LITERAL(14, 183, 9), // "axisLeftY"
QT_MOC_LITERAL(15, 193, 10), // "axisRightX"
QT_MOC_LITERAL(16, 204, 8), // "buttonL2"
QT_MOC_LITERAL(17, 213, 8), // "buttonR2"
QT_MOC_LITERAL(18, 222, 7), // "buttonA"
QT_MOC_LITERAL(19, 230, 7) // "buttonB"

    },
    "Myhandle\0reqturntablesend\0\0axisLeftXChanged\0"
    "value\0axisLeftYChanged\0axisRightXChanged\0"
    "axisRightYChanged\0buttonL2Changed\0"
    "buttonR2Changed\0buttonAChanged\0pressed\0"
    "buttonBChanged\0axisLeftX\0axisLeftY\0"
    "axisRightX\0buttonL2\0buttonR2\0buttonA\0"
    "buttonB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Myhandle[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       7,   84, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,
       6,    1,   66,    2, 0x06 /* Public */,
       7,    1,   69,    2, 0x06 /* Public */,
       8,    1,   72,    2, 0x06 /* Public */,
       9,    1,   75,    2, 0x06 /* Public */,
      10,    1,   78,    2, 0x06 /* Public */,
      12,    1,   81,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,

 // properties: name, type, flags
      13, QMetaType::Float, 0x00495003,
      14, QMetaType::Float, 0x00495003,
      15, QMetaType::Float, 0x00495003,
      16, QMetaType::Float, 0x00495003,
      17, QMetaType::Float, 0x00495003,
      18, QMetaType::Bool, 0x00495003,
      19, QMetaType::Bool, 0x00495003,

 // properties: notify_signal_id
       1,
       2,
       3,
       5,
       6,
       7,
       8,

       0        // eod
};

void Myhandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Myhandle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reqturntablesend(); break;
        case 1: _t->axisLeftXChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->axisLeftYChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->axisRightXChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->axisRightYChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->buttonL2Changed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->buttonR2Changed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->buttonAChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->buttonBChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Myhandle::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::reqturntablesend)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::axisLeftXChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::axisLeftYChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::axisRightXChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::axisRightYChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::buttonL2Changed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::buttonR2Changed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::buttonAChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Myhandle::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::buttonBChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Myhandle *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->m_axisLeftX; break;
        case 1: *reinterpret_cast< float*>(_v) = _t->m_axisLeftY; break;
        case 2: *reinterpret_cast< float*>(_v) = _t->m_axisRightX; break;
        case 3: *reinterpret_cast< float*>(_v) = _t->m_buttonL2; break;
        case 4: *reinterpret_cast< float*>(_v) = _t->m_buttonR2; break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->m_buttonA; break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->m_buttonB; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Myhandle *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_axisLeftX != *reinterpret_cast< float*>(_v)) {
                _t->m_axisLeftX = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->axisLeftXChanged(_t->m_axisLeftX);
            }
            break;
        case 1:
            if (_t->m_axisLeftY != *reinterpret_cast< float*>(_v)) {
                _t->m_axisLeftY = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->axisLeftYChanged(_t->m_axisLeftY);
            }
            break;
        case 2:
            if (_t->m_axisRightX != *reinterpret_cast< float*>(_v)) {
                _t->m_axisRightX = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->axisRightXChanged(_t->m_axisRightX);
            }
            break;
        case 3:
            if (_t->m_buttonL2 != *reinterpret_cast< float*>(_v)) {
                _t->m_buttonL2 = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->buttonL2Changed(_t->m_buttonL2);
            }
            break;
        case 4:
            if (_t->m_buttonR2 != *reinterpret_cast< float*>(_v)) {
                _t->m_buttonR2 = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->buttonR2Changed(_t->m_buttonR2);
            }
            break;
        case 5:
            if (_t->m_buttonA != *reinterpret_cast< bool*>(_v)) {
                _t->m_buttonA = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->buttonAChanged(_t->m_buttonA);
            }
            break;
        case 6:
            if (_t->m_buttonB != *reinterpret_cast< bool*>(_v)) {
                _t->m_buttonB = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->buttonBChanged(_t->m_buttonB);
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject Myhandle::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Myhandle.data,
    qt_meta_data_Myhandle,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Myhandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Myhandle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Myhandle.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Myhandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Myhandle::reqturntablesend()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Myhandle::axisLeftXChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Myhandle::axisLeftYChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Myhandle::axisRightXChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Myhandle::axisRightYChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Myhandle::buttonL2Changed(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Myhandle::buttonR2Changed(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Myhandle::buttonAChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Myhandle::buttonBChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
