#pragma once
#include "BaseDialog.h"
#include "MainViewWidget.h"
#include "IServer.h"
class MainFrame:public BaseDialog
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

private:
    MainViewWidget::Ptr m_pMainViewWiget = nullptr;
};

extern ls::IServiceEngine::Ptr g_pEngine;
extern ls::ICallBackEngin::Ptr g_pCallBack;
