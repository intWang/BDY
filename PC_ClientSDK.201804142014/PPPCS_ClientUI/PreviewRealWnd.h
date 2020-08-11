#pragma once

#include "ui_PreviewRealWnd.h"
#include "AreableWidget.h"

class PreviewRealWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = PreviewRealWnd * ;
    PreviewRealWnd(QWidget *parent = Q_NULLPTR);
    ~PreviewRealWnd();

private:
    Ui::PreviewRealWnd ui;
};
