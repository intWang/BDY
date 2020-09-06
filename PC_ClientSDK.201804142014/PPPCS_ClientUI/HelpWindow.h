#pragma once

#include <QWidget>
#include "ui_HelpWindow.h"
#include "BaseDialog.h"
class HelpWindow : public BaseDialog
{
    Q_OBJECT

public:
    HelpWindow(QWidget *parent = Q_NULLPTR);
    ~HelpWindow();

private:
    Ui::HelpWindow ui;
};
