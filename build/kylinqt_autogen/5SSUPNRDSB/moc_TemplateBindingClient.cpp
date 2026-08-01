/****************************************************************************
** Meta object code from reading C++ file 'TemplateBindingClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../network/TemplateBindingClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TemplateBindingClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TemplateBindingData_t {
    QByteArrayData data[81];
    char stringdata0[1090];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TemplateBindingData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TemplateBindingData_t qt_meta_stringdata_TemplateBindingData = {
    {
QT_MOC_LITERAL(0, 0, 19), // "TemplateBindingData"
QT_MOC_LITERAL(1, 20, 11), // "hostChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "portChanged"
QT_MOC_LITERAL(4, 45, 16), // "connectedChanged"
QT_MOC_LITERAL(5, 62, 24), // "currentImageIndexChanged"
QT_MOC_LITERAL(6, 87, 17), // "imageCountChanged"
QT_MOC_LITERAL(7, 105, 23), // "currentImagePathChanged"
QT_MOC_LITERAL(8, 129, 18), // "isSatelliteChanged"
QT_MOC_LITERAL(9, 148, 17), // "templateIdChanged"
QT_MOC_LITERAL(10, 166, 14), // "imageIdChanged"
QT_MOC_LITERAL(11, 181, 17), // "imageWidthChanged"
QT_MOC_LITERAL(12, 199, 18), // "imageHeightChanged"
QT_MOC_LITERAL(13, 218, 17), // "targetPosXChanged"
QT_MOC_LITERAL(14, 236, 17), // "targetPosYChanged"
QT_MOC_LITERAL(15, 254, 20), // "targetPixelsXChanged"
QT_MOC_LITERAL(16, 275, 20), // "targetPixelsYChanged"
QT_MOC_LITERAL(17, 296, 12), // "scaleChanged"
QT_MOC_LITERAL(18, 309, 19), // "templateModeChanged"
QT_MOC_LITERAL(19, 329, 15), // "distanceChanged"
QT_MOC_LITERAL(20, 345, 14), // "azimuthChanged"
QT_MOC_LITERAL(21, 360, 17), // "pitchAngleChanged"
QT_MOC_LITERAL(22, 378, 18), // "focalLengthChanged"
QT_MOC_LITERAL(23, 397, 16), // "pixelSizeChanged"
QT_MOC_LITERAL(24, 414, 18), // "txtFileNameChanged"
QT_MOC_LITERAL(25, 433, 19), // "txtGeneratedChanged"
QT_MOC_LITERAL(26, 453, 19), // "sendProgressChanged"
QT_MOC_LITERAL(27, 473, 16), // "imageSentChanged"
QT_MOC_LITERAL(28, 490, 14), // "txtSentChanged"
QT_MOC_LITERAL(29, 505, 20), // "statusMessageChanged"
QT_MOC_LITERAL(30, 526, 14), // "requestConnect"
QT_MOC_LITERAL(31, 541, 4), // "host"
QT_MOC_LITERAL(32, 546, 4), // "port"
QT_MOC_LITERAL(33, 551, 17), // "requestDisconnect"
QT_MOC_LITERAL(34, 569, 17), // "requestSendImages"
QT_MOC_LITERAL(35, 587, 14), // "requestSendTxt"
QT_MOC_LITERAL(36, 602, 7), // "txtData"
QT_MOC_LITERAL(37, 610, 12), // "setConnected"
QT_MOC_LITERAL(38, 623, 4), // "conn"
QT_MOC_LITERAL(39, 628, 15), // "setTxtGenerated"
QT_MOC_LITERAL(40, 644, 3), // "gen"
QT_MOC_LITERAL(41, 648, 15), // "setSendProgress"
QT_MOC_LITERAL(42, 664, 8), // "progress"
QT_MOC_LITERAL(43, 673, 12), // "setImageSent"
QT_MOC_LITERAL(44, 686, 4), // "sent"
QT_MOC_LITERAL(45, 691, 10), // "setTxtSent"
QT_MOC_LITERAL(46, 702, 16), // "setStatusMessage"
QT_MOC_LITERAL(47, 719, 3), // "msg"
QT_MOC_LITERAL(48, 723, 8), // "addImage"
QT_MOC_LITERAL(49, 732, 8), // "filePath"
QT_MOC_LITERAL(50, 741, 11), // "removeImage"
QT_MOC_LITERAL(51, 753, 5), // "index"
QT_MOC_LITERAL(52, 759, 9), // "nextImage"
QT_MOC_LITERAL(53, 769, 9), // "prevImage"
QT_MOC_LITERAL(54, 779, 11), // "generateTxt"
QT_MOC_LITERAL(55, 791, 9), // "connected"
QT_MOC_LITERAL(56, 801, 17), // "currentImageIndex"
QT_MOC_LITERAL(57, 819, 10), // "imageCount"
QT_MOC_LITERAL(58, 830, 16), // "currentImagePath"
QT_MOC_LITERAL(59, 847, 11), // "isSatellite"
QT_MOC_LITERAL(60, 859, 10), // "templateId"
QT_MOC_LITERAL(61, 870, 7), // "imageId"
QT_MOC_LITERAL(62, 878, 10), // "imageWidth"
QT_MOC_LITERAL(63, 889, 11), // "imageHeight"
QT_MOC_LITERAL(64, 901, 10), // "targetPosX"
QT_MOC_LITERAL(65, 912, 10), // "targetPosY"
QT_MOC_LITERAL(66, 923, 13), // "targetPixelsX"
QT_MOC_LITERAL(67, 937, 13), // "targetPixelsY"
QT_MOC_LITERAL(68, 951, 5), // "scale"
QT_MOC_LITERAL(69, 957, 12), // "templateMode"
QT_MOC_LITERAL(70, 970, 8), // "distance"
QT_MOC_LITERAL(71, 979, 7), // "azimuth"
QT_MOC_LITERAL(72, 987, 10), // "pitchAngle"
QT_MOC_LITERAL(73, 998, 11), // "focalLength"
QT_MOC_LITERAL(74, 1010, 9), // "pixelSize"
QT_MOC_LITERAL(75, 1020, 11), // "txtFileName"
QT_MOC_LITERAL(76, 1032, 12), // "txtGenerated"
QT_MOC_LITERAL(77, 1045, 12), // "sendProgress"
QT_MOC_LITERAL(78, 1058, 9), // "imageSent"
QT_MOC_LITERAL(79, 1068, 7), // "txtSent"
QT_MOC_LITERAL(80, 1076, 13) // "statusMessage"

    },
    "TemplateBindingData\0hostChanged\0\0"
    "portChanged\0connectedChanged\0"
    "currentImageIndexChanged\0imageCountChanged\0"
    "currentImagePathChanged\0isSatelliteChanged\0"
    "templateIdChanged\0imageIdChanged\0"
    "imageWidthChanged\0imageHeightChanged\0"
    "targetPosXChanged\0targetPosYChanged\0"
    "targetPixelsXChanged\0targetPixelsYChanged\0"
    "scaleChanged\0templateModeChanged\0"
    "distanceChanged\0azimuthChanged\0"
    "pitchAngleChanged\0focalLengthChanged\0"
    "pixelSizeChanged\0txtFileNameChanged\0"
    "txtGeneratedChanged\0sendProgressChanged\0"
    "imageSentChanged\0txtSentChanged\0"
    "statusMessageChanged\0requestConnect\0"
    "host\0port\0requestDisconnect\0"
    "requestSendImages\0requestSendTxt\0"
    "txtData\0setConnected\0conn\0setTxtGenerated\0"
    "gen\0setSendProgress\0progress\0setImageSent\0"
    "sent\0setTxtSent\0setStatusMessage\0msg\0"
    "addImage\0filePath\0removeImage\0index\0"
    "nextImage\0prevImage\0generateTxt\0"
    "connected\0currentImageIndex\0imageCount\0"
    "currentImagePath\0isSatellite\0templateId\0"
    "imageId\0imageWidth\0imageHeight\0"
    "targetPosX\0targetPosY\0targetPixelsX\0"
    "targetPixelsY\0scale\0templateMode\0"
    "distance\0azimuth\0pitchAngle\0focalLength\0"
    "pixelSize\0txtFileName\0txtGenerated\0"
    "sendProgress\0imageSent\0txtSent\0"
    "statusMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TemplateBindingData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      43,   14, // methods
      28,  294, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      32,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  229,    2, 0x06 /* Public */,
       3,    0,  230,    2, 0x06 /* Public */,
       4,    0,  231,    2, 0x06 /* Public */,
       5,    0,  232,    2, 0x06 /* Public */,
       6,    0,  233,    2, 0x06 /* Public */,
       7,    0,  234,    2, 0x06 /* Public */,
       8,    0,  235,    2, 0x06 /* Public */,
       9,    0,  236,    2, 0x06 /* Public */,
      10,    0,  237,    2, 0x06 /* Public */,
      11,    0,  238,    2, 0x06 /* Public */,
      12,    0,  239,    2, 0x06 /* Public */,
      13,    0,  240,    2, 0x06 /* Public */,
      14,    0,  241,    2, 0x06 /* Public */,
      15,    0,  242,    2, 0x06 /* Public */,
      16,    0,  243,    2, 0x06 /* Public */,
      17,    0,  244,    2, 0x06 /* Public */,
      18,    0,  245,    2, 0x06 /* Public */,
      19,    0,  246,    2, 0x06 /* Public */,
      20,    0,  247,    2, 0x06 /* Public */,
      21,    0,  248,    2, 0x06 /* Public */,
      22,    0,  249,    2, 0x06 /* Public */,
      23,    0,  250,    2, 0x06 /* Public */,
      24,    0,  251,    2, 0x06 /* Public */,
      25,    0,  252,    2, 0x06 /* Public */,
      26,    0,  253,    2, 0x06 /* Public */,
      27,    0,  254,    2, 0x06 /* Public */,
      28,    0,  255,    2, 0x06 /* Public */,
      29,    0,  256,    2, 0x06 /* Public */,
      30,    2,  257,    2, 0x06 /* Public */,
      33,    0,  262,    2, 0x06 /* Public */,
      34,    0,  263,    2, 0x06 /* Public */,
      35,    1,  264,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      37,    1,  267,    2, 0x0a /* Public */,
      39,    1,  270,    2, 0x0a /* Public */,
      41,    1,  273,    2, 0x0a /* Public */,
      43,    1,  276,    2, 0x0a /* Public */,
      45,    1,  279,    2, 0x0a /* Public */,
      46,    1,  282,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      48,    1,  285,    2, 0x02 /* Public */,
      50,    1,  288,    2, 0x02 /* Public */,
      52,    0,  291,    2, 0x02 /* Public */,
      53,    0,  292,    2, 0x02 /* Public */,
      54,    0,  293,    2, 0x02 /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   36,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   38,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void, QMetaType::Double,   42,
    QMetaType::Void, QMetaType::Bool,   44,
    QMetaType::Void, QMetaType::Bool,   44,
    QMetaType::Void, QMetaType::QString,   47,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   49,
    QMetaType::Void, QMetaType::Int,   51,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      31, QMetaType::QString, 0x00495103,
      32, QMetaType::Int, 0x00495103,
      55, QMetaType::Bool, 0x00495001,
      56, QMetaType::Int, 0x00495103,
      57, QMetaType::Int, 0x00495001,
      58, QMetaType::QString, 0x00495001,
      59, QMetaType::Bool, 0x00495103,
      60, QMetaType::Int, 0x00495103,
      61, QMetaType::Int, 0x00495103,
      62, QMetaType::Int, 0x00495103,
      63, QMetaType::Int, 0x00495103,
      64, QMetaType::Int, 0x00495103,
      65, QMetaType::Int, 0x00495103,
      66, QMetaType::Int, 0x00495103,
      67, QMetaType::Int, 0x00495103,
      68, QMetaType::Double, 0x00495103,
      69, QMetaType::Int, 0x00495103,
      70, QMetaType::Double, 0x00495103,
      71, QMetaType::Double, 0x00495103,
      72, QMetaType::Double, 0x00495103,
      73, QMetaType::Double, 0x00495103,
      74, QMetaType::Double, 0x00495103,
      75, QMetaType::QString, 0x00495001,
      76, QMetaType::Bool, 0x00495001,
      77, QMetaType::Double, 0x00495001,
      78, QMetaType::Bool, 0x00495001,
      79, QMetaType::Bool, 0x00495001,
      80, QMetaType::QString, 0x00495001,

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
      26,
      27,

       0        // eod
};

