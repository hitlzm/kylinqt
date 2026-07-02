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
struct qt_meta_stringdata_TurntableData_t {
    QByteArrayData data[30];
    char stringdata0[535];
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
QT_MOC_LITERAL(13, 263, 20), // "myinner_angleChanged"
QT_MOC_LITERAL(14, 284, 11), // "inner_angle"
QT_MOC_LITERAL(15, 296, 21), // "mymiddle_angleChanged"
QT_MOC_LITERAL(16, 318, 12), // "middle_angle"
QT_MOC_LITERAL(17, 331, 21), // "myoutter_angleChanged"
QT_MOC_LITERAL(18, 353, 12), // "outter_angle"
QT_MOC_LITERAL(19, 366, 11), // "updateframe"
QT_MOC_LITERAL(20, 378, 14), // "StatusFeedback"
QT_MOC_LITERAL(21, 393, 8), // "recvdata"
QT_MOC_LITERAL(22, 402, 4), // "time"
QT_MOC_LITERAL(23, 407, 9), // "ctlnumber"
QT_MOC_LITERAL(24, 417, 18), // "inner_statusnumber"
QT_MOC_LITERAL(25, 436, 18), // "inner_ctlDeviation"
QT_MOC_LITERAL(26, 455, 19), // "middle_statusnumber"
QT_MOC_LITERAL(27, 475, 19), // "middle_ctlDeviation"
QT_MOC_LITERAL(28, 495, 19), // "outter_statusnumber"
QT_MOC_LITERAL(29, 515, 19) // "outter_ctlDeviation"

    },
    "TurntableData\0timeChanged\0\0ctlnumberChanged\0"
    "inner_statusnumberChanged\0inner_angleChanged\0"
    "inner_ctlDeviationChanged\0"
    "middle_statusnumberChanged\0"
    "middle_angleChanged\0middle_ctlDeviationChanged\0"
    "outter_statusnumberChanged\0"
    "outter_angleChanged\0outter_ctlDeviationChanged\0"
    "myinner_angleChanged\0inner_angle\0"
    "mymiddle_angleChanged\0middle_angle\0"
    "myoutter_angleChanged\0outter_angle\0"
    "updateframe\0StatusFeedback\0recvdata\0"
    "time\0ctlnumber\0inner_statusnumber\0"
    "inner_ctlDeviation\0middle_statusnumber\0"
    "middle_ctlDeviation\0outter_statusnumber\0"
    "outter_ctlDeviation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
      11,  112, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    0,   90,    2, 0x06 /* Public */,
       4,    0,   91,    2, 0x06 /* Public */,
       5,    0,   92,    2, 0x06 /* Public */,
       6,    0,   93,    2, 0x06 /* Public */,
       7,    0,   94,    2, 0x06 /* Public */,
       8,    0,   95,    2, 0x06 /* Public */,
       9,    0,   96,    2, 0x06 /* Public */,
      10,    0,   97,    2, 0x06 /* Public */,
      11,    0,   98,    2, 0x06 /* Public */,
      12,    0,   99,    2, 0x06 /* Public */,
      13,    1,  100,    2, 0x06 /* Public */,
      15,    1,  103,    2, 0x06 /* Public */,
      17,    1,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    1,  109,    2, 0x0a /* Public */,

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
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, QMetaType::Float,   18,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 20,   21,

 // properties: name, type, flags
      22, QMetaType::Int, 0x00495003,
      23, QMetaType::Int, 0x00495003,
      24, QMetaType::Int, 0x00495003,
      14, QMetaType::Float, 0x00495003,
      25, QMetaType::Float, 0x00495003,
      26, QMetaType::Int, 0x00495003,
      16, QMetaType::Float, 0x00495003,
      27, QMetaType::Float, 0x00495003,
      28, QMetaType::Int, 0x00495003,
      18, QMetaType::Float, 0x00495003,
      29, QMetaType::Float, 0x00495003,

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
        case 11: _t->myinner_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 12: _t->mymiddle_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 13: _t->myoutter_angleChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->updateframe((*reinterpret_cast< StatusFeedback(*)>(_a[1]))); break;
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
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::myinner_angleChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::mymiddle_angleChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TurntableData::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableData::myoutter_angleChanged)) {
                *result = 13;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
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

// SIGNAL 11
void TurntableData::myinner_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void TurntableData::mymiddle_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void TurntableData::myoutter_angleChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
struct qt_meta_stringdata_TurntableSendData_t {
    QByteArrayData data[30];
    char stringdata0[504];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TurntableSendData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TurntableSendData_t qt_meta_stringdata_TurntableSendData = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TurntableSendData"
QT_MOC_LITERAL(1, 18, 20), // "requestOpenTurntable"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 21), // "requestCloseTurntable"
QT_MOC_LITERAL(4, 62, 21), // "requestResetTurntable"
QT_MOC_LITERAL(5, 84, 20), // "requestZeroTurntable"
QT_MOC_LITERAL(6, 105, 22), // "requestSendProgramMode"
QT_MOC_LITERAL(7, 128, 18), // "programSend_frame&"
QT_MOC_LITERAL(8, 147, 5), // "frame"
QT_MOC_LITERAL(9, 153, 14), // "runtimeChanged"
QT_MOC_LITERAL(10, 168, 23), // "inner_startangleChanged"
QT_MOC_LITERAL(11, 192, 21), // "inner_endangleChanged"
QT_MOC_LITERAL(12, 214, 24), // "middle_startangleChanged"
QT_MOC_LITERAL(13, 239, 22), // "middle_endangleChanged"
QT_MOC_LITERAL(14, 262, 24), // "outter_startangleChanged"
QT_MOC_LITERAL(15, 287, 22), // "outter_endangleChanged"
QT_MOC_LITERAL(16, 310, 12), // "indexChanged"
QT_MOC_LITERAL(17, 323, 15), // "recvinner_angle"
QT_MOC_LITERAL(18, 339, 5), // "angle"
QT_MOC_LITERAL(19, 345, 16), // "recvmiddle_angle"
QT_MOC_LITERAL(20, 362, 16), // "recvoutter_angle"
QT_MOC_LITERAL(21, 379, 10), // "buildFrame"
QT_MOC_LITERAL(22, 390, 7), // "runtime"
QT_MOC_LITERAL(23, 398, 16), // "inner_startangle"
QT_MOC_LITERAL(24, 415, 14), // "inner_endangle"
QT_MOC_LITERAL(25, 430, 17), // "middle_startangle"
QT_MOC_LITERAL(26, 448, 15), // "middle_endangle"
QT_MOC_LITERAL(27, 464, 17), // "outter_startangle"
QT_MOC_LITERAL(28, 482, 15), // "outter_endangle"
QT_MOC_LITERAL(29, 498, 5) // "index"

    },
    "TurntableSendData\0requestOpenTurntable\0"
    "\0requestCloseTurntable\0requestResetTurntable\0"
    "requestZeroTurntable\0requestSendProgramMode\0"
    "programSend_frame&\0frame\0runtimeChanged\0"
    "inner_startangleChanged\0inner_endangleChanged\0"
    "middle_startangleChanged\0"
    "middle_endangleChanged\0outter_startangleChanged\0"
    "outter_endangleChanged\0indexChanged\0"
    "recvinner_angle\0angle\0recvmiddle_angle\0"
    "recvoutter_angle\0buildFrame\0runtime\0"
    "inner_startangle\0inner_endangle\0"
    "middle_startangle\0middle_endangle\0"
    "outter_startangle\0outter_endangle\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TurntableSendData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       8,  124, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    1,  103,    2, 0x06 /* Public */,
       9,    0,  106,    2, 0x06 /* Public */,
      10,    0,  107,    2, 0x06 /* Public */,
      11,    0,  108,    2, 0x06 /* Public */,
      12,    0,  109,    2, 0x06 /* Public */,
      13,    0,  110,    2, 0x06 /* Public */,
      14,    0,  111,    2, 0x06 /* Public */,
      15,    0,  112,    2, 0x06 /* Public */,
      16,    0,  113,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  114,    2, 0x0a /* Public */,
      19,    1,  117,    2, 0x0a /* Public */,
      20,    1,  120,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      21,    0,  123,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::Float,   18,

 // methods: parameters
    QMetaType::Void,

 // properties: name, type, flags
      22, QMetaType::Int, 0x00495001,
      23, QMetaType::Float, 0x00495001,
      24, QMetaType::Float, 0x00495001,
      25, QMetaType::Float, 0x00495001,
      26, QMetaType::Float, 0x00495001,
      27, QMetaType::Float, 0x00495001,
      28, QMetaType::Float, 0x00495001,
      29, QMetaType::Int, 0x00495001,

 // properties: notify_signal_id
       5,
       6,
       7,
       8,
       9,
      10,
      11,
      12,

       0        // eod
};

