#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "ui_PreviewPanel.h"

class PreviewPanel : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = PreviewPanel*;
    PreviewPanel(QWidget *parent = Q_NULLPTR);
    ~PreviewPanel();

private:
    Ui::PreviewPanel ui;
};
