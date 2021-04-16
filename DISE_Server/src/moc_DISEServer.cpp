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
    QByteArrayData data[53];
    char stringdata0[712];
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
QT_MOC_LITERAL(12, 148, 23), // "handleEncryptionRequest"
QT_MOC_LITERAL(13, 172, 14), // "unsigned char*"
QT_MOC_LITERAL(14, 187, 7), // "message"
QT_MOC_LITERAL(15, 195, 13), // "sizeOfMessage"
QT_MOC_LITERAL(16, 209, 23), // "handleDecryptionRequest"
QT_MOC_LITERAL(17, 233, 10), // "cipherText"
QT_MOC_LITERAL(18, 244, 14), // "cipherTextSize"
QT_MOC_LITERAL(19, 259, 7), // "a_cat_j"
QT_MOC_LITERAL(20, 267, 24), // "getParticipantServerList"
QT_MOC_LITERAL(21, 292, 11), // "QList<int>*"
QT_MOC_LITERAL(22, 304, 26), // "getParticipantServerKeyMap"
QT_MOC_LITERAL(23, 331, 22), // "QMap<int,QList<int>*>*"
QT_MOC_LITERAL(24, 354, 16), // "partipantServers"
QT_MOC_LITERAL(25, 371, 21), // "honestInitiatorThread"
QT_MOC_LITERAL(26, 393, 2), // "ip"
QT_MOC_LITERAL(27, 396, 4), // "port"
QT_MOC_LITERAL(28, 401, 9), // "keysToUse"
QT_MOC_LITERAL(29, 411, 7), // "encMode"
QT_MOC_LITERAL(30, 419, 33), // "QMap<int,QList<unsigned char*..."
QT_MOC_LITERAL(31, 453, 14), // "partialResults"
QT_MOC_LITERAL(32, 468, 7), // "encrypt"
QT_MOC_LITERAL(33, 476, 6), // "msgLen"
QT_MOC_LITERAL(34, 483, 3), // "key"
QT_MOC_LITERAL(35, 487, 16), // "encryptedMessage"
QT_MOC_LITERAL(36, 504, 7), // "decrypt"
QT_MOC_LITERAL(37, 512, 16), // "decryptedMessage"
QT_MOC_LITERAL(38, 529, 22), // "encryptDecryptWithKeys"
QT_MOC_LITERAL(39, 552, 25), // "QMap<int,unsigned char*>*"
QT_MOC_LITERAL(40, 578, 7), // "keyList"
QT_MOC_LITERAL(41, 586, 7), // "msgSize"
QT_MOC_LITERAL(42, 594, 4), // "mode"
QT_MOC_LITERAL(43, 599, 20), // "printServerKeysToUse"
QT_MOC_LITERAL(44, 620, 15), // "serverKeysToUse"
QT_MOC_LITERAL(45, 636, 10), // "cryptoHash"
QT_MOC_LITERAL(46, 647, 4), // "data"
QT_MOC_LITERAL(47, 652, 7), // "dataLen"
QT_MOC_LITERAL(48, 660, 6), // "result"
QT_MOC_LITERAL(49, 667, 20), // "randomNumberWithSeed"
QT_MOC_LITERAL(50, 688, 4), // "seed"
QT_MOC_LITERAL(51, 693, 7), // "seedLen"
QT_MOC_LITERAL(52, 701, 10) // "resultSize"

    },
    "DISEServer\0newMessage\0\0newConnection\0"
    "appendToSocketList\0QTcpSocket*\0socket\0"
    "readSocket\0discardSocket\0handleDealer\0"
    "handleClient\0handleHonestInitiator\0"
    "handleEncryptionRequest\0unsigned char*\0"
    "message\0sizeOfMessage\0handleDecryptionRequest\0"
    "cipherText\0cipherTextSize\0a_cat_j\0"
    "getParticipantServerList\0QList<int>*\0"
    "getParticipantServerKeyMap\0"
    "QMap<int,QList<int>*>*\0partipantServers\0"
    "honestInitiatorThread\0ip\0port\0keysToUse\0"
    "encMode\0QMap<int,QList<unsigned char*>*>*\0"
    "partialResults\0encrypt\0msgLen\0key\0"
    "encryptedMessage\0decrypt\0decryptedMessage\0"
    "encryptDecryptWithKeys\0QMap<int,unsigned char*>*\0"
    "keyList\0msgSize\0mode\0printServerKeysToUse\0"
    "serverKeysToUse\0cryptoHash\0data\0dataLen\0"
    "result\0randomNumberWithSeed\0seed\0"
    "seedLen\0resultSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DISEServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  112,    2, 0x08 /* Private */,
       4,    1,  113,    2, 0x08 /* Private */,
       7,    0,  116,    2, 0x08 /* Private */,
       8,    0,  117,    2, 0x08 /* Private */,
       9,    1,  118,    2, 0x08 /* Private */,
      10,    1,  121,    2, 0x08 /* Private */,
      11,    1,  124,    2, 0x08 /* Private */,
      12,    3,  127,    2, 0x08 /* Private */,
      16,    4,  134,    2, 0x08 /* Private */,
      20,    0,  143,    2, 0x08 /* Private */,
      22,    1,  144,    2, 0x08 /* Private */,
      25,    6,  147,    2, 0x08 /* Private */,
      32,    4,  160,    2, 0x08 /* Private */,
      36,    4,  169,    2, 0x08 /* Private */,
      38,    4,  178,    2, 0x08 /* Private */,
      43,    1,  187,    2, 0x08 /* Private */,
      45,    3,  190,    2, 0x08 /* Private */,
      49,    4,  197,    2, 0x08 /* Private */,

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
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 13, QMetaType::Int,    6,   14,   15,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 13,    6,   17,   18,   19,
    0x80000000 | 21,
    0x80000000 | 23, 0x80000000 | 21,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 21, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 30,   26,   27,   28,   14,   29,   31,
    QMetaType::Int, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 13, 0x80000000 | 13,   14,   33,   34,   35,
    QMetaType::Int, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 13, 0x80000000 | 13,   14,   33,   34,   37,
    0x80000000 | 39, 0x80000000 | 21, 0x80000000 | 13, QMetaType::Int, QMetaType::Int,   40,   14,   41,   42,
    QMetaType::Void, 0x80000000 | 23,   44,
    QMetaType::UInt, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 13,   46,   47,   48,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 13, QMetaType::Int,   50,   51,   48,   52,

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
        case 8: _t->handleEncryptionRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->handleDecryptionRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4]))); break;
        case 10: { QList<int>* _r = _t->getParticipantServerList();
            if (_a[0]) *reinterpret_cast< QList<int>**>(_a[0]) = std::move(_r); }  break;
        case 11: { QMap<int,QList<int>*>* _r = _t->getParticipantServerKeyMap((*reinterpret_cast< QList<int>*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QMap<int,QList<int>*>**>(_a[0]) = std::move(_r); }  break;
        case 12: _t->honestInitiatorThread((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QList<int>*(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< QMap<int,QList<unsigned char*>*>*(*)>(_a[6]))); break;
        case 13: { int _r = _t->encrypt((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->decrypt((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 15: { QMap<int,unsigned char*>* _r = _t->encryptDecryptWithKeys((*reinterpret_cast< QList<int>*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QMap<int,unsigned char*>**>(_a[0]) = std::move(_r); }  break;
        case 16: _t->printServerKeysToUse((*reinterpret_cast< QMap<int,QList<int>*>*(*)>(_a[1]))); break;
        case 17: { uint _r = _t->cryptoHash((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->randomNumberWithSeed((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 9:
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
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
