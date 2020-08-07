#pragma once
#include "BaseDialog.h"
#include "MainViewWnd.h"
class MainFrame:public BaseDialog
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

private:
    MainViewWnd::Ptr m_pMainView = nullptr;
};

