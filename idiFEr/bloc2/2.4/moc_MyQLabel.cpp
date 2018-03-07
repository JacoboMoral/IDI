/****************************************************************************
** Meta object code from reading C++ file 'MyQLabel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MyQLabel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyQLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyQLabel_t {
    QByteArrayData data[12];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyQLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyQLabel_t qt_meta_stringdata_MyQLabel = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyQLabel"
QT_MOC_LITERAL(1, 9, 13), // "setTerraColor"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 3), // "red"
QT_MOC_LITERAL(4, 28, 5), // "green"
QT_MOC_LITERAL(5, 34, 4), // "blue"
QT_MOC_LITERAL(6, 39, 6), // "setRed"
QT_MOC_LITERAL(7, 46, 1), // "r"
QT_MOC_LITERAL(8, 48, 8), // "setGreen"
QT_MOC_LITERAL(9, 57, 1), // "g"
QT_MOC_LITERAL(10, 59, 7), // "setBlue"
QT_MOC_LITERAL(11, 67, 1) // "b"

    },
    "MyQLabel\0setTerraColor\0\0red\0green\0"
    "blue\0setRed\0r\0setGreen\0g\0setBlue\0b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyQLabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   41,    2, 0x0a /* Public */,
       8,    1,   44,    2, 0x0a /* Public */,
      10,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void MyQLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyQLabel *_t = static_cast<MyQLabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setTerraColor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setRed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setGreen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setBlue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyQLabel::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyQLabel::setTerraColor)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyQLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MyQLabel.data,
      qt_meta_data_MyQLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyQLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyQLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyQLabel.stringdata0))
        return static_cast<void*>(const_cast< MyQLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int MyQLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MyQLabel::setTerraColor(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
