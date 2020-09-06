#pragma once
#include "BaseDialog.h"
class DevTreeWnd;
class PopUpTreeWnd:public BaseDialog
{
public:
    PopUpTreeWnd(QWidget* pParent = nullptr);
    ~PopUpTreeWnd();

    void SetTreeView(DevTreeWnd* pTree);
    DevTreeWnd* GetTreeView();

protected: 
    DevTreeWnd* m_pTree = nullptr;
};

