#include "MainFrame.h"
#include <QHBoxLayout>
MainFrame::MainFrame(QWidget *parent)
    :BaseDialog(parent)
{
    m_pMainView = MQ(MainViewWnd)(this);
    auto pLayout = GetLayout();
    pLayout->addWidget(new QWidget(this));
    pLayout->addWidget(m_pMainView);
    m_pMainView->show();
    pLayout->update();
}


MainFrame::~MainFrame()
{
}
