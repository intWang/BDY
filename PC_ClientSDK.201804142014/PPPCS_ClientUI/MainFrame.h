#pragma once
#include "BaseDialog.h"
#include "MainViewWidget.h"
class MainFrame:public BaseDialog
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

private:
    MainViewWidget::Ptr m_pMainViewWiget = nullptr;
};

