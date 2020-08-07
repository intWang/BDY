#pragma once
#include "BaseWidget.h"
class MainViewWnd: public BaseWidget
{
public:
    using Ptr = MainViewWnd * ;
    MainViewWnd(QWidget *parent = Q_NULLPTR);
    ~MainViewWnd();
};

