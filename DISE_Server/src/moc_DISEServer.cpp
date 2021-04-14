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
    QByteArrayData data[12];
    char stringdata0[148];
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
QT_MOC_LITERAL(11, 126, 21) // "handleHonestInitiator"

    },
    "DISEServer\0newMessage\0\0newConnection\0"
    "appendToSocketList\0QTcpSocket*\0socket\0"
    "readSocket\0discardSocket\0handleDealer\0"
    "handleClient\0handleHonestInitiator"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DISEServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   57,    2, 0x08 /* Private */,
       4,    1,   58,    2, 0x08 /* Private */,
       7,    0,   61,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x08 /* Private */,
      10,    1,   66,    2, 0x08 /* Private */,
      11,    1,   69,    2, 0x08 /* Private */,

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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
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
