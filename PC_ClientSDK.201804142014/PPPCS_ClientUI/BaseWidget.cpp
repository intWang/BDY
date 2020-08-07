#include "BaseWidget.h"
#include <QPainter>

BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

BaseWidget::~BaseWidget()
{
}

void BaseWidget::SetArea(int heightTop, int heightBottom)
{
    m_heightTop = heightTop;
    m_heightBottom = heightBottom;
}

void BaseWidget::SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom)
{
    m_clrTop = clrTop;
    m_clrBottom = clrBottom;
    m_clrMid = clrMid;
}

void BaseWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

void BaseWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
        move(event->pos() - m_point + pos());
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void BaseWidget::paintEvent(QPaintEvent *event)
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
