/****************************************************************************
** Meta object code from reading C++ file 'vlcplayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../video/vlcplayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vlcplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VLCPlayer_t {
    QByteArrayData data[26];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VLCPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VLCPlayer_t qt_meta_stringdata_VLCPlayer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "VLCPlayer"
QT_MOC_LITERAL(1, 10, 10), // "urlChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 14), // "playingChanged"
QT_MOC_LITERAL(4, 37, 13), // "volumeChanged"
QT_MOC_LITERAL(5, 51, 15), // "positionChanged"
QT_MOC_LITERAL(6, 67, 13), // "lengthChanged"
QT_MOC_LITERAL(7, 81, 15), // "seekableChanged"
QT_MOC_LITERAL(8, 97, 7), // "stopped"
QT_MOC_LITERAL(9, 105, 5), // "ended"
QT_MOC_LITERAL(10, 111, 5), // "error"
QT_MOC_LITERAL(11, 117, 8), // "errorMsg"
QT_MOC_LITERAL(12, 126, 14), // "onStateChanged"
QT_MOC_LITERAL(13, 141, 15), // "onLengthChanged"
QT_MOC_LITERAL(14, 157, 6), // "length"
QT_MOC_LITERAL(15, 164, 17), // "onSeekableChanged"
QT_MOC_LITERAL(16, 182, 8), // "seekable"
QT_MOC_LITERAL(17, 191, 4), // "play"
QT_MOC_LITERAL(18, 196, 5), // "pause"
QT_MOC_LITERAL(19, 202, 4), // "stop"
QT_MOC_LITERAL(20, 207, 9), // "openLocal"
QT_MOC_LITERAL(21, 217, 7), // "openUrl"
QT_MOC_LITERAL(22, 225, 3), // "url"
QT_MOC_LITERAL(23, 229, 7), // "playing"
QT_MOC_LITERAL(24, 237, 6), // "volume"
QT_MOC_LITERAL(25, 244, 8) // "position"

    },
    "VLCPlayer\0urlChanged\0\0playingChanged\0"
    "volumeChanged\0positionChanged\0"
    "lengthChanged\0seekableChanged\0stopped\0"
    "ended\0error\0errorMsg\0onStateChanged\0"
    "onLengthChanged\0length\0onSeekableChanged\0"
    "seekable\0play\0pause\0stop\0openLocal\0"
    "openUrl\0url\0playing\0volume\0position"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VLCPlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       6,  124, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    0,  103,    2, 0x06 /* Public */,
       7,    0,  104,    2, 0x06 /* Public */,
       8,    0,  105,    2, 0x06 /* Public */,
       9,    0,  106,    2, 0x06 /* Public */,
      10,    1,  107,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  110,    2, 0x08 /* Private */,
      13,    1,  111,    2, 0x08 /* Private */,
      15,    1,  114,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      17,    0,  117,    2, 0x02 /* Public */,
      18,    0,  118,    2, 0x02 /* Public */,
      19,    0,  119,    2, 0x02 /* Public */,
      20,    0,  120,    2, 0x02 /* Public */,
      21,    1,  121,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   14,
    QMetaType::Void, QMetaType::Bool,   16,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QUrl,   22,

 // properties: name, type, flags
      22, QMetaType::QUrl, 0x00495103,
      23, QMetaType::Bool, 0x00495001,
      24, QMetaType::Int, 0x00495103,
      25, QMetaType::Float, 0x00495103,
      14, QMetaType::LongLong, 0x00495001,
      16, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void VLCPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VLCPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->urlChanged(); break;
        case 1: _t->playingChanged(); break;
        case 2: _t->volumeChanged(); break;
        case 3: _t->positionChanged(); break;
        case 4: _t->lengthChanged(); break;
        case 5: _t->seekableChanged(); break;
        case 6: _t->stopped(); break;
        case 7: _t->ended(); break;
        case 8: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onStateChanged(); break;
        case 10: _t->onLengthChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 11: _t->onSeekableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->play(); break;
        case 13: _t->pause(); break;
        case 14: _t->stop(); break;
        case 15: _t->openLocal(); break;
        case 16: _t->openUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::urlChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::playingChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::volumeChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::positionChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::lengthChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::seekableChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::stopped)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::ended)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VLCPlayer::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VLCPlayer::error)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VLCPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QUrl*>(_v) = _t->url(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isPlaying(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->volume(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->position(); break;
        case 4: *reinterpret_cast< qint64*>(_v) = _t->length(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->isSeekable(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VLCPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUrl(*reinterpret_cast< QUrl*>(_v)); break;
        case 2: _t->setVolume(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setPosition(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject VLCPlayer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VLCPlayer.data,
    qt_meta_data_VLCPlayer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VLCPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VLCPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VLCPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VLCPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void VLCPlayer::urlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VLCPlayer::playingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VLCPlayer::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VLCPlayer::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void VLCPlayer::lengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void VLCPlayer::seekableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void VLCPlayer::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void VLCPlayer::ended()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void VLCPlayer::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
