#pragma once

#include <QWidget>
#include "QtDefine.h"
#include "IBarWidgetCallBack.h"
class BarWidget :public QWidget
{
public:
    using Ptr = BarWidget * ;
    ~BarWidget();
    BarWidget(QWidget *parent = 0);
    QHBoxLayoutPtr GetLayout();

protected:
    QHBoxLayoutPtr m_pMainLayout = nullptr;
    IBarWidgetCallBack::Ptr m_pCallBack;
};

