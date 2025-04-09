/****************************************************************************
** Meta object code from reading C++ file 'myrenderwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../myrenderwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myrenderwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyRenderWindow_t {
    QByteArrayData data[22];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyRenderWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyRenderWindow_t qt_meta_stringdata_MyRenderWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MyRenderWindow"
QT_MOC_LITERAL(1, 15, 16), // "fullScreenSignal"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 18), // "cursorChangeSignal"
QT_MOC_LITERAL(4, 52, 11), // "rightSignal"
QT_MOC_LITERAL(5, 64, 5), // "point"
QT_MOC_LITERAL(6, 70, 10), // "pointLocal"
QT_MOC_LITERAL(7, 81, 16), // "middlePickSignal"
QT_MOC_LITERAL(8, 98, 1), // "x"
QT_MOC_LITERAL(9, 100, 1), // "y"
QT_MOC_LITERAL(10, 102, 12), // "leftUpSignal"
QT_MOC_LITERAL(11, 115, 5), // "index"
QT_MOC_LITERAL(12, 121, 15), // "mouseMoveSignal"
QT_MOC_LITERAL(13, 137, 14), // "isMousePressed"
QT_MOC_LITERAL(14, 152, 19), // "leftMouseDownSignal"
QT_MOC_LITERAL(15, 172, 17), // "leftMouseUpSignal"
QT_MOC_LITERAL(16, 190, 14), // "KeyPressSignal"
QT_MOC_LITERAL(17, 205, 10), // "QKeyEvent*"
QT_MOC_LITERAL(18, 216, 2), // "ev"
QT_MOC_LITERAL(19, 219, 20), // "TranslatePressSignal"
QT_MOC_LITERAL(20, 240, 16), // "RoatePressSignal"
QT_MOC_LITERAL(21, 257, 15) // "PickPressSignal"

    },
    "MyRenderWindow\0fullScreenSignal\0\0"
    "cursorChangeSignal\0rightSignal\0point\0"
    "pointLocal\0middlePickSignal\0x\0y\0"
    "leftUpSignal\0index\0mouseMoveSignal\0"
    "isMousePressed\0leftMouseDownSignal\0"
    "leftMouseUpSignal\0KeyPressSignal\0"
    "QKeyEvent*\0ev\0TranslatePressSignal\0"
    "RoatePressSignal\0PickPressSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyRenderWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    1,   75,    2, 0x06 /* Public */,
       4,    2,   78,    2, 0x06 /* Public */,
       7,    2,   83,    2, 0x06 /* Public */,
      10,    2,   88,    2, 0x06 /* Public */,
      12,    3,   93,    2, 0x06 /* Public */,
      14,    2,  100,    2, 0x06 /* Public */,
      15,    2,  105,    2, 0x06 /* Public */,
      16,    1,  110,    2, 0x06 /* Public */,
      19,    1,  113,    2, 0x06 /* Public */,
      20,    1,  116,    2, 0x06 /* Public */,
      21,    1,  119,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QPoint, QMetaType::QPoint,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QPoint, QMetaType::Int,    5,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    8,    9,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void MyRenderWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyRenderWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fullScreenSignal(); break;
        case 1: _t->cursorChangeSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->rightSignal((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2]))); break;
        case 3: _t->middlePickSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->leftUpSignal((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->mouseMoveSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->leftMouseDownSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->leftMouseUpSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->KeyPressSignal((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 9: _t->TranslatePressSignal((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 10: _t->RoatePressSignal((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 11: _t->PickPressSignal((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyRenderWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::fullScreenSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::cursorChangeSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QPoint , QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::rightSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::middlePickSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QPoint , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::leftUpSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(int , int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::mouseMoveSignal)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::leftMouseDownSignal)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::leftMouseUpSignal)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::KeyPressSignal)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::TranslatePressSignal)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::RoatePressSignal)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (MyRenderWindow::*)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyRenderWindow::PickPressSignal)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MyRenderWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QVTKOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_MyRenderWindow.data,
    qt_meta_data_MyRenderWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyRenderWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyRenderWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyRenderWindow.stringdata0))
        return static_cast<void*>(this);
    return QVTKOpenGLWidget::qt_metacast(_clname);
}

int MyRenderWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVTKOpenGLWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void MyRenderWindow::fullScreenSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MyRenderWindow::cursorChangeSignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyRenderWindow::rightSignal(QPoint _t1, QPoint _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyRenderWindow::middlePickSignal(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MyRenderWindow::leftUpSignal(QPoint _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MyRenderWindow::mouseMoveSignal(int _t1, int _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MyRenderWindow::leftMouseDownSignal(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MyRenderWindow::leftMouseUpSignal(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MyRenderWindow::KeyPressSignal(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MyRenderWindow::TranslatePressSignal(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MyRenderWindow::RoatePressSignal(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void MyRenderWindow::PickPressSignal(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
