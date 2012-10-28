/****************************************************************************
** Meta object code from reading C++ file 'mypanelopengl.h'
**
** Created: Sat Oct 27 17:56:10 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Light_Wave_GL/mypanelopengl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mypanelopengl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyPanelOpenGL[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      31,   14,   14,   14, 0x0a,
      47,   14,   14,   14, 0x0a,
      62,   14,   14,   14, 0x0a,
      77,   14,   14,   14, 0x0a,
      90,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyPanelOpenGL[] = {
    "MyPanelOpenGL\0\0selectObject1()\0"
    "selectObject2()\0selectCamera()\0"
    "changeShader()\0resetLight()\0resetObjects()\0"
};

void MyPanelOpenGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyPanelOpenGL *_t = static_cast<MyPanelOpenGL *>(_o);
        switch (_id) {
        case 0: _t->selectObject1(); break;
        case 1: _t->selectObject2(); break;
        case 2: _t->selectCamera(); break;
        case 3: _t->changeShader(); break;
        case 4: _t->resetLight(); break;
        case 5: _t->resetObjects(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MyPanelOpenGL::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyPanelOpenGL::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_MyPanelOpenGL,
      qt_meta_data_MyPanelOpenGL, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyPanelOpenGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyPanelOpenGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyPanelOpenGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyPanelOpenGL))
        return static_cast<void*>(const_cast< MyPanelOpenGL*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int MyPanelOpenGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
