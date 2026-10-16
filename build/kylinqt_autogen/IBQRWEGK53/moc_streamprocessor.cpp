/****************************************************************************
** Meta object code from reading C++ file 'streamprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../opencv/streamprocessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'streamprocessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StreamProcessor_t {
    QByteArrayData data[16];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StreamProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StreamProcessor_t qt_meta_stringdata_StreamProcessor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "StreamProcessor"
QT_MOC_LITERAL(1, 16, 10), // "frameReady"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "frame"
QT_MOC_LITERAL(4, 34, 15), // "detectionsReady"
QT_MOC_LITERAL(5, 50, 26), // "std::vector<OnnxDetection>"
QT_MOC_LITERAL(6, 77, 10), // "detections"
QT_MOC_LITERAL(7, 88, 19), // "targetCenterChanged"
QT_MOC_LITERAL(8, 108, 7), // "centerX"
QT_MOC_LITERAL(9, 116, 7), // "centerY"
QT_MOC_LITERAL(10, 124, 13), // "errorOccurred"
QT_MOC_LITERAL(11, 138, 3), // "msg"
QT_MOC_LITERAL(12, 142, 8), // "finished"
QT_MOC_LITERAL(13, 151, 5), // "start"
QT_MOC_LITERAL(14, 157, 12), // "processFrame"
QT_MOC_LITERAL(15, 170, 4) // "stop"

    },
    "StreamProcessor\0frameReady\0\0frame\0"
    "detectionsReady\0std::vector<OnnxDetection>\0"
    "detections\0targetCenterChanged\0centerX\0"
    "centerY\0errorOccurred\0msg\0finished\0"
    "start\0processFrame\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StreamProcessor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       7,    2,   60,    2, 0x06 /* Public */,
      10,    1,   65,    2, 0x06 /* Public */,
      12,    0,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   69,    2, 0x0a /* Public */,
      14,    0,   70,    2, 0x0a /* Public */,
      15,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void StreamProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StreamProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 1: _t->detectionsReady((*reinterpret_cast< const std::vector<OnnxDetection>(*)>(_a[1]))); break;
        case 2: _t->targetCenterChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->finished(); break;
        case 5: _t->start(); break;
        case 6: _t->processFrame(); break;
        case 7: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StreamProcessor::*)(const QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StreamProcessor::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (StreamProcessor::*)(const std::vector<OnnxDetection> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StreamProcessor::detectionsReady)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (StreamProcessor::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StreamProcessor::targetCenterChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (StreamProcessor::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StreamProcessor::errorOccurred)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (StreamProcessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StreamProcessor::finished)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StreamProcessor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_StreamProcessor.data,
    qt_meta_data_StreamProcessor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StreamProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StreamProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StreamProcessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int StreamProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void StreamProcessor::frameReady(const QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void StreamProcessor::detectionsReady(const std::vector<OnnxDetection> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void StreamProcessor::targetCenterChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void StreamProcessor::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void StreamProcessor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
