#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PPPCS_ClientUI.h"

class PPPCS_ClientUI : public QMainWindow
{
    Q_OBJECT

public:
    PPPCS_ClientUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::PPPCS_ClientUIClass ui;
};
