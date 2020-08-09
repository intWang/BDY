#pragma once
#include "CmdBar.h"
#include "QtDefine.h"
#include "AreableWidget.h"
class BaseDialog :public AreableWidget<QWidget>
{
    Q_OBJECT

    using QPixmapPtr = QPixmap * ;
public:
    BaseDialog(QWidget *parent = Q_NULLPTR);
    ~BaseDialog();
    QGridLayoutPtr GetLayout();
private:
    CmdBar::Ptr m_pTitleBar = nullptr;
    BottomBar::Ptr m_pBottomBar = nullptr;
    QGridLayoutPtr m_pMainLayout = nullptr;
protected:

};

