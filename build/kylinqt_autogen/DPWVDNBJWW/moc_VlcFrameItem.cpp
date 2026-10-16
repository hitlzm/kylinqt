/****************************************************************************
** Meta object code from reading C++ file 'VlcFrameItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../vlcvideo/VlcFrameItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VlcFrameItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VlcFrameItem_t {
    QByteArrayData data[11];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VlcFrameItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VlcFrameItem_t qt_meta_stringdata_VlcFrameItem = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VlcFrameItem"
QT_MOC_LITERAL(1, 13, 13), // "sourceChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 17), // "zoomFactorChanged"
QT_MOC_LITERAL(4, 46, 14), // "centerXChanged"
QT_MOC_LITERAL(5, 61, 14), // "centerYChanged"
QT_MOC_LITERAL(6, 76, 6), // "source"
QT_MOC_LITERAL(7, 83, 13), // "VlcVideoItem*"
QT_MOC_LITERAL(8, 97, 10), // "zoomFactor"
QT_MOC_LITERAL(9, 108, 7), // "centerX"
QT_MOC_LITERAL(10, 116, 7) // "centerY"

    },
    "VlcFrameItem\0sourceChanged\0\0"
    "zoomFactorChanged\0centerXChanged\0"
    "centerYChanged\0source\0VlcVideoItem*\0"
    "zoomFactor\0centerX\0centerY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VlcFrameItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, 0x80000000 | 7, 0x0049510b,
       8, QMetaType::QReal, 0x00495103,
       9, QMetaType::QReal, 0x00495103,
      10, QMetaType::QReal, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,

       0        // eod
};

void VlcFrameItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VlcFrameItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sourceChanged(); break;
        case 1: _t->zoomFactorChanged(); break;
        case 2: _t->centerXChanged(); break;
        case 3: _t->centerYChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VlcFrameItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcFrameItem::sourceChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VlcFrameItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcFrameItem::zoomFactorChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VlcFrameItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcFrameItem::centerXChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VlcFrameItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VlcFrameItem::centerYChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<VlcFrameItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< VlcVideoItem**>(_v) = _t->source(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->zoomFactor(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->centerX(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->centerY(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<VlcFrameItem *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSource(*reinterpret_cast< VlcVideoItem**>(_v)); break;
        case 1: _t->setZoomFactor(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setCenterX(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setCenterY(*reinterpret_cast< qreal*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject VlcFrameItem::staticMetaObject = { {
    &QQuickFramebufferObject::staticMetaObject,
    qt_meta_stringdata_VlcFrameItem.data,
    qt_meta_data_VlcFrameItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VlcFrameItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VlcFrameItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VlcFrameItem.stringdata0))
        return static_cast<void*>(this);
    return QQuickFramebufferObject::qt_metacast(_clname);
}

int VlcFrameItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickFramebufferObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void VlcFrameItem::sourceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VlcFrameItem::zoomFactorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VlcFrameItem::centerXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VlcFrameItem::centerYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
