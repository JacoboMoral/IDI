/****************************************************************************
** Meta object code from reading C++ file 'MyGLWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MyGLWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyGLWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyGLWidget_t {
    QByteArrayData data[26];
    char stringdata0[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyGLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyGLWidget_t qt_meta_stringdata_MyGLWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyGLWidget"
QT_MOC_LITERAL(1, 11, 6), // "angleX"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 1), // "x"
QT_MOC_LITERAL(4, 21, 6), // "angleY"
QT_MOC_LITERAL(5, 28, 1), // "y"
QT_MOC_LITERAL(6, 30, 10), // "changeZoom"
QT_MOC_LITERAL(7, 41, 5), // "angle"
QT_MOC_LITERAL(8, 47, 14), // "chargePatricio"
QT_MOC_LITERAL(9, 62, 15), // "chargePatricio2"
QT_MOC_LITERAL(10, 78, 10), // "chargeLego"
QT_MOC_LITERAL(11, 89, 11), // "chargeHomer"
QT_MOC_LITERAL(12, 101, 14), // "setPerspective"
QT_MOC_LITERAL(13, 116, 13), // "setOrthogonal"
QT_MOC_LITERAL(14, 130, 9), // "setEscala"
QT_MOC_LITERAL(15, 140, 3), // "esc"
QT_MOC_LITERAL(16, 144, 6), // "eulerX"
QT_MOC_LITERAL(17, 151, 5), // "graus"
QT_MOC_LITERAL(18, 157, 6), // "eulerY"
QT_MOC_LITERAL(19, 164, 16), // "changeTerraColor"
QT_MOC_LITERAL(20, 181, 5), // "r_new"
QT_MOC_LITERAL(21, 187, 5), // "g_new"
QT_MOC_LITERAL(22, 193, 5), // "b_new"
QT_MOC_LITERAL(23, 199, 9), // "viewportW"
QT_MOC_LITERAL(24, 209, 4), // "prop"
QT_MOC_LITERAL(25, 214, 9) // "viewportH"

    },
    "MyGLWidget\0angleX\0\0x\0angleY\0y\0changeZoom\0"
    "angle\0chargePatricio\0chargePatricio2\0"
    "chargeLego\0chargeHomer\0setPerspective\0"
    "setOrthogonal\0setEscala\0esc\0eulerX\0"
    "graus\0eulerY\0changeTerraColor\0r_new\0"
    "g_new\0b_new\0viewportW\0prop\0viewportH"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   95,    2, 0x0a /* Public */,
       8,    0,   98,    2, 0x0a /* Public */,
       9,    0,   99,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    0,  101,    2, 0x0a /* Public */,
      12,    0,  102,    2, 0x0a /* Public */,
      13,    0,  103,    2, 0x0a /* Public */,
      14,    1,  104,    2, 0x0a /* Public */,
      16,    1,  107,    2, 0x0a /* Public */,
      18,    1,  110,    2, 0x0a /* Public */,
      19,    3,  113,    2, 0x0a /* Public */,
      23,    1,  120,    2, 0x0a /* Public */,
      25,    1,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   20,   21,   22,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::Int,   24,

       0        // eod
};

void MyGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyGLWidget *_t = static_cast<MyGLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->angleX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->angleY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->chargePatricio(); break;
        case 4: _t->chargePatricio2(); break;
        case 5: _t->chargeLego(); break;
        case 6: _t->chargeHomer(); break;
        case 7: _t->setPerspective(); break;
        case 8: _t->setOrthogonal(); break;
        case 9: _t->setEscala((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->eulerX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->eulerY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->changeTerraColor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 13: _t->viewportW((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->viewportH((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyGLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyGLWidget::angleX)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyGLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyGLWidget::angleY)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MyGLWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_MyGLWidget.data,
      qt_meta_data_MyGLWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyGLWidget.stringdata0))
        return static_cast<void*>(const_cast< MyGLWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(const_cast< MyGLWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int MyGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MyGLWidget::angleX(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyGLWidget::angleY(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
