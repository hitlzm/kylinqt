/****************************************************************************
** Meta object code from reading C++ file 'DataRecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../record/DataRecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataRecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataRecorder_t {
    QByteArrayData data[32];
    char stringdata0[431];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataRecorder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataRecorder_t qt_meta_stringdata_DataRecorder = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DataRecorder"
QT_MOC_LITERAL(1, 13, 13), // "savingChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 18), // "savingImageChanged"
QT_MOC_LITERAL(4, 47, 18), // "savingLaserChanged"
QT_MOC_LITERAL(5, 66, 22), // "savingTurntableChanged"
QT_MOC_LITERAL(6, 89, 14), // "saveDirChanged"
QT_MOC_LITERAL(7, 104, 17), // "ffmpegPathChanged"
QT_MOC_LITERAL(8, 122, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 136, 3), // "msg"
QT_MOC_LITERAL(10, 140, 20), // "videoConvertFinished"
QT_MOC_LITERAL(11, 161, 2), // "ok"
QT_MOC_LITERAL(12, 164, 7), // "mp4Path"
QT_MOC_LITERAL(13, 172, 22), // "videoRecordInterrupted"
QT_MOC_LITERAL(14, 195, 14), // "startImageSave"
QT_MOC_LITERAL(15, 210, 13), // "stopImageSave"
QT_MOC_LITERAL(16, 224, 14), // "startLaserSave"
QT_MOC_LITERAL(17, 239, 13), // "stopLaserSave"
QT_MOC_LITERAL(18, 253, 18), // "startTurntableSave"
QT_MOC_LITERAL(19, 272, 17), // "stopTurntableSave"
QT_MOC_LITERAL(20, 290, 7), // "stopAll"
QT_MOC_LITERAL(21, 298, 12), // "onImageFrame"
QT_MOC_LITERAL(22, 311, 12), // "onLaserFrame"
QT_MOC_LITERAL(23, 324, 16), // "onTurntableFrame"
QT_MOC_LITERAL(24, 341, 17), // "StatusFeedbackHex"
QT_MOC_LITERAL(25, 359, 5), // "frame"
QT_MOC_LITERAL(26, 365, 6), // "saving"
QT_MOC_LITERAL(27, 372, 11), // "savingImage"
QT_MOC_LITERAL(28, 384, 11), // "savingLaser"
QT_MOC_LITERAL(29, 396, 15), // "savingTurntable"
QT_MOC_LITERAL(30, 412, 7), // "saveDir"
QT_MOC_LITERAL(31, 420, 10) // "ffmpegPath"

    },
    "DataRecorder\0savingChanged\0\0"
    "savingImageChanged\0savingLaserChanged\0"
    "savingTurntableChanged\0saveDirChanged\0"
    "ffmpegPathChanged\0errorOccurred\0msg\0"
    "videoConvertFinished\0ok\0mp4Path\0"
    "videoRecordInterrupted\0startImageSave\0"
    "stopImageSave\0startLaserSave\0stopLaserSave\0"
    "startTurntableSave\0stopTurntableSave\0"
    "stopAll\0onImageFrame\0onLaserFrame\0"
    "onTurntableFrame\0StatusFeedbackHex\0"
    "frame\0saving\0savingImage\0savingLaser\0"
    "savingTurntable\0saveDir\0ffmpegPath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataRecorder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       6,  138, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    0,  110,    2, 0x06 /* Public */,
       4,    0,  111,    2, 0x06 /* Public */,
       5,    0,  112,    2, 0x06 /* Public */,
       6,    0,  113,    2, 0x06 /* Public */,
       7,    0,  114,    2, 0x06 /* Public */,
       8,    1,  115,    2, 0x06 /* Public */,
      10,    2,  118,    2, 0x06 /* Public */,
      13,    1,  123,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  126,    2, 0x0a /* Public */,
      15,    0,  127,    2, 0x0a /* Public */,
      16,    0,  128,    2, 0x0a /* Public */,
      17,    0,  129,    2, 0x0a /* Public */,
      18,    0,  130,    2, 0x0a /* Public */,
      19,    0,  131,    2, 0x0a /* Public */,
      20,    0,  132,    2, 0x0a /* Public */,
      21,    0,  133,    2, 0x0a /* Public */,
      22,    0,  134,    2, 0x0a /* Public */,
      23,    1,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,

 // properties: name, type, flags
      26, QMetaType::Bool, 0x00495001,
      27, QMetaType::Bool, 0x00495001,
      28, QMetaType::Bool, 0x00495001,
      29, QMetaType::Bool, 0x00495001,
      30, QMetaType::QString, 0x00495103,
      31, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void DataRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataRecorder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->savingChanged(); break;
        case 1: _t->savingImageChanged(); break;
        case 2: _t->savingLaserChanged(); break;
        case 3: _t->savingTurntableChanged(); break;
        case 4: _t->saveDirChanged(); break;
        case 5: _t->ffmpegPathChanged(); break;
        case 6: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->videoConvertFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->videoRecordInterrupted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: { bool _r = _t->startImageSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->stopImageSave(); break;
        case 11: { bool _r = _t->startLaserSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->stopLaserSave(); break;
        case 13: { bool _r = _t->startTurntableSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->stopTurntableSave(); break;
        case 15: _t->stopAll(); break;
        case 16: _t->onImageFrame(); break;
        case 17: _t->onLaserFrame(); break;
        case 18: _t->onTurntableFrame((*reinterpret_cast< const StatusFeedbackHex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::savingChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::savingImageChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::savingLaserChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::savingTurntableChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::saveDirChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::ffmpegPathChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::errorOccurred)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::videoConvertFinished)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (DataRecorder::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataRecorder::videoRecordInterrupted)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<DataRecorder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->saving(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->savingImage(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->savingLaser(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->savingTurntable(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->saveDir(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->ffmpegPath(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<DataRecorder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 4: _t->setSaveDir(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setFfmpegPath(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject DataRecorder::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DataRecorder.data,
    qt_meta_data_DataRecorder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataRecorder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
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
void DataRecorder::savingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DataRecorder::savingImageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DataRecorder::savingLaserChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DataRecorder::savingTurntableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DataRecorder::saveDirChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DataRecorder::ffmpegPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void DataRecorder::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DataRecorder::videoConvertFinished(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DataRecorder::videoRecordInterrupted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
