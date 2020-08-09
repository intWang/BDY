#pragma once

#include "AreableWidget.h"
#include "ui_MainViewWidget.h"
class MainViewWidget : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = MainViewWidget * ;
    MainViewWidget(QWidget *parent = Q_NULLPTR);
    ~MainViewWidget();

private:
    Ui::MainViewWidget ui;

protected:

};
