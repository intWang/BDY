#include "BarWidget.h"

BarWidget::BarWidget(QWidget *parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    m_pMainLayout = MQ(QHBoxLayout)(this);
    setLayout(m_pMainLayout);
}


QHBoxLayoutPtr BarWidget::GetLayout()
{
    return m_pMainLayout;
}

BarWidget::~BarWidget()
{
}
