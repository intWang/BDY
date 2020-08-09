#include "MainFrame.h"
#include <QHBoxLayout>
MainFrame::MainFrame(QWidget *parent)
    :BaseDialog(parent)
{
    m_pMainViewWiget = MQ(MainViewWidget)(this);
    auto pLayout = GetLayout();
    pLayout->addWidget(m_pMainViewWiget);
    m_pMainViewWiget->show();
    pLayout->setContentsMargins(10, 0, 10, 0);
    pLayout->update();
}


MainFrame::~MainFrame()
{
}
