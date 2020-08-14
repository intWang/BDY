#include "BaseDialog.h"
#include "utils.h"
BaseDialog::BaseDialog(QWidget *parent)
    :AreableWidget<QWidget>(parent)
{
    m_pTitleBar = MQ(CmdBar)(this);
    m_pBottomBar = MQ(HintBar)(this);
    auto pLayout = MQ(QVBoxLayout)(this);
    m_pMainLayout = MQ(QGridLayout)(this);

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

QGridLayoutPtr BaseDialog::GetLayout()
{
    return dynamic_cast<QGridLayoutPtr>(m_pMainLayout);
}

HintBar::Ptr BaseDialog::GetBottomBar()
{
    return m_pBottomBar;
}
