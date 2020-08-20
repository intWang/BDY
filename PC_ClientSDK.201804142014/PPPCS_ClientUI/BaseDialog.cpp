#include "BaseDialog.h"
#include "utils.h"
BaseDialog::BaseDialog(QWidget *parent)
    :AreableWidget<QDialog>(parent)
{
    m_pTitleBar = MQ(CmdBar)(this);
    m_pBottomBar = MQ(HintBar)(this);
    auto pLayout = MQ(QVBoxLayout)(this);
    m_pMainLayout = MQ(QVBoxLayout)(this);

    if (!utils::CheckPointer({ m_pTitleBar, pLayout, m_pMainLayout}))
    {
        LogError("Error param %d %d %d %d", m_pTitleBar, pLayout, m_pMainLayout, m_pBottomBar);
    }

    installEventFilter(m_pTitleBar);

    //setWindowIcon(QIcon(":/Images/logo
    SetArea(30, 30);
    pLayout->addWidget(m_pTitleBar);
    pLayout->addLayout(m_pMainLayout);
    pLayout->addWidget(m_pBottomBar);
    pLayout->setSpacing(0);
    pLayout->setStretch(1,10);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}


BaseDialog::~BaseDialog()
{
}

QVBoxLayoutPtr BaseDialog::GetLayout()
{
    return dynamic_cast<QVBoxLayoutPtr>(m_pMainLayout);
}

HintBar::Ptr BaseDialog::GetBottomBar()
{
    return m_pBottomBar;
}

void BaseDialog::setMinimizeVisible(bool bVisiable)
{
    if (m_pTitleBar)
    {
        m_pTitleBar->setMinimizeVisible(bVisiable);
    }
}

void BaseDialog::setMaximizeVisible(bool bVisiable)
{
    if (m_pTitleBar)
    {
        m_pTitleBar->setMaximizeVisible(bVisiable);
    }
}

void BaseDialog::setWidgetResizable(bool bVisiable)
{
    if (m_pTitleBar)
    {
        m_pTitleBar->setWidgetResizable(bVisiable);
    }
}
