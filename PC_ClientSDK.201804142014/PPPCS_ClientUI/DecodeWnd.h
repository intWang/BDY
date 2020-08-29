#pragma once

#include <QWidget>
#include "ui_DecodeWnd.h"

class DecodeWnd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = DecodeWnd * ;
    DecodeWnd(QWidget *parent = Q_NULLPTR);
    ~DecodeWnd();

private:
    Ui::DecodeWnd ui;
};
