#include "ContainerWnd.h"



ContainerWnd::ContainerWnd(QWidget* pParent)
    :AreableWidget<QWidget>(pParent)
{
    SetArea(0, 0);
}


ContainerWnd::~ContainerWnd()
{
}

void ContainerWnd::AddLayout(QLayoutPtr pLayout)
{
    m_pLayout = pLayout;
    setLayout(m_pLayout);
    
}

void ContainerWnd::SetBkColor(QColor qclBorder)
{
    m_qclBk = qclBorder;
}

void ContainerWnd::SetBorder(QRect rcBorder, QColor qclBorder /*= s_qclBorder2*/)
{
    m_qclBorder = qclBorder;
    m_rcBorder = rcBorder;
}

void ContainerWnd::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), m_qclBk);

    if (m_rcBorder.left())
    {
        painter.fillRect(0, 0, m_rcBorder.left(), height(), m_qclBorder);
    }

    if (m_rcBorder.right())
    {
        painter.fillRect(width() - m_rcBorder.right(), 0, m_rcBorder.right(), height(), m_qclBorder);
    }

    if (m_rcBorder.top())
    {
        painter.fillRect(0, 0, width(), m_rcBorder.top(), m_qclBorder);
    }

    if (m_rcBorder.bottom())
    {
        painter.fillRect(0, height() - m_rcBorder.bottom(), width(), m_rcBorder.bottom(), m_qclBorder);
    }
}