void TurntableSendData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TurntableSendData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestOpenTurntable(); break;
        case 1: _t->requestCloseTurntable(); break;
        case 2: _t->requestResetTurntable(); break;
        case 3: _t->requestZeroTurntable(); break;
        case 4: _t->requestSendProgramMode((*reinterpret_cast< programSend_frame(*)>(_a[1]))); break;
        case 5: _t->runtimeChanged(); break;
        case 6: _t->inner_startangleChanged(); break;
        case 7: _t->inner_endangleChanged(); break;
        case 8: _t->middle_startangleChanged(); break;
        case 9: _t->middle_endangleChanged(); break;
        case 10: _t->outter_startangleChanged(); break;
        case 11: _t->outter_endangleChanged(); break;
        case 12: _t->indexChanged(); break;
        case 13: _t->recvinner_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: _t->recvmiddle_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: _t->recvoutter_angle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 16: _t->buildFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestOpenTurntable)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestCloseTurntable)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestResetTurntable)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestZeroTurntable)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)(programSend_frame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::requestSendProgramMode)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::runtimeChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::inner_startangleChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::inner_endangleChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::middle_startangleChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::middle_endangleChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::outter_startangleChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::outter_endangleChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TurntableSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TurntableSendData::indexChanged)) {
                *result = 12;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TurntableSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->runtime(); break;
        case 1: *reinterpret_cast< float*>(_v) = _t->inner_startangle(); break;
        case 2: *reinterpret_cast< float*>(_v) = _t->inner_endangle(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->middle_startangle(); break;
        case 4: *reinterpret_cast< float*>(_v) = _t->middle_endangle(); break;
        case 5: *reinterpret_cast< float*>(_v) = _t->outter_startangle(); break;
        case 6: *reinterpret_cast< float*>(_v) = _t->outter_endangle(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->index(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TurntableSendData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TurntableSendData.data,
    qt_meta_data_TurntableSendData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TurntableSendData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TurntableSendData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TurntableSendData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TurntableSendData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TurntableSendData::requestOpenTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TurntableSendData::requestCloseTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TurntableSendData::requestResetTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TurntableSendData::requestZeroTurntable()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TurntableSendData::requestSendProgramMode(programSend_frame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TurntableSendData::runtimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TurntableSendData::inner_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TurntableSendData::inner_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TurntableSendData::middle_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TurntableSendData::middle_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TurntableSendData::outter_startangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TurntableSendData::outter_endangleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TurntableSendData::indexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
struct qt_meta_stringdata_SerialPortTurntable_t {
    QByteArrayData data[13];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortTurntable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortTurntable_t qt_meta_stringdata_SerialPortTurntable = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SerialPortTurntable"
QT_MOC_LITERAL(1, 20, 14), // "requpdateframe"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "StatusFeedback"
QT_MOC_LITERAL(4, 51, 8), // "recvdata"
QT_MOC_LITERAL(5, 60, 13), // "openTurntable"
QT_MOC_LITERAL(6, 74, 14), // "closeTurntable"
QT_MOC_LITERAL(7, 89, 14), // "resetTurntable"
QT_MOC_LITERAL(8, 104, 13), // "zeroTurntable"
QT_MOC_LITERAL(9, 118, 15), // "sendProgramMode"
QT_MOC_LITERAL(10, 134, 18), // "programSend_frame&"
QT_MOC_LITERAL(11, 153, 5), // "frame"
QT_MOC_LITERAL(12, 159, 6) // "dowork"

    },
    "SerialPortTurntable\0requpdateframe\0\0"
    "StatusFeedback\0recvdata\0openTurntable\0"
    "closeTurntable\0resetTurntable\0"
    "zeroTurntable\0sendProgramMode\0"
    "programSend_frame&\0frame\0dowork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortTurntable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,
       9,    1,   56,    2, 0x0a /* Public */,
      12,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void SerialPortTurntable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPortTurntable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requpdateframe((*reinterpret_cast< StatusFeedback(*)>(_a[1]))); break;
        case 1: _t->openTurntable(); break;
        case 2: _t->closeTurntable(); break;
        case 3: _t->resetTurntable(); break;
        case 4: _t->zeroTurntable(); break;
        case 5: _t->sendProgramMode((*reinterpret_cast< programSend_frame(*)>(_a[1]))); break;
        case 6: _t->dowork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPortTurntable::*)(StatusFeedback );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPortTurntable::requpdateframe)) {
                *result = 0;
                return;
            }
        }
    }
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
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SerialPortTurntable::requpdateframe(StatusFeedback _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
