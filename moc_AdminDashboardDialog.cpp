/****************************************************************************
** Meta object code from reading C++ file 'AdminDashboardDialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/AdminDashboardDialog.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdminDashboardDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20AdminDashboardDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto AdminDashboardDialog::qt_create_metaobjectdata<qt_meta_tag_ZN20AdminDashboardDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AdminDashboardDialog",
        "productAdded",
        "",
        "productId",
        "productUpdated",
        "productRemoved",
        "userUpdated",
        "userId",
        "userRemoved",
        "orderStatusUpdated",
        "orderId",
        "newStatus"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'productAdded'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'productUpdated'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'productRemoved'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'userUpdated'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'userRemoved'
        QtMocHelpers::SignalData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'orderStatusUpdated'
        QtMocHelpers::SignalData<void(int, const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::QString, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AdminDashboardDialog, qt_meta_tag_ZN20AdminDashboardDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AdminDashboardDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20AdminDashboardDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20AdminDashboardDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20AdminDashboardDialogE_t>.metaTypes,
    nullptr
} };

void AdminDashboardDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AdminDashboardDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->productAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->productUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->productRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->userUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->userRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->orderStatusUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int )>(_a, &AdminDashboardDialog::productAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int )>(_a, &AdminDashboardDialog::productUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int )>(_a, &AdminDashboardDialog::productRemoved, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int )>(_a, &AdminDashboardDialog::userUpdated, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int )>(_a, &AdminDashboardDialog::userRemoved, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (AdminDashboardDialog::*)(int , const QString & )>(_a, &AdminDashboardDialog::orderStatusUpdated, 5))
            return;
    }
}

const QMetaObject *AdminDashboardDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminDashboardDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20AdminDashboardDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int AdminDashboardDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AdminDashboardDialog::productAdded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void AdminDashboardDialog::productUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void AdminDashboardDialog::productRemoved(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void AdminDashboardDialog::userUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void AdminDashboardDialog::userRemoved(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void AdminDashboardDialog::orderStatusUpdated(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}
QT_WARNING_POP
