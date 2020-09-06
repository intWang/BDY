#pragma once
#include <QPainter>
#include <QPaintEvent>
#include "QtDefine.h"
#include "BarWidget.h"
template <typename T>
class AreableWidget :public T
{
public:
    AreableWidget(QWidget *parent = Q_NULLPTR);
    ~AreableWidget();

    void SetArea(int heightTop, int heightBottom);
    void SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom);
    void SetBorder(int nBorderWidth);

    QRect GetTopArea();
    QRect GetCenterArea();
    QRect GetBottomArea();

    void EnableBottom(bool bEnable);
    void SetNoBottom(bool bSet = true);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual BarWidget::Ptr GetTopWnd();
    virtual BarWidget::Ptr GetBottomWnd();
    virtual BarWidget::Ptr InitTopBar() { return GetTopWnd(); };
    virtual BarWidget::Ptr InitBottomBar() { return GetBottomWnd(); };
protected:
    int m_heightTop = 0;
    int m_heightBottom = 0;
    int m_nBorderWidth = 0;
    bool m_bNoBottom = true;
    QColor m_clrTop = s_qcl292C39;
    QColor m_clrBottom = s_qcl292C39;
    QColor m_clrMid = s_qcl1E2233;

    BarWidget::Ptr m_TopBar = nullptr;
    BarWidget::Ptr m_BottomBar = nullptr;

    QRect m_rcTop;
    QRect m_rcCenter;
    QRect m_rcBottom;
};

template <typename T>
void AreableWidget<T>::SetNoBottom(bool bSet)
{
    m_bNoBottom = bSet;
    if (m_bNoBottom)
    {
        if (auto pBottom = GetBottomWnd())
        {
            pBottom->hide();
        }
    }
    else
    {
        if (auto pBottom = GetBottomWnd())
        {
            pBottom->show();
        }
    }
}

template <typename T>
void AreableWidget<T>::EnableBottom(bool bEnable)
{
    if (m_BottomBar)
    {
        m_BottomBar->setEnabled(bEnable);
    }
}

template <typename T>
QRect AreableWidget<T>::GetBottomArea()
{
    return m_rcBottom;
}

template <typename T>
QRect AreableWidget<T>::GetCenterArea()
{
    return m_rcCenter;
}

template <typename T>
QRect AreableWidget<T>::GetTopArea()
{
    return m_rcTop;
}

template <typename T>
AreableWidget<T>::AreableWidget(QWidget *parent /*= Q_NULLPTR*/)
    :T(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

template <typename T>
void AreableWidget<T>::SetBorder(int nBorderWidth)
{
    m_nBorderWidth = nBorderWidth;
}

template <typename T>
BarWidget::Ptr AreableWidget<T>::GetBottomWnd()
{
    if (m_bNoBottom)
    {
        return nullptr;
    }
    return m_BottomBar;
}

template <typename T>
BarWidget::Ptr AreableWidget<T>::GetTopWnd()
{
    return m_TopBar;
}

template <typename T>
void AreableWidget<T>::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int nBottomHeight = m_heightBottom;
    int nTopHeight = m_heightTop;
    int nBorderWidth = m_nBorderWidth;
//     if (m_TopBar && !m_TopBar->isVisible())
//     {
//         nTopHeight = 0;
//     }
//     if (m_BottomBar && !m_BottomBar->isVisible())
//     {
//         nBottomHeight = 0;
//     }
    QSize wndSize = size();
    QRect top(0, 0, wndSize.width(), nTopHeight);
    QRect bottom(0, wndSize.height() - nBottomHeight, wndSize.width(), nBottomHeight);
    QRect center(0, top.height(), wndSize.width(), wndSize.height() - nBottomHeight - nTopHeight);

    m_rcTop = top;
    m_rcCenter = center;
    m_rcBottom = bottom;
    if (nTopHeight)
    {
        painter.fillRect(top, m_clrTop);
    }
    if (nBorderWidth)
    {
        painter.fillRect(center, m_clrTop);
        center.setLeft(center.left() + nBorderWidth);
        center.setRight(center.right() - nBorderWidth);
        center.setBottom(center.bottom() - nBorderWidth);
    }
    painter.fillRect(center, m_clrMid);
    if (nBottomHeight)
    {
        painter.fillRect(bottom, m_clrBottom);
    }
// 
//     painter.setPen(QPen(s_qclBorder2));
//     painter.drawRect(0, 0, wndSize.width(), wndSize.height());
}

template <typename T>
void AreableWidget<T>::SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom)
{
    m_clrTop = clrTop;
    m_clrBottom = clrBottom;
    m_clrMid = clrMid;
}

template <typename T>
void AreableWidget<T>::SetArea(int heightTop, int heightBottom)
{
    m_heightTop = heightTop;
    m_heightBottom = heightBottom;
    if (m_heightBottom && !m_BottomBar)
    {
        m_BottomBar = MQ(BarWidget)(this);
        m_bNoBottom = false;
    }
    if (m_heightBottom)
    {
        m_BottomBar->setFixedHeight(m_heightBottom);
    }
    if (m_heightTop && !m_TopBar)
    {
        m_TopBar = MQ(BarWidget)(this);
    }
    if (m_heightTop)
    {
        m_TopBar->setFixedHeight(m_heightTop);
    }
}

template <typename T>
AreableWidget<T>::~AreableWidget()
{

}