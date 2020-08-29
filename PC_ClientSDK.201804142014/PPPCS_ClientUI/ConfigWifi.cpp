#include "ConfigWifi.h"
#include "ConfigWifiList.h"
#include "ConfigWifiConfirm.h"
ConfigWifi::ConfigWifi(DevNode::Ptr pDevNode, QWidget *parent)
    : QWidget(parent)
    , m_pDevNode(pDevNode)
{
    auto pLayout = MQ(QHBoxLayout)(this);
    m_pStackWnd = MQ(QStackedWidget)(this);
    if (m_pStackWnd)
    {
        InitStackWnd();
        pLayout->addWidget(m_pStackWnd);
        pLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(pLayout);
    }
    if (m_pDevNode)
    {
        m_pDevNode->SearchWifi();
    }
}

ConfigWifi::~ConfigWifi()
{
}

void ConfigWifi::SetWifiData(const IPCNetWifiAplist::Ptr& stData)
{
    if (m_pListWnd)
    {
        m_pListWnd->SetWifiData(stData);
    }
    m_pWifiData = stData;
    m_pDevNode->SetWifiList(m_pWifiData);
}

void ConfigWifi::SetConnectedWif(const IPCNetWirelessConfig_st::Ptr& stData)
{
    if (m_pListWnd)
    {
        m_pListWnd->SetConnectedWif(stData);
    }
}

void ConfigWifi::StartLinkWifi(int nIndex)
{
    if (m_pStackWnd )
    {
        m_pStackWnd->setCurrentIndex(1);
    }
    m_nCurrentSelWifi = nIndex;
}

void ConfigWifi::LinkWifi(QString strPwd)
{
    ReturnToLisk();
    if (m_pDevNode && m_pWifiData && m_nCurrentSelWifi != -1 && m_nCurrentSelWifi < m_pWifiData->num)
    {
        if (auto pItem = m_pWifiData->ApItem[m_nCurrentSelWifi])
        {
            m_pDevNode->SetWifi(pItem->SSID, strPwd.toStdString(), pItem->EncType);
        }
    }
}

void ConfigWifi::ReturnToLisk()
{
    if (m_pStackWnd)
    {
        m_pStackWnd->setCurrentIndex(0);
    }
}

void ConfigWifi::InitStackWnd()
{
    if (m_pStackWnd)
    {
        auto pListWnd = MQ(ConfigWifiList)(this);
        auto pConfirmWnd = MQ(ConfigWifiConfirm)(this);

        m_pStackWnd->addWidget(pListWnd);
        m_pStackWnd->addWidget(pConfirmWnd);

        m_pListWnd = pListWnd;
        m_pConfirmWnd = pConfirmWnd;
        connect(m_pListWnd, &ConfigWifiList::LinkClicked, this, &ConfigWifi::StartLinkWifi);
        connect(m_pConfirmWnd, &ConfigWifiConfirm::ConfirmClicked, this, &ConfigWifi::LinkWifi);
        connect(m_pConfirmWnd, &ConfigWifiConfirm::CancellClicked, this, &ConfigWifi::ReturnToLisk);

        if (m_pListWnd && m_pDevNode)
        {
            m_pListWnd->SetWifiData(m_pDevNode->GetWifiList());
        }
    }
}
