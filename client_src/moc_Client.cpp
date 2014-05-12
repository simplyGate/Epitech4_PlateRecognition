/****************************************************************************
** Meta object code from reading C++ file 'Client.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Client.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Client.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FenClient_t {
    QByteArrayData data[14];
    char stringdata[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_FenClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_FenClient_t qt_meta_stringdata_FenClient = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 26),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 24),
QT_MOC_LITERAL(4, 63, 24),
QT_MOC_LITERAL(5, 88, 21),
QT_MOC_LITERAL(6, 110, 18),
QT_MOC_LITERAL(7, 129, 17),
QT_MOC_LITERAL(8, 147, 13),
QT_MOC_LITERAL(9, 161, 8),
QT_MOC_LITERAL(10, 170, 10),
QT_MOC_LITERAL(11, 181, 12),
QT_MOC_LITERAL(12, 194, 28),
QT_MOC_LITERAL(13, 223, 6)
    },
    "FenClient\0on_boutonConnexion_clicked\0"
    "\0on_boutonEnvoyer_clicked\0"
    "on_message_returnPressed\0on_pushButton_clicked\0"
    "on_btnSend_clicked\0finishedUploading\0"
    "donneesRecues\0connecte\0deconnecte\0"
    "erreurSocket\0QAbstractSocket::SocketError\0"
    "erreur\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FenClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08,
       3,    0,   65,    2, 0x08,
       4,    0,   66,    2, 0x08,
       5,    0,   67,    2, 0x08,
       6,    0,   68,    2, 0x08,
       7,    0,   69,    2, 0x08,
       8,    0,   70,    2, 0x08,
       9,    0,   71,    2, 0x08,
      10,    0,   72,    2, 0x08,
      11,    1,   73,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void FenClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FenClient *_t = static_cast<FenClient *>(_o);
        switch (_id) {
        case 0: _t->on_boutonConnexion_clicked(); break;
        case 1: _t->on_boutonEnvoyer_clicked(); break;
        case 2: _t->on_message_returnPressed(); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->on_btnSend_clicked(); break;
        case 5: _t->finishedUploading(); break;
        case 6: _t->donneesRecues(); break;
        case 7: _t->connecte(); break;
        case 8: _t->deconnecte(); break;
        case 9: _t->erreurSocket((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject FenClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FenClient.data,
      qt_meta_data_FenClient,  qt_static_metacall, 0, 0}
};


const QMetaObject *FenClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FenClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FenClient.stringdata))
        return static_cast<void*>(const_cast< FenClient*>(this));
    return QWidget::qt_metacast(_clname);
}

int FenClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
