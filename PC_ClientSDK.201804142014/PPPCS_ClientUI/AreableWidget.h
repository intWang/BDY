#pragma once
#include <QPainter>
#include <QPaintEvent>
#include "QtDefine.h"
template <typename T>
class AreableWidget :public T
{
public:
    AreableWidget(QWidget *parent = Q_NULLPTR);
    ~AreableWidget();

    void SetArea(int heightTop, int heightBottom);
    void SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    int m_heightTop = 0;
    int m_heightBottom = 0;
    QColor m_clrTop = s_qcl292C39;
    QColor m_clrBottom = s_qcl292C39;
    QColor m_clrMid = s_qcl1E2233;
};

template <typename T>
void AreableWidget<T>::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int nBottomHeight = m_heightBottom;
    int nTopHeight = m_heightTop;

    QSize wndSize = size();
    QRect top(0, 0, wndSize.width(), nTopHeight);
    QRect bottom(0, wndSize.height() - nBottomHeight, wndSize.width(), nBottomHeight);
    QRect center(0, top.height(), wndSize.width(), wndSize.height() - nBottomHeight);

    if (nTopHeight)
    {
        painter.fillRect(top, m_clrTop);
    }
    painter.fillRect(center, m_clrMid);
    if (nBottomHeight)
    {
        painter.fillRect(bottom, m_clrBottom);
    }
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
}

template <typename T>
AreableWidget<T>::~AreableWidget()
{

}

template <typename T>
AreableWidget<T>::AreableWidget(QWidget *parent)
    :T(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}
