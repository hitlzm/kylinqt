/****************************************************************************
** Meta object code from reading C++ file 'AuthManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../auth/AuthManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AuthManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AuthManager_t {
    QByteArrayData data[16];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuthManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuthManager_t qt_meta_stringdata_AuthManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AuthManager"
QT_MOC_LITERAL(1, 12, 20), // "authenticatedChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 19), // "attemptsLeftChanged"
QT_MOC_LITERAL(4, 54, 22), // "lockSecondsLeftChanged"
QT_MOC_LITERAL(5, 77, 14), // "loginSucceeded"
QT_MOC_LITERAL(6, 92, 12), // "verifyFailed"
QT_MOC_LITERAL(7, 105, 6), // "reason"
QT_MOC_LITERAL(8, 112, 16), // "startupRequested"
QT_MOC_LITERAL(9, 129, 6), // "verify"
QT_MOC_LITERAL(10, 136, 5), // "input"
QT_MOC_LITERAL(11, 142, 6), // "giveUp"
QT_MOC_LITERAL(12, 149, 12), // "beginStartup"
QT_MOC_LITERAL(13, 162, 13), // "authenticated"
QT_MOC_LITERAL(14, 176, 12), // "attemptsLeft"
QT_MOC_LITERAL(15, 189, 15) // "lockSecondsLeft"

    },
    "AuthManager\0authenticatedChanged\0\0"
    "attemptsLeftChanged\0lockSecondsLeftChanged\0"
    "loginSucceeded\0verifyFailed\0reason\0"
    "startupRequested\0verify\0input\0giveUp\0"
    "beginStartup\0authenticated\0attemptsLeft\0"
    "lockSecondsLeft"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuthManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       3,   72, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,
       5,    0,   62,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,
       8,    0,   66,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    1,   67,    2, 0x02 /* Public */,
      11,    0,   70,    2, 0x02 /* Public */,
      12,    0,   71,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      13, QMetaType::Bool, 0x00495001,
      14, QMetaType::Int, 0x00495001,
      15, QMetaType::Int, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void AuthManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AuthManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->authenticatedChanged(); break;
        case 1: _t->attemptsLeftChanged(); break;
        case 2: _t->lockSecondsLeftChanged(); break;
        case 3: _t->loginSucceeded(); break;
        case 4: _t->verifyFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->startupRequested(); break;
        case 6: { bool _r = _t->verify((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->giveUp(); break;
        case 8: _t->beginStartup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AuthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::authenticatedChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AuthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::attemptsLeftChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AuthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::lockSecondsLeftChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (AuthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::loginSucceeded)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (AuthManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::verifyFailed)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (AuthManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthManager::startupRequested)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<AuthManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->authenticated(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->attemptsLeft(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->lockSecondsLeft(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject AuthManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_AuthManager.data,
    qt_meta_data_AuthManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AuthManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuthManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AuthManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AuthManager::authenticatedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AuthManager::attemptsLeftChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AuthManager::lockSecondsLeftChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AuthManager::loginSucceeded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AuthManager::verifyFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AuthManager::startupRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
