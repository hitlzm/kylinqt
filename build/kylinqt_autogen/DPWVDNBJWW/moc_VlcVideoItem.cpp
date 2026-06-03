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
    QByteArrayData data[21];
    char stringdata0[195];
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
QT_MOC_LITERAL(12, 132, 4), // "play"
QT_MOC_LITERAL(13, 137, 5), // "pause"
QT_MOC_LITERAL(14, 143, 4), // "stop"
QT_MOC_LITERAL(15, 148, 6), // "source"
QT_MOC_LITERAL(16, 155, 7), // "playing"
QT_MOC_LITERAL(17, 163, 6), // "volume"
QT_MOC_LITERAL(18, 170, 8), // "position"
QT_MOC_LITERAL(19, 179, 6), // "length"
QT_MOC_LITERAL(20, 186, 8) // "seekable"

    },
    "VlcVideoItem\0sourceChanged\0\0playingChanged\0"
    "volumeChanged\0positionChanged\0"
    "lengthChanged\0seekableChanged\0stopped\0"
    "ended\0error\0errorMsg\0play\0pause\0stop\0"
    "source\0playing\0volume\0position\0length\0"
    "seekable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VlcVideoItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       6,   88, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    0,   78,    2, 0x06 /* Public */,
       7,    0,   79,    2, 0x06 /* Public */,
       8,    0,   80,    2, 0x06 /* Public */,
       9,    0,   81,    2, 0x06 /* Public */,
      10,    1,   82,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      12,    0,   85,    2, 0x02 /* Public */,
      13,    0,   86,    2, 0x02 /* Public */,
      14,    0,   87,    2, 0x02 /* Public */,

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

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      15, QMetaType::QString, 0x00495103,
      16, QMetaType::Bool, 0x00495001,
      17, QMetaType::Int, 0x00495103,
      18, QMetaType::Float, 0x00495103,
      19, QMetaType::LongLong, 0x00495001,
      20, QMetaType::Bool, 0x00495001,

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
        case 9: _t->play(); break;
        case 10: _t->pause(); break;
        case 11: _t->stop(); break;
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
    &QQuickPaintedItem::staticMetaObject,
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
    return QQuickPaintedItem::qt_metacast(_clname);
}

int VlcVideoItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
