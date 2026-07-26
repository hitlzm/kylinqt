/****************************************************************************
** Meta object code from reading C++ file 'VlcVideoItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../vlcvideo/VlcVideoItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VlcVideoItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VlcVideoItem_t {
    QByteArrayData data[32];
    char stringdata0[296];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VlcVideoItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VlcVideoItem_t qt_meta_stringdata_VlcVideoItem = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VlcVideoItem"
QT_MOC_LITERAL(1, 13, 13), // "sourceChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "playingChanged"
QT_MOC_LITERAL(4, 43, 13), // "volumeChanged"
QT_MOC_LITERAL(5, 57, 15), // "positionChanged"
QT_MOC_LITERAL(6, 73, 13), // "lengthChanged"
QT_MOC_LITERAL(7, 87, 15), // "seekableChanged"
QT_MOC_LITERAL(8, 103, 7), // "stopped"
QT_MOC_LITERAL(9, 111, 5), // "ended"
QT_MOC_LITERAL(10, 117, 5), // "error"
QT_MOC_LITERAL(11, 123, 8), // "errorMsg"
QT_MOC_LITERAL(12, 132, 9), // "pixelRead"
QT_MOC_LITERAL(13, 142, 6), // "frameX"
QT_MOC_LITERAL(14, 149, 6), // "frameY"
QT_MOC_LITERAL(15, 156, 17), // "errorReadingPixel"
QT_MOC_LITERAL(16, 174, 7), // "message"
QT_MOC_LITERAL(17, 182, 17), // "reqDeviationToImg"
QT_MOC_LITERAL(18, 200, 1), // "x"
QT_MOC_LITERAL(19, 202, 1), // "y"
QT_MOC_LITERAL(20, 204, 9), // "setVolume"
QT_MOC_LITERAL(21, 214, 3), // "vol"
QT_MOC_LITERAL(22, 218, 4), // "play"
QT_MOC_LITERAL(23, 223, 5), // "pause"
QT_MOC_LITERAL(24, 229, 4), // "stop"
QT_MOC_LITERAL(25, 234, 14), // "requestPixelAt"
QT_MOC_LITERAL(26, 249, 6), // "source"
QT_MOC_LITERAL(27, 256, 7), // "playing"
QT_MOC_LITERAL(28, 264, 6), // "volume"
QT_MOC_LITERAL(29, 271, 8), // "position"
QT_MOC_LITERAL(30, 280, 6), // "length"
QT_MOC_LITERAL(31, 287, 8) // "seekable"

    },
    "VlcVideoItem\0sourceChanged\0\0playingChanged\0"
    "volumeChanged\0positionChanged\0"
    "lengthChanged\0seekableChanged\0stopped\0"
    "ended\0error\0errorMsg\0pixelRead\0frameX\0"
    "frameY\0errorReadingPixel\0message\0"
    "reqDeviationToImg\0x\0y\0setVolume\0vol\0"
    "play\0pause\0stop\0requestPixelAt\0source\0"
    "playing\0volume\0position\0length\0seekable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VlcVideoItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       6,  134, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

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
      12,    2,  110,    2, 0x06 /* Public */,
      15,    1,  115,    2, 0x06 /* Public */,
      17,    2,  118,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      20,    1,  123,    2, 0x02 /* Public */,
      22,    0,  126,    2, 0x02 /* Public */,
      23,    0,  127,    2, 0x02 /* Public */,
      24,    0,  128,    2, 0x02 /* Public */,
      25,    2,  129,    2, 0x02 /* Public */,

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
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   13,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   18,   19,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   18,   19,

 // properties: name, type, flags
      26, QMetaType::QString, 0x00495103,
      27, QMetaType::Bool, 0x00495001,
      28, QMetaType::Int, 0x00495103,
      29, QMetaType::Float, 0x00495103,
      30, QMetaType::LongLong, 0x00495001,
      31, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void VlcVideoItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VlcVideoItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sourceChanged(); break;
        case 1: _t->playingChanged(); break;
        case 2: _t->volumeChanged(); break;
        case 3: _t->positionChanged(); break;
        case 4: _t->lengthChanged(); break;
        case 5: _t->seekableChanged(); break;
        case 6: _t->stopped(); break;
        case 7: _t->ended(); break;
        case 8: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->pixelRead((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->errorReadingPixel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->reqDeviationToImg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->play(); break;
        case 14: _t->pause(); break;
        case 15: _t->stop(); break;
        case 16: _t->requestPixelAt((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::sourceChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::playingChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::volumeChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::positionChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::lengthChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::seekableChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::stopped)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::ended)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::error)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::pixelRead)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::errorReadingPixel)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (VlcVideoItem::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcVideoItem::reqDeviationToImg)) {
                *result = 11;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VlcVideoItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->source(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isPlaying(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->volume(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->position(); break;
        case 4: *reinterpret_cast< qint64*>(_v) = _t->length(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->isSeekable(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VlcVideoItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSource(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setVolume(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setPosition(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject VlcVideoItem::staticMetaObject = { {
    &QQuickFramebufferObject::staticMetaObject,
    qt_meta_stringdata_VlcVideoItem.data,
    qt_meta_data_VlcVideoItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VlcVideoItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VlcVideoItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VlcVideoItem.stringdata0))
        return static_cast<void*>(this);
    return QQuickFramebufferObject::qt_metacast(_clname);
}

int VlcVideoItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickFramebufferObject::qt_metacall(_c, _id, _a);
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
void VlcVideoItem::sourceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VlcVideoItem::playingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VlcVideoItem::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VlcVideoItem::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void VlcVideoItem::lengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void VlcVideoItem::seekableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void VlcVideoItem::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void VlcVideoItem::ended()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void VlcVideoItem::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void VlcVideoItem::pixelRead(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void VlcVideoItem::errorReadingPixel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void VlcVideoItem::reqDeviationToImg(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
