/****************************************************************************
** Meta object code from reading C++ file 'serialport_laser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../serialport/serialport_laser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport_laser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LaserData_t {
    QByteArrayData data[54];
    char stringdata0[921];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LaserData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LaserData_t qt_meta_stringdata_LaserData = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LaserData"
QT_MOC_LITERAL(1, 10, 18), // "frameStatusChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "frameIdChanged"
QT_MOC_LITERAL(4, 45, 16), // "dytStatusChanged"
QT_MOC_LITERAL(5, 62, 21), // "detectorStatusChanged"
QT_MOC_LITERAL(6, 84, 22), // "detectorStatus1Changed"
QT_MOC_LITERAL(7, 107, 22), // "detectorStatus2Changed"
QT_MOC_LITERAL(8, 130, 22), // "detectorStatus3Changed"
QT_MOC_LITERAL(9, 153, 16), // "faultInfoChanged"
QT_MOC_LITERAL(10, 170, 17), // "faultInfo1Changed"
QT_MOC_LITERAL(11, 188, 17), // "faultInfo2Changed"
QT_MOC_LITERAL(12, 206, 21), // "opticalAzimuthChanged"
QT_MOC_LITERAL(13, 228, 19), // "opticalPitchChanged"
QT_MOC_LITERAL(14, 248, 22), // "gyroAzimuthRateChanged"
QT_MOC_LITERAL(15, 271, 20), // "gyroPitchRateChanged"
QT_MOC_LITERAL(16, 292, 21), // "losAzimuthRateChanged"
QT_MOC_LITERAL(17, 314, 19), // "losPitchRateChanged"
QT_MOC_LITERAL(18, 334, 23), // "deviationAzimuthChanged"
QT_MOC_LITERAL(19, 358, 21), // "deviationPitchChanged"
QT_MOC_LITERAL(20, 380, 18), // "laserPeriodChanged"
QT_MOC_LITERAL(21, 399, 17), // "gainStatusChanged"
QT_MOC_LITERAL(22, 417, 22), // "quadrant1EnergyChanged"
QT_MOC_LITERAL(23, 440, 22), // "quadrant2EnergyChanged"
QT_MOC_LITERAL(24, 463, 22), // "quadrant3EnergyChanged"
QT_MOC_LITERAL(25, 486, 22), // "quadrant4EnergyChanged"
QT_MOC_LITERAL(26, 509, 23), // "softwareVersion1Changed"
QT_MOC_LITERAL(27, 533, 23), // "softwareVersion2Changed"
QT_MOC_LITERAL(28, 557, 11), // "frameStatus"
QT_MOC_LITERAL(29, 569, 7), // "frameId"
QT_MOC_LITERAL(30, 577, 9), // "dytStatus"
QT_MOC_LITERAL(31, 587, 14), // "detectorStatus"
QT_MOC_LITERAL(32, 602, 15), // "detectorStatus1"
QT_MOC_LITERAL(33, 618, 15), // "detectorStatus2"
QT_MOC_LITERAL(34, 634, 15), // "detectorStatus3"
QT_MOC_LITERAL(35, 650, 9), // "faultInfo"
QT_MOC_LITERAL(36, 660, 10), // "faultInfo1"
QT_MOC_LITERAL(37, 671, 10), // "faultInfo2"
QT_MOC_LITERAL(38, 682, 14), // "opticalAzimuth"
QT_MOC_LITERAL(39, 697, 12), // "opticalPitch"
QT_MOC_LITERAL(40, 710, 15), // "gyroAzimuthRate"
QT_MOC_LITERAL(41, 726, 13), // "gyroPitchRate"
QT_MOC_LITERAL(42, 740, 14), // "losAzimuthRate"
QT_MOC_LITERAL(43, 755, 12), // "losPitchRate"
QT_MOC_LITERAL(44, 768, 16), // "deviationAzimuth"
QT_MOC_LITERAL(45, 785, 14), // "deviationPitch"
QT_MOC_LITERAL(46, 800, 11), // "laserPeriod"
QT_MOC_LITERAL(47, 812, 10), // "gainStatus"
QT_MOC_LITERAL(48, 823, 15), // "quadrant1Energy"
QT_MOC_LITERAL(49, 839, 15), // "quadrant2Energy"
QT_MOC_LITERAL(50, 855, 15), // "quadrant3Energy"
QT_MOC_LITERAL(51, 871, 15), // "quadrant4Energy"
QT_MOC_LITERAL(52, 887, 16), // "softwareVersion1"
QT_MOC_LITERAL(53, 904, 16) // "softwareVersion2"

    },
    "LaserData\0frameStatusChanged\0\0"
    "frameIdChanged\0dytStatusChanged\0"
    "detectorStatusChanged\0detectorStatus1Changed\0"
    "detectorStatus2Changed\0detectorStatus3Changed\0"
    "faultInfoChanged\0faultInfo1Changed\0"
    "faultInfo2Changed\0opticalAzimuthChanged\0"
    "opticalPitchChanged\0gyroAzimuthRateChanged\0"
    "gyroPitchRateChanged\0losAzimuthRateChanged\0"
    "losPitchRateChanged\0deviationAzimuthChanged\0"
    "deviationPitchChanged\0laserPeriodChanged\0"
    "gainStatusChanged\0quadrant1EnergyChanged\0"
    "quadrant2EnergyChanged\0quadrant3EnergyChanged\0"
    "quadrant4EnergyChanged\0softwareVersion1Changed\0"
    "softwareVersion2Changed\0frameStatus\0"
    "frameId\0dytStatus\0detectorStatus\0"
    "detectorStatus1\0detectorStatus2\0"
    "detectorStatus3\0faultInfo\0faultInfo1\0"
    "faultInfo2\0opticalAzimuth\0opticalPitch\0"
    "gyroAzimuthRate\0gyroPitchRate\0"
    "losAzimuthRate\0losPitchRate\0"
    "deviationAzimuth\0deviationPitch\0"
    "laserPeriod\0gainStatus\0quadrant1Energy\0"
    "quadrant2Energy\0quadrant3Energy\0"
    "quadrant4Energy\0softwareVersion1\0"
    "softwareVersion2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LaserData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
      26,  170, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      26,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  144,    2, 0x06 /* Public */,
       3,    0,  145,    2, 0x06 /* Public */,
       4,    0,  146,    2, 0x06 /* Public */,
       5,    0,  147,    2, 0x06 /* Public */,
       6,    0,  148,    2, 0x06 /* Public */,
       7,    0,  149,    2, 0x06 /* Public */,
       8,    0,  150,    2, 0x06 /* Public */,
       9,    0,  151,    2, 0x06 /* Public */,
      10,    0,  152,    2, 0x06 /* Public */,
      11,    0,  153,    2, 0x06 /* Public */,
      12,    0,  154,    2, 0x06 /* Public */,
      13,    0,  155,    2, 0x06 /* Public */,
      14,    0,  156,    2, 0x06 /* Public */,
      15,    0,  157,    2, 0x06 /* Public */,
      16,    0,  158,    2, 0x06 /* Public */,
      17,    0,  159,    2, 0x06 /* Public */,
      18,    0,  160,    2, 0x06 /* Public */,
      19,    0,  161,    2, 0x06 /* Public */,
      20,    0,  162,    2, 0x06 /* Public */,
      21,    0,  163,    2, 0x06 /* Public */,
      22,    0,  164,    2, 0x06 /* Public */,
      23,    0,  165,    2, 0x06 /* Public */,
      24,    0,  166,    2, 0x06 /* Public */,
      25,    0,  167,    2, 0x06 /* Public */,
      26,    0,  168,    2, 0x06 /* Public */,
      27,    0,  169,    2, 0x06 /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      28, QMetaType::Int, 0x00495001,
      29, QMetaType::Int, 0x00495001,
      30, QMetaType::Int, 0x00495001,
      31, QMetaType::Int, 0x00495001,
      32, QMetaType::Int, 0x00495001,
      33, QMetaType::Int, 0x00495001,
      34, QMetaType::Int, 0x00495001,
      35, QMetaType::Int, 0x00495001,
      36, QMetaType::Int, 0x00495001,
      37, QMetaType::Int, 0x00495001,
      38, QMetaType::Float, 0x00495001,
      39, QMetaType::Float, 0x00495001,
      40, QMetaType::Float, 0x00495001,
      41, QMetaType::Float, 0x00495001,
      42, QMetaType::Float, 0x00495001,
      43, QMetaType::Float, 0x00495001,
      44, QMetaType::Float, 0x00495001,
      45, QMetaType::Float, 0x00495001,
      46, QMetaType::Int, 0x00495001,
      47, QMetaType::Int, 0x00495001,
      48, QMetaType::Float, 0x00495001,
      49, QMetaType::Float, 0x00495001,
      50, QMetaType::Float, 0x00495001,
      51, QMetaType::Float, 0x00495001,
      52, QMetaType::Float, 0x00495001,
      53, QMetaType::Float, 0x00495001,

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
      11,
      12,
      13,
      14,
      15,
      16,
      17,
      18,
      19,
      20,
      21,
      22,
      23,
      24,
      25,

       0        // eod
};

