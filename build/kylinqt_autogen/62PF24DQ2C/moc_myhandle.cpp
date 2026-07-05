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
    QByteArrayData data[22];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Myhandle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Myhandle_t qt_meta_stringdata_Myhandle = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Myhandle"
QT_MOC_LITERAL(1, 9, 16), // "handleModeSignal"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "axisLeftX"
QT_MOC_LITERAL(4, 37, 9), // "axisLeftY"
QT_MOC_LITERAL(5, 47, 10), // "axisRightX"
QT_MOC_LITERAL(6, 58, 8), // "buttonL2"
QT_MOC_LITERAL(7, 67, 8), // "buttonR2"
QT_MOC_LITERAL(8, 76, 7), // "buttonA"
QT_MOC_LITERAL(9, 84, 7), // "buttonB"
QT_MOC_LITERAL(10, 92, 11), // "modechanged"
QT_MOC_LITERAL(11, 104, 5), // "index"
QT_MOC_LITERAL(12, 110, 16), // "axisLeftXChanged"
QT_MOC_LITERAL(13, 127, 5), // "value"
QT_MOC_LITERAL(14, 133, 16), // "axisLeftYChanged"
QT_MOC_LITERAL(15, 150, 17), // "axisRightXChanged"
QT_MOC_LITERAL(16, 168, 15), // "buttonL2Changed"
QT_MOC_LITERAL(17, 184, 15), // "buttonR2Changed"
QT_MOC_LITERAL(18, 200, 14), // "buttonAChanged"
QT_MOC_LITERAL(19, 215, 7), // "pressed"
QT_MOC_LITERAL(20, 223, 14), // "buttonBChanged"
QT_MOC_LITERAL(21, 238, 6) // "update"

    },
    "Myhandle\0handleModeSignal\0\0axisLeftX\0"
    "axisLeftY\0axisRightX\0buttonL2\0buttonR2\0"
    "buttonA\0buttonB\0modechanged\0index\0"
    "axisLeftXChanged\0value\0axisLeftYChanged\0"
    "axisRightXChanged\0buttonL2Changed\0"
    "buttonR2Changed\0buttonAChanged\0pressed\0"
    "buttonBChanged\0update"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Myhandle[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    7,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   79,    2, 0x0a /* Public */,
      12,    1,   82,    2, 0x0a /* Public */,
      14,    1,   85,    2, 0x0a /* Public */,
      15,    1,   88,    2, 0x0a /* Public */,
      16,    1,   91,    2, 0x0a /* Public */,
      17,    1,   94,    2, 0x0a /* Public */,
      18,    1,   97,    2, 0x0a /* Public */,
      20,    1,  100,    2, 0x0a /* Public */,
      21,    7,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::Bool,    3,    4,    5,    6,    7,    8,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::Bool, QMetaType::Float, QMetaType::Float,    3,    4,    5,    8,    9,    6,    7,

       0        // eod
};

void Myhandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Myhandle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleModeSignal((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 1: _t->modechanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->axisLeftXChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->axisLeftYChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->axisRightXChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->buttonL2Changed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->buttonR2Changed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->buttonAChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->buttonBChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->update((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6])),(*reinterpret_cast< float(*)>(_a[7]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Myhandle::*)(float , float , float , float , float , bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myhandle::handleModeSignal)) {
                *result = 0;
                return;
            }
        }
    }
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Myhandle::handleModeSignal(float _t1, float _t2, float _t3, float _t4, float _t5, bool _t6, bool _t7)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_GamepadBridge_t {
    QByteArrayData data[19];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GamepadBridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GamepadBridge_t qt_meta_stringdata_GamepadBridge = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GamepadBridge"
QT_MOC_LITERAL(1, 14, 15), // "axisLeftXChange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "value"
QT_MOC_LITERAL(4, 37, 15), // "axisLeftYChange"
QT_MOC_LITERAL(5, 53, 16), // "axisRightXChange"
QT_MOC_LITERAL(6, 70, 14), // "buttonL2Change"
QT_MOC_LITERAL(7, 85, 14), // "buttonR2Change"
QT_MOC_LITERAL(8, 100, 13), // "buttonAChange"
QT_MOC_LITERAL(9, 114, 7), // "pressed"
QT_MOC_LITERAL(10, 122, 13), // "buttonBChange"
QT_MOC_LITERAL(11, 136, 13), // "updateGamepad"
QT_MOC_LITERAL(12, 150, 9), // "axisLeftX"
QT_MOC_LITERAL(13, 160, 9), // "axisLeftY"
QT_MOC_LITERAL(14, 170, 10), // "axisRightX"
QT_MOC_LITERAL(15, 181, 7), // "buttonA"
QT_MOC_LITERAL(16, 189, 7), // "buttonB"
QT_MOC_LITERAL(17, 197, 8), // "buttonL2"
QT_MOC_LITERAL(18, 206, 8) // "buttonR2"

    },
    "GamepadBridge\0axisLeftXChange\0\0value\0"
    "axisLeftYChange\0axisRightXChange\0"
    "buttonL2Change\0buttonR2Change\0"
    "buttonAChange\0pressed\0buttonBChange\0"
    "updateGamepad\0axisLeftX\0axisLeftY\0"
    "axisRightX\0buttonA\0buttonB\0buttonL2\0"
    "buttonR2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GamepadBridge[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,
       7,    1,   66,    2, 0x06 /* Public */,
       8,    1,   69,    2, 0x06 /* Public */,
      10,    1,   72,    2, 0x06 /* Public */,
      11,    7,   75,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::Bool, QMetaType::Float, QMetaType::Float,   12,   13,   14,   15,   16,   17,   18,

       0        // eod
};

void GamepadBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GamepadBridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->axisLeftXChange((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->axisLeftYChange((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->axisRightXChange((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->buttonL2Change((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->buttonR2Change((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->buttonAChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->buttonBChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->updateGamepad((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6])),(*reinterpret_cast< float(*)>(_a[7]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GamepadBridge::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::axisLeftXChange)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::axisLeftYChange)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::axisRightXChange)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::buttonL2Change)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::buttonR2Change)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::buttonAChange)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::buttonBChange)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (GamepadBridge::*)(float , float , float , bool , bool , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GamepadBridge::updateGamepad)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GamepadBridge::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_GamepadBridge.data,
    qt_meta_data_GamepadBridge,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GamepadBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GamepadBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GamepadBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GamepadBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void GamepadBridge::axisLeftXChange(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GamepadBridge::axisLeftYChange(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GamepadBridge::axisRightXChange(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GamepadBridge::buttonL2Change(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GamepadBridge::buttonR2Change(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GamepadBridge::buttonAChange(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GamepadBridge::buttonBChange(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GamepadBridge::updateGamepad(float _t1, float _t2, float _t3, bool _t4, bool _t5, float _t6, float _t7)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
