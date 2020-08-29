#pragma once
#include "AreableWidget.h"
#include "QtDefine.h"
class ContainerWnd:public AreableWidget<QWidget>
{
public:
    using Ptr = ContainerWnd*;
    ContainerWnd(QWidget* pParent);
    ~ContainerWnd();

    void AddLayout(QLayoutPtr pLayout);
    void SetBkColor(QColor qclBorder);
    void SetBorder(QRect rcBorder, QColor qclBorder = s_qclBorder2);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    
    QLayoutPtr m_pLayout = nullptr;
    QRect m_rcBorder;
    QColor m_qclBorder;
    QColor m_qclBk;
};

