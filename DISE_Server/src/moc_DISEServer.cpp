/****************************************************************************
** Meta object code from reading C++ file 'DISEServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hdr/DISEServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DISEServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DISEServer_t {
    QByteArrayData data[38];
    char stringdata0[511];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DISEServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DISEServer_t qt_meta_stringdata_DISEServer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DISEServer"
QT_MOC_LITERAL(1, 11, 10), // "newMessage"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "newConnection"
QT_MOC_LITERAL(4, 37, 18), // "appendToSocketList"
QT_MOC_LITERAL(5, 56, 11), // "QTcpSocket*"
QT_MOC_LITERAL(6, 68, 6), // "socket"
QT_MOC_LITERAL(7, 75, 10), // "readSocket"
QT_MOC_LITERAL(8, 86, 13), // "discardSocket"
QT_MOC_LITERAL(9, 100, 12), // "handleDealer"
QT_MOC_LITERAL(10, 113, 12), // "handleClient"
QT_MOC_LITERAL(11, 126, 21), // "handleHonestInitiator"
QT_MOC_LITERAL(12, 148, 24), // "getParticipantServerList"
QT_MOC_LITERAL(13, 173, 11), // "QList<int>*"
QT_MOC_LITERAL(14, 185, 26), // "getParticipantServerKeyMap"
QT_MOC_LITERAL(15, 212, 22), // "QMap<int,QList<int>*>*"
QT_MOC_LITERAL(16, 235, 16), // "partipantServers"
QT_MOC_LITERAL(17, 252, 21), // "honestInitiatorThread"
QT_MOC_LITERAL(18, 274, 2), // "ip"
QT_MOC_LITERAL(19, 277, 4), // "port"
QT_MOC_LITERAL(20, 282, 9), // "keysToUse"
QT_MOC_LITERAL(21, 292, 14), // "unsigned char*"
QT_MOC_LITERAL(22, 307, 7), // "message"
QT_MOC_LITERAL(23, 315, 7), // "encMode"
QT_MOC_LITERAL(24, 323, 33), // "QMap<int,QList<unsigned char*..."
QT_MOC_LITERAL(25, 357, 14), // "partialResults"
QT_MOC_LITERAL(26, 372, 7), // "encrypt"
QT_MOC_LITERAL(27, 380, 6), // "msgLen"
QT_MOC_LITERAL(28, 387, 3), // "key"
QT_MOC_LITERAL(29, 391, 7), // "keySize"
QT_MOC_LITERAL(30, 399, 16), // "encryptedMessage"
QT_MOC_LITERAL(31, 416, 7), // "decrypt"
QT_MOC_LITERAL(32, 424, 16), // "decryptedMessage"
QT_MOC_LITERAL(33, 441, 22), // "encryptDecrpytWithKeys"
QT_MOC_LITERAL(34, 464, 25), // "QMap<int,unsigned char*>*"
QT_MOC_LITERAL(35, 490, 7), // "keyList"
QT_MOC_LITERAL(36, 498, 7), // "msgSize"
QT_MOC_LITERAL(37, 506, 4) // "mode"

    },
    "DISEServer\0newMessage\0\0newConnection\0"
    "appendToSocketList\0QTcpSocket*\0socket\0"
    "readSocket\0discardSocket\0handleDealer\0"
    "handleClient\0handleHonestInitiator\0"
    "getParticipantServerList\0QList<int>*\0"
    "getParticipantServerKeyMap\0"
    "QMap<int,QList<int>*>*\0partipantServers\0"
    "honestInitiatorThread\0ip\0port\0keysToUse\0"
    "unsigned char*\0message\0encMode\0"
    "QMap<int,QList<unsigned char*>*>*\0"
    "partialResults\0encrypt\0msgLen\0key\0"
    "keySize\0encryptedMessage\0decrypt\0"
    "decryptedMessage\0encryptDecrpytWithKeys\0"
    "QMap<int,unsigned char*>*\0keyList\0"
    "msgSize\0mode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DISEServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   87,    2, 0x08 /* Private */,
       4,    1,   88,    2, 0x08 /* Private */,
       7,    0,   91,    2, 0x08 /* Private */,
       8,    0,   92,    2, 0x08 /* Private */,
       9,    1,   93,    2, 0x08 /* Private */,
      10,    1,   96,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      14,    1,  103,    2, 0x08 /* Private */,
      17,    6,  106,    2, 0x08 /* Private */,
      26,    5,  119,    2, 0x08 /* Private */,
      31,    5,  130,    2, 0x08 /* Private */,
      33,    4,  141,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    0x80000000 | 13,
    0x80000000 | 15, 0x80000000 | 13,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 13, 0x80000000 | 21, QMetaType::Int, 0x80000000 | 24,   18,   19,   20,   22,   23,   25,
    QMetaType::Int, 0x80000000 | 21, QMetaType::Int, 0x80000000 | 21, QMetaType::Int, 0x80000000 | 21,   22,   27,   28,   29,   30,
    QMetaType::Int, 0x80000000 | 21, QMetaType::Int, 0x80000000 | 21, QMetaType::Int, 0x80000000 | 21,   22,   27,   28,   29,   32,
    0x80000000 | 34, 0x80000000 | 13, 0x80000000 | 21, QMetaType::Int, QMetaType::Int,   35,   22,   36,   37,

       0        // eod
};

void DISEServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DISEServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->newConnection(); break;
        case 2: _t->appendToSocketList((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 3: _t->readSocket(); break;
        case 4: _t->discardSocket(); break;
        case 5: _t->handleDealer((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 6: _t->handleClient((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 7: _t->handleHonestInitiator((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 8: { QList<int>* _r = _t->getParticipantServerList();
            if (_a[0]) *reinterpret_cast< QList<int>**>(_a[0]) = std::move(_r); }  break;
        case 9: { QMap<int,QList<int>*>* _r = _t->getParticipantServerKeyMap((*reinterpret_cast< QList<int>*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QMap<int,QList<int>*>**>(_a[0]) = std::move(_r); }  break;
        case 10: _t->honestInitiatorThread((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QList<int>*(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< QMap<int,QList<unsigned char*>*>*(*)>(_a[6]))); break;
        case 11: { int _r = _t->encrypt((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< unsigned char*(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->decrypt((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< unsigned char*(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { QMap<int,unsigned char*>* _r = _t->encryptDecrpytWithKeys((*reinterpret_cast< QList<int>*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QMap<int,unsigned char*>**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DISEServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DISEServer::newMessage)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DISEServer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DISEServer.data,
    qt_meta_data_DISEServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DISEServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DISEServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DISEServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DISEServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DISEServer::newMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
