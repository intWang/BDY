#include "BaseDialog.h"
BaseDialog::BaseDialog(QWidget *parent)
    :BaseWidget(parent)
{
    m_pTitleBar = MQ(CmdBar)(this);
    auto pLayout = MQ(QVBoxLayout)(this);
    m_pMainLayout = MQ(QVBoxLayout)(this);

    if (!CheckPointer({ m_pTitleBar, pLayout, m_pMainLayout}))
    {
        qCritical("Error param %d %d %d", m_pTitleBar, pLayout, m_pMainLayout);
    }

    installEventFilter(m_pTitleBar);

    resize(800, 600);
    setWindowTitle("Custom Window");
    //setWindowIcon(QIcon(":/Images/logo
    SetArea(30, 30);
    pLayout->addWidget(m_pTitleBar);
    pLayout->addLayout(m_pMainLayout);
    pLayout->addStretch();
    pLayout->setSpacing(0);
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