void TemplateBindingData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TemplateBindingData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hostChanged(); break;
        case 1: _t->portChanged(); break;
        case 2: _t->connectedChanged(); break;
        case 3: _t->currentImageIndexChanged(); break;
        case 4: _t->imageCountChanged(); break;
        case 5: _t->currentImagePathChanged(); break;
        case 6: _t->isSatelliteChanged(); break;
        case 7: _t->templateIdChanged(); break;
        case 8: _t->imageIdChanged(); break;
        case 9: _t->imageWidthChanged(); break;
        case 10: _t->imageHeightChanged(); break;
        case 11: _t->targetPosXChanged(); break;
        case 12: _t->targetPosYChanged(); break;
        case 13: _t->targetPixelsXChanged(); break;
        case 14: _t->targetPixelsYChanged(); break;
        case 15: _t->scaleChanged(); break;
        case 16: _t->templateModeChanged(); break;
        case 17: _t->distanceChanged(); break;
        case 18: _t->azimuthChanged(); break;
        case 19: _t->pitchAngleChanged(); break;
        case 20: _t->focalLengthChanged(); break;
        case 21: _t->pixelSizeChanged(); break;
        case 22: _t->txtFileNameChanged(); break;
        case 23: _t->txtGeneratedChanged(); break;
        case 24: _t->sendProgressChanged(); break;
        case 25: _t->imageSentChanged(); break;
        case 26: _t->txtSentChanged(); break;
        case 27: _t->statusMessageChanged(); break;
        case 28: _t->requestConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 29: _t->requestDisconnect(); break;
        case 30: _t->requestSendImages(); break;
        case 31: _t->requestSendTxt((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 32: _t->setConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->setTxtGenerated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->setSendProgress((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 35: _t->setImageSent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->setTxtSent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->setStatusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 38: _t->addImage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->removeImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->nextImage(); break;
        case 41: _t->prevImage(); break;
        case 42: _t->generateTxt(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::hostChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::portChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::connectedChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::currentImageIndexChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::imageCountChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::currentImagePathChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::isSatelliteChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::templateIdChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::imageIdChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::imageWidthChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::imageHeightChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::targetPosXChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::targetPosYChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::targetPixelsXChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::targetPixelsYChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::scaleChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::templateModeChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::distanceChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::azimuthChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::pitchAngleChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::focalLengthChanged)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::pixelSizeChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::txtFileNameChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::txtGeneratedChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::sendProgressChanged)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::imageSentChanged)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::txtSentChanged)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::statusMessageChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::requestConnect)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::requestDisconnect)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::requestSendImages)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (TemplateBindingData::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateBindingData::requestSendTxt)) {
                *result = 31;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TemplateBindingData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->host(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->port(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->connected(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->currentImageIndex(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->imageCount(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->currentImagePath(); break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->isSatellite(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->templateId(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->imageId(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->imageWidth(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->imageHeight(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->targetPosX(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->targetPosY(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->targetPixelsX(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->targetPixelsY(); break;
        case 15: *reinterpret_cast< double*>(_v) = _t->scale(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->templateMode(); break;
        case 17: *reinterpret_cast< double*>(_v) = _t->distance(); break;
        case 18: *reinterpret_cast< double*>(_v) = _t->azimuth(); break;
        case 19: *reinterpret_cast< double*>(_v) = _t->pitchAngle(); break;
        case 20: *reinterpret_cast< double*>(_v) = _t->focalLength(); break;
        case 21: *reinterpret_cast< double*>(_v) = _t->pixelSize(); break;
        case 22: *reinterpret_cast< QString*>(_v) = _t->txtFileName(); break;
        case 23: *reinterpret_cast< bool*>(_v) = _t->txtGenerated(); break;
        case 24: *reinterpret_cast< double*>(_v) = _t->sendProgress(); break;
        case 25: *reinterpret_cast< bool*>(_v) = _t->imageSent(); break;
        case 26: *reinterpret_cast< bool*>(_v) = _t->txtSent(); break;
        case 27: *reinterpret_cast< QString*>(_v) = _t->statusMessage(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<TemplateBindingData *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHost(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setPort(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setCurrentImageIndex(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setIsSatellite(*reinterpret_cast< bool*>(_v)); break;
        case 7: _t->setTemplateId(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setImageId(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setImageWidth(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setImageHeight(*reinterpret_cast< int*>(_v)); break;
        case 11: _t->setTargetPosX(*reinterpret_cast< int*>(_v)); break;
        case 12: _t->setTargetPosY(*reinterpret_cast< int*>(_v)); break;
        case 13: _t->setTargetPixelsX(*reinterpret_cast< int*>(_v)); break;
        case 14: _t->setTargetPixelsY(*reinterpret_cast< int*>(_v)); break;
        case 15: _t->setScale(*reinterpret_cast< double*>(_v)); break;
        case 16: _t->setTemplateMode(*reinterpret_cast< int*>(_v)); break;
        case 17: _t->setDistance(*reinterpret_cast< double*>(_v)); break;
        case 18: _t->setAzimuth(*reinterpret_cast< double*>(_v)); break;
        case 19: _t->setPitchAngle(*reinterpret_cast< double*>(_v)); break;
        case 20: _t->setFocalLength(*reinterpret_cast< double*>(_v)); break;
        case 21: _t->setPixelSize(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TemplateBindingData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TemplateBindingData.data,
    qt_meta_data_TemplateBindingData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TemplateBindingData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TemplateBindingData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TemplateBindingData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TemplateBindingData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 43)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 43;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 28;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 28;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 28;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 28;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 28;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TemplateBindingData::hostChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TemplateBindingData::portChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TemplateBindingData::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TemplateBindingData::currentImageIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TemplateBindingData::imageCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TemplateBindingData::currentImagePathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TemplateBindingData::isSatelliteChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TemplateBindingData::templateIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TemplateBindingData::imageIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TemplateBindingData::imageWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TemplateBindingData::imageHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TemplateBindingData::targetPosXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TemplateBindingData::targetPosYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void TemplateBindingData::targetPixelsXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void TemplateBindingData::targetPixelsYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void TemplateBindingData::scaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void TemplateBindingData::templateModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void TemplateBindingData::distanceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void TemplateBindingData::azimuthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void TemplateBindingData::pitchAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void TemplateBindingData::focalLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void TemplateBindingData::pixelSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void TemplateBindingData::txtFileNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void TemplateBindingData::txtGeneratedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void TemplateBindingData::sendProgressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void TemplateBindingData::imageSentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void TemplateBindingData::txtSentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void TemplateBindingData::statusMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void TemplateBindingData::requestConnect(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void TemplateBindingData::requestDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void TemplateBindingData::requestSendImages()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void TemplateBindingData::requestSendTxt(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}
struct qt_meta_stringdata_TemplateBindingWorker_t {
    QByteArrayData data[14];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TemplateBindingWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TemplateBindingWorker_t qt_meta_stringdata_TemplateBindingWorker = {
    {
QT_MOC_LITERAL(0, 0, 21), // "TemplateBindingWorker"
QT_MOC_LITERAL(1, 22, 9), // "onConnect"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "host"
QT_MOC_LITERAL(4, 38, 4), // "port"
QT_MOC_LITERAL(5, 43, 12), // "onDisconnect"
QT_MOC_LITERAL(6, 56, 12), // "onSendImages"
QT_MOC_LITERAL(7, 69, 9), // "onSendTxt"
QT_MOC_LITERAL(8, 79, 7), // "txtData"
QT_MOC_LITERAL(9, 87, 17), // "onSocketConnected"
QT_MOC_LITERAL(10, 105, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(11, 126, 13), // "onSocketError"
QT_MOC_LITERAL(12, 140, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(13, 169, 5) // "error"

    },
    "TemplateBindingWorker\0onConnect\0\0host\0"
    "port\0onDisconnect\0onSendImages\0onSendTxt\0"
    "txtData\0onSocketConnected\0"
    "onSocketDisconnected\0onSocketError\0"
    "QAbstractSocket::SocketError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TemplateBindingWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x0a /* Public */,
       5,    0,   54,    2, 0x0a /* Public */,
       6,    0,   55,    2, 0x0a /* Public */,
       7,    1,   56,    2, 0x0a /* Public */,
       9,    0,   59,    2, 0x08 /* Private */,
      10,    0,   60,    2, 0x08 /* Private */,
      11,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void TemplateBindingWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TemplateBindingWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onDisconnect(); break;
        case 2: _t->onSendImages(); break;
        case 3: _t->onSendTxt((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->onSocketConnected(); break;
        case 5: _t->onSocketDisconnected(); break;
        case 6: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TemplateBindingWorker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TemplateBindingWorker.data,
    qt_meta_data_TemplateBindingWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TemplateBindingWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TemplateBindingWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TemplateBindingWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TemplateBindingWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
