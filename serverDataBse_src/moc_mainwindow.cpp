/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata[393];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 23),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 23),
QT_MOC_LITERAL(4, 60, 23),
QT_MOC_LITERAL(5, 84, 23),
QT_MOC_LITERAL(6, 108, 24),
QT_MOC_LITERAL(7, 133, 24),
QT_MOC_LITERAL(8, 158, 24),
QT_MOC_LITERAL(9, 183, 24),
QT_MOC_LITERAL(10, 208, 24),
QT_MOC_LITERAL(11, 233, 24),
QT_MOC_LITERAL(12, 258, 24),
QT_MOC_LITERAL(13, 283, 25),
QT_MOC_LITERAL(14, 309, 4),
QT_MOC_LITERAL(15, 314, 25),
QT_MOC_LITERAL(16, 340, 25),
QT_MOC_LITERAL(17, 366, 25)
    },
    "MainWindow\0on_pushButton_7_clicked\0\0"
    "on_pushButton_6_clicked\0on_pushButton_8_clicked\0"
    "on_pushButton_9_clicked\0"
    "on_pushButton_10_clicked\0"
    "on_pushButton_11_clicked\0"
    "on_pushButton_12_clicked\0"
    "on_pushButton_13_clicked\0"
    "on_pushButton_14_clicked\0"
    "on_pushButton_16_clicked\0"
    "on_pushButton_15_clicked\0"
    "on_lineEdit_2_textChanged\0arg1\0"
    "on_lineEdit_3_textChanged\0"
    "on_lineEdit_4_textChanged\0"
    "on_ButtonUpdateDB_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08,
       3,    0,   90,    2, 0x08,
       4,    0,   91,    2, 0x08,
       5,    0,   92,    2, 0x08,
       6,    0,   93,    2, 0x08,
       7,    0,   94,    2, 0x08,
       8,    0,   95,    2, 0x08,
       9,    0,   96,    2, 0x08,
      10,    0,   97,    2, 0x08,
      11,    0,   98,    2, 0x08,
      12,    0,   99,    2, 0x08,
      13,    1,  100,    2, 0x08,
      15,    1,  103,    2, 0x08,
      16,    1,  106,    2, 0x08,
      17,    0,  109,    2, 0x08,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_7_clicked(); break;
        case 1: _t->on_pushButton_6_clicked(); break;
        case 2: _t->on_pushButton_8_clicked(); break;
        case 3: _t->on_pushButton_9_clicked(); break;
        case 4: _t->on_pushButton_10_clicked(); break;
        case 5: _t->on_pushButton_11_clicked(); break;
        case 6: _t->on_pushButton_12_clicked(); break;
        case 7: _t->on_pushButton_13_clicked(); break;
        case 8: _t->on_pushButton_14_clicked(); break;
        case 9: _t->on_pushButton_16_clicked(); break;
        case 10: _t->on_pushButton_15_clicked(); break;
        case 11: _t->on_lineEdit_2_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->on_lineEdit_3_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->on_lineEdit_4_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->on_ButtonUpdateDB_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_END_MOC_NAMESPACE
