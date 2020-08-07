#pragma once
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <initializer_list>

using QLabelPtr = QLabel*;
using QPushButtonPtr = QPushButton*;
using QObjectPtr = QObject *;
using QWidgetPtr = QWidget * ;
using QHBoxLayoutPtr = QHBoxLayout * ;
using QVBoxLayoutPtr = QVBoxLayout * ;
template<typename T>
auto makeQTObject(QWidgetPtr pParent = nullptr) -> T*
{
    return new T(pParent);
}

#define  MQ(type) makeQTObject<type>

bool CheckPointer(std::initializer_list<void*> list);


