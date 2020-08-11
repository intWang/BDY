#pragma once

#include <QWidget>
#include "ui_DevTreeWnd.h"
#include "AreableWidget.h"
class DevTreeWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = DevTreeWnd * ;
    DevTreeWnd(QWidget *parent = Q_NULLPTR);
    ~DevTreeWnd();

private:
    Ui::DevTreeWnd ui;
protected:
    virtual BarWidget::Ptr InitTopBar() override;
    virtual BarWidget::Ptr InitBottomBar() override;

    QLayoutPtr InitCenterCtl();

};
