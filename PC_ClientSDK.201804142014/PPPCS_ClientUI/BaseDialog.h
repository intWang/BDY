#pragma once
#include "BaseWidget.h"
#include "CmdBar.h"
#include "QtDefine.h"
class BaseDialog :public BaseWidget
{
    using QPixmapPtr = QPixmap * ;
public:
    BaseDialog(QWidget *parent = Q_NULLPTR);
    ~BaseDialog();
    QVBoxLayoutPtr GetLayout();
private:
    CmdBar::Ptr m_pTitleBar = nullptr;
    QVBoxLayoutPtr m_pMainLayout = nullptr;
protected:

};