void LaserData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LaserData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameStatusChanged(); break;
        case 1: _t->frameIdChanged(); break;
        case 2: _t->dytStatusChanged(); break;
        case 3: _t->detectorStatusChanged(); break;
        case 4: _t->detectorStatus1Changed(); break;
        case 5: _t->detectorStatus2Changed(); break;
        case 6: _t->detectorStatus3Changed(); break;
        case 7: _t->faultInfoChanged(); break;
        case 8: _t->faultInfo1Changed(); break;
        case 9: _t->faultInfo2Changed(); break;
        case 10: _t->opticalAzimuthChanged(); break;
        case 11: _t->opticalPitchChanged(); break;
        case 12: _t->gyroAzimuthRateChanged(); break;
        case 13: _t->gyroPitchRateChanged(); break;
        case 14: _t->losAzimuthRateChanged(); break;
        case 15: _t->losPitchRateChanged(); break;
        case 16: _t->deviationAzimuthChanged(); break;
        case 17: _t->deviationPitchChanged(); break;
        case 18: _t->laserPeriodChanged(); break;
        case 19: _t->gainStatusChanged(); break;
        case 20: _t->quadrant1EnergyChanged(); break;
        case 21: _t->quadrant2EnergyChanged(); break;
        case 22: _t->quadrant3EnergyChanged(); break;
        case 23: _t->quadrant4EnergyChanged(); break;
        case 24: _t->softwareVersion1Changed(); break;
        case 25: _t->softwareVersion2Changed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::frameStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::frameIdChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::dytStatusChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::detectorStatusChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::detectorStatus1Changed)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::detectorStatus2Changed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::detectorStatus3Changed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::faultInfoChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::faultInfo1Changed)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::faultInfo2Changed)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::opticalAzimuthChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::opticalPitchChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::gyroAzimuthRateChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::gyroPitchRateChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::losAzimuthRateChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::losPitchRateChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::deviationAzimuthChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::deviationPitchChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::laserPeriodChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::gainStatusChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::quadrant1EnergyChanged)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::quadrant2EnergyChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::quadrant3EnergyChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::quadrant4EnergyChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::softwareVersion1Changed)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (LaserData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserData::softwareVersion2Changed)) {
                *result = 25;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<LaserData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->frameStatus(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->frameId(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->dytStatus(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->detectorStatus(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->detectorStatus1(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->detectorStatus2(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->detectorStatus3(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->faultInfo(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->faultInfo1(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->faultInfo2(); break;
        case 10: *reinterpret_cast< float*>(_v) = _t->opticalAzimuth(); break;
        case 11: *reinterpret_cast< float*>(_v) = _t->opticalPitch(); break;
        case 12: *reinterpret_cast< float*>(_v) = _t->gyroAzimuthRate(); break;
        case 13: *reinterpret_cast< float*>(_v) = _t->gyroPitchRate(); break;
        case 14: *reinterpret_cast< float*>(_v) = _t->losAzimuthRate(); break;
        case 15: *reinterpret_cast< float*>(_v) = _t->losPitchRate(); break;
        case 16: *reinterpret_cast< float*>(_v) = _t->deviationAzimuth(); break;
        case 17: *reinterpret_cast< float*>(_v) = _t->deviationPitch(); break;
        case 18: *reinterpret_cast< int*>(_v) = _t->laserPeriod(); break;
        case 19: *reinterpret_cast< int*>(_v) = _t->gainStatus(); break;
        case 20: *reinterpret_cast< float*>(_v) = _t->quadrant1Energy(); break;
        case 21: *reinterpret_cast< float*>(_v) = _t->quadrant2Energy(); break;
        case 22: *reinterpret_cast< float*>(_v) = _t->quadrant3Energy(); break;
        case 23: *reinterpret_cast< float*>(_v) = _t->quadrant4Energy(); break;
        case 24: *reinterpret_cast< float*>(_v) = _t->softwareVersion1(); break;
        case 25: *reinterpret_cast< float*>(_v) = _t->softwareVersion2(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LaserData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_LaserData.data,
    qt_meta_data_LaserData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LaserData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LaserData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LaserData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LaserData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 26;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LaserData::frameStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LaserData::frameIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LaserData::dytStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LaserData::detectorStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void LaserData::detectorStatus1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void LaserData::detectorStatus2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void LaserData::detectorStatus3Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void LaserData::faultInfoChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void LaserData::faultInfo1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void LaserData::faultInfo2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void LaserData::opticalAzimuthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void LaserData::opticalPitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void LaserData::gyroAzimuthRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void LaserData::gyroPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void LaserData::losAzimuthRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void LaserData::losPitchRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void LaserData::deviationAzimuthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void LaserData::deviationPitchChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void LaserData::laserPeriodChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void LaserData::gainStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void LaserData::quadrant1EnergyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void LaserData::quadrant2EnergyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void LaserData::quadrant3EnergyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void LaserData::quadrant4EnergyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void LaserData::softwareVersion1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void LaserData::softwareVersion2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}
struct qt_meta_stringdata_LaserSendData_t {
    QByteArrayData data[25];
    char stringdata0[443];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LaserSendData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LaserSendData_t qt_meta_stringdata_LaserSendData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LaserSendData"
QT_MOC_LITERAL(1, 14, 18), // "frameStatusChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 14), // "frameIdChanged"
QT_MOC_LITERAL(4, 49, 10), // "cmdChanged"
QT_MOC_LITERAL(5, 60, 18), // "laserPeriodChanged"
QT_MOC_LITERAL(6, 79, 19), // "azimuthAngleChanged"
QT_MOC_LITERAL(7, 99, 21), // "elevationAngleChanged"
QT_MOC_LITERAL(8, 121, 26), // "searchCenterAzimuthChanged"
QT_MOC_LITERAL(9, 148, 28), // "searchCenterElevationChanged"
QT_MOC_LITERAL(10, 177, 25), // "azimuthSearchRangeChanged"
QT_MOC_LITERAL(11, 203, 27), // "elevationSearchRangeChanged"
QT_MOC_LITERAL(12, 231, 19), // "searchRadiusChanged"
QT_MOC_LITERAL(13, 251, 10), // "buildFrame"
QT_MOC_LITERAL(14, 262, 13), // "m_frameStatus"
QT_MOC_LITERAL(15, 276, 9), // "m_frameId"
QT_MOC_LITERAL(16, 286, 5), // "m_cmd"
QT_MOC_LITERAL(17, 292, 13), // "m_laserPeriod"
QT_MOC_LITERAL(18, 306, 14), // "m_azimuthAngle"
QT_MOC_LITERAL(19, 321, 16), // "m_elevationAngle"
QT_MOC_LITERAL(20, 338, 21), // "m_searchCenterAzimuth"
QT_MOC_LITERAL(21, 360, 23), // "m_searchCenterElevation"
QT_MOC_LITERAL(22, 384, 20), // "m_azimuthSearchRange"
QT_MOC_LITERAL(23, 405, 22), // "m_elevationSearchRange"
QT_MOC_LITERAL(24, 428, 14) // "m_searchRadius"

    },
    "LaserSendData\0frameStatusChanged\0\0"
    "frameIdChanged\0cmdChanged\0laserPeriodChanged\0"
    "azimuthAngleChanged\0elevationAngleChanged\0"
    "searchCenterAzimuthChanged\0"
    "searchCenterElevationChanged\0"
    "azimuthSearchRangeChanged\0"
    "elevationSearchRangeChanged\0"
    "searchRadiusChanged\0buildFrame\0"
    "m_frameStatus\0m_frameId\0m_cmd\0"
    "m_laserPeriod\0m_azimuthAngle\0"
    "m_elevationAngle\0m_searchCenterAzimuth\0"
    "m_searchCenterElevation\0m_azimuthSearchRange\0"
    "m_elevationSearchRange\0m_searchRadius"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LaserSendData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
      11,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    0,   78,    2, 0x06 /* Public */,
       7,    0,   79,    2, 0x06 /* Public */,
       8,    0,   80,    2, 0x06 /* Public */,
       9,    0,   81,    2, 0x06 /* Public */,
      10,    0,   82,    2, 0x06 /* Public */,
      11,    0,   83,    2, 0x06 /* Public */,
      12,    0,   84,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      13,    0,   85,    2, 0x02 /* Public */,

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

 // methods: parameters
    QMetaType::QByteArray,

 // properties: name, type, flags
      14, QMetaType::Int, 0x00495003,
      15, QMetaType::Int, 0x00495003,
      16, QMetaType::Int, 0x00495003,
      17, QMetaType::Int, 0x00495003,
      18, QMetaType::Float, 0x00495003,
      19, QMetaType::Float, 0x00495003,
      20, QMetaType::Float, 0x00495003,
      21, QMetaType::Float, 0x00495003,
      22, QMetaType::Float, 0x00495003,
      23, QMetaType::Float, 0x00495003,
      24, QMetaType::Float, 0x00495003,

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

void LaserSendData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LaserSendData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameStatusChanged(); break;
        case 1: _t->frameIdChanged(); break;
        case 2: _t->cmdChanged(); break;
        case 3: _t->laserPeriodChanged(); break;
        case 4: _t->azimuthAngleChanged(); break;
        case 5: _t->elevationAngleChanged(); break;
        case 6: _t->searchCenterAzimuthChanged(); break;
        case 7: _t->searchCenterElevationChanged(); break;
        case 8: _t->azimuthSearchRangeChanged(); break;
        case 9: _t->elevationSearchRangeChanged(); break;
        case 10: _t->searchRadiusChanged(); break;
        case 11: { QByteArray _r = _t->buildFrame();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::frameStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::frameIdChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::cmdChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::laserPeriodChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::azimuthAngleChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::elevationAngleChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::searchCenterAzimuthChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::searchCenterElevationChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::azimuthSearchRangeChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::elevationSearchRangeChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (LaserSendData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LaserSendData::searchRadiusChanged)) {
                *result = 10;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<LaserSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->m_frameStatus; break;
        case 1: *reinterpret_cast< int*>(_v) = _t->m_frameId; break;
        case 2: *reinterpret_cast< int*>(_v) = _t->m_cmd; break;
        case 3: *reinterpret_cast< int*>(_v) = _t->m_laserPeriod; break;
        case 4: *reinterpret_cast< float*>(_v) = _t->m_azimuthAngle; break;
        case 5: *reinterpret_cast< float*>(_v) = _t->m_elevationAngle; break;
        case 6: *reinterpret_cast< float*>(_v) = _t->m_searchCenterAzimuth; break;
        case 7: *reinterpret_cast< float*>(_v) = _t->m_searchCenterElevation; break;
        case 8: *reinterpret_cast< float*>(_v) = _t->m_azimuthSearchRange; break;
        case 9: *reinterpret_cast< float*>(_v) = _t->m_elevationSearchRange; break;
        case 10: *reinterpret_cast< float*>(_v) = _t->m_searchRadius; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<LaserSendData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_frameStatus != *reinterpret_cast< int*>(_v)) {
                _t->m_frameStatus = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->frameStatusChanged();
            }
            break;
        case 1:
            if (_t->m_frameId != *reinterpret_cast< int*>(_v)) {
                _t->m_frameId = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->frameIdChanged();
            }
            break;
        case 2:
            if (_t->m_cmd != *reinterpret_cast< int*>(_v)) {
                _t->m_cmd = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->cmdChanged();
            }
            break;
        case 3:
            if (_t->m_laserPeriod != *reinterpret_cast< int*>(_v)) {
                _t->m_laserPeriod = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->laserPeriodChanged();
            }
            break;
        case 4:
            if (_t->m_azimuthAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_azimuthAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->azimuthAngleChanged();
            }
            break;
        case 5:
            if (_t->m_elevationAngle != *reinterpret_cast< float*>(_v)) {
                _t->m_elevationAngle = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->elevationAngleChanged();
            }
            break;
        case 6:
            if (_t->m_searchCenterAzimuth != *reinterpret_cast< float*>(_v)) {
                _t->m_searchCenterAzimuth = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchCenterAzimuthChanged();
            }
            break;
        case 7:
            if (_t->m_searchCenterElevation != *reinterpret_cast< float*>(_v)) {
                _t->m_searchCenterElevation = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchCenterElevationChanged();
            }
            break;
        case 8:
            if (_t->m_azimuthSearchRange != *reinterpret_cast< float*>(_v)) {
                _t->m_azimuthSearchRange = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->azimuthSearchRangeChanged();
            }
            break;
        case 9:
            if (_t->m_elevationSearchRange != *reinterpret_cast< float*>(_v)) {
                _t->m_elevationSearchRange = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->elevationSearchRangeChanged();
            }
            break;
        case 10:
            if (_t->m_searchRadius != *reinterpret_cast< float*>(_v)) {
                _t->m_searchRadius = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->searchRadiusChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject LaserSendData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_LaserSendData.data,
    qt_meta_data_LaserSendData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LaserSendData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LaserSendData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LaserSendData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LaserSendData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void LaserSendData::frameStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LaserSendData::frameIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LaserSendData::cmdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LaserSendData::laserPeriodChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void LaserSendData::azimuthAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void LaserSendData::elevationAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void LaserSendData::searchCenterAzimuthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void LaserSendData::searchCenterElevationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void LaserSendData::azimuthSearchRangeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void LaserSendData::elevationSearchRangeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void LaserSendData::searchRadiusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
struct qt_meta_stringdata_SerialPortLaser_t {
    QByteArrayData data[5];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPortLaser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPortLaser_t qt_meta_stringdata_SerialPortLaser = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SerialPortLaser"
QT_MOC_LITERAL(1, 16, 9), // "laserData"
QT_MOC_LITERAL(2, 26, 10), // "LaserData*"
QT_MOC_LITERAL(3, 37, 13), // "laserSendData"
QT_MOC_LITERAL(4, 51, 14) // "LaserSendData*"

    },
    "SerialPortLaser\0laserData\0LaserData*\0"
    "laserSendData\0LaserSendData*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPortLaser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, 0x80000000 | 2, 0x00095409,
       3, 0x80000000 | 4, 0x00095409,

       0        // eod
};

void SerialPortLaser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LaserData* >(); break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LaserSendData* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SerialPortLaser *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< LaserData**>(_v) = _t->laserData(); break;
        case 1: *reinterpret_cast< LaserSendData**>(_v) = _t->laserSendData(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
}

QT_INIT_METAOBJECT const QMetaObject SerialPortLaser::staticMetaObject = { {
    &SerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPortLaser.data,
    qt_meta_data_SerialPortLaser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPortLaser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPortLaser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPortLaser.stringdata0))
        return static_cast<void*>(this);
    return SerialPort::qt_metacast(_clname);
}

int SerialPortLaser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
