#include "ConfigDlg.h"
#include <QListWidget>
#include <QApplication>
#include "ContainerWnd.h"
#include "ConfigBaseInfoWnd.h"
#include "ConfigChangePwd.h"
#include "ConfigHotSpot.h"
#include "ConfigNetWork.h"
#include "ConfigWifi.h"
ConfigDlg::ConfigDlg(DevNode::Ptr pDevNode, QWidget *parent)
    : BaseDialog(parent)
    , m_pDevNode(pDevNode)
{
     resize(598,586);
     setWindowTitle(QStringLiteral("设置"));
     setMinimizeVisible(false);
     setMaximizeVisible(false);
     setWidgetResizable(false);
     SetArea(30, 0);
     SetBorder(4);
     SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
     SetNoBottomBar();
     auto pMainLayout = GetLayout();
     auto pLayout = MQ(QHBoxLayout)(this);
     auto pBtnWnd = MQ(ContainerWnd)(this);
     auto pBtnListLayout = MQ(QVBoxLayout)(pBtnWnd);

     m_pStackWidget = new QStackedWidget(this);

     InitBtnList(pBtnListLayout, pBtnWnd);
     InitStackWnds();
     pBtnWnd->AddLayout(pBtnListLayout);
     pBtnWnd->SetBkColor(s_qclBK283248);
     pBtnWnd->SetBorder({2,2,2,0}, s_qclBorder2);
 
     pLayout->addWidget(pBtnWnd);
     pLayout->addWidget(m_pStackWidget);
     pLayout->setContentsMargins(4, 0, 4, 4);
     pMainLayout->addLayout(pLayout);


     connect(this, &ConfigDlg::BtnSel, this, &ConfigDlg::OnTabSel);
     emit BtnSel(BaseInfo);

     ///init local server call back
     m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
     if (m_CallBackFunc)
     {
         m_CallBackFunc->funcOnDevTimeData = std::bind(&ConfigDlg::OnDevTimeData, this, std::placeholders::_1, std::placeholders::_2);
         m_CallBackFunc->funcOnSearchWifiData = std::bind(&ConfigDlg::OnWifiData, this, std::placeholders::_1, std::placeholders::_2);
         m_CallBackFunc->funcOnGetHotSpotData = std::bind(&ConfigDlg::OnHotSpotData, this, std::placeholders::_1, std::placeholders::_2);
         m_CallBackFunc->funcOnGetNetStrategyData = std::bind(&ConfigDlg::OnNetStrategyData, this, std::placeholders::_1, std::placeholders::_2);
     }
     auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
     if (pIPCCallBack)
     {
         pIPCCallBack->Register(m_CallBackFunc);
     }
}

ConfigDlg::~ConfigDlg()
{
    auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
    if (pIPCCallBack)
    {
        pIPCCallBack->UnRegister(m_CallBackFunc);
    }
}

void ConfigDlg::OnDevTimeData(const std::string& strUId, const IPCNetTimeCfg_st& stData)
{
    if (m_pDevNode && strUId == m_pDevNode->GetDevUid())
    {
        if (m_pBaseInfoWnd)
        {
            QString strDateTime = QString("%1/%2/%3 %4:%5:%6 GMT%7").arg(stData.Date.Year)
                .arg(stData.Date.Mon).arg(stData.Date.Day).arg(stData.Time.Hour)
                .arg(stData.Time.Min).arg(stData.Time.Sec).arg(stData.TimeZone);
            
            m_pBaseInfoWnd->SetTime(strDateTime);
        }
    }
}

void ConfigDlg::OnWifiData(const std::string& strUId, const IPCNetWifiAplist::Ptr& stData)
{
    if (m_pDevNode && m_pDevNode  && strUId == m_pDevNode->GetDevUid())
    {
        if (m_pConfigWifi)
        {
            m_pConfigWifi->SetWifiData(stData);
        }
    }
}

void ConfigDlg::OnHotSpotData(const std::string& strUId, const IPCNetWiFiAPInfo_t::Ptr& pData)
{
    if (m_pConfigHotSpot && m_pDevNode && strUId == m_pDevNode->GetDevUid())
    {
        m_pConfigHotSpot->SetHotSpotData(pData);
    }
}

void ConfigDlg::OnNetStrategyData(const std::string& strUId, const IPCNetNetworkStrategy::Ptr& pData)
{
    if (m_pConfigNetWork)
    {
        m_pConfigNetWork->SetStrategyData(pData);
    }
}

void ConfigDlg::OnBtnClicked()
{
    if (auto pSender = qobject_cast<QPushButtonPtr>(sender()))
    {
        for (auto pBtn : m_vcBtns)
        {
            if (pBtn == pSender)
            {
                pBtn->setProperty("SelStatu", "On");
            }
            else
            {
                pBtn->setProperty("SelStatu", "Off");
            }
            pBtn->setStyle(QApplication::style());
        }
        auto data = pSender->property("ID");
        emit BtnSel((BtnIDS)data.toInt());
    }
}

void ConfigDlg::OnTabSel(BtnIDS emID)
{
    int nIndex = (int)emID;
    if (m_pStackWidget && nIndex < m_pStackWidget->count())
    {
        m_pStackWidget->setCurrentIndex((int)emID);
    }
}

void ConfigDlg::InitBtnList(QVBoxLayoutPtr pBtnLayout, QWidget* pParent)
{
    QWidgetPtr parent = pParent ? pParent: this;
    auto pBtnBaseInfo = MQ(QPushButton)(parent);
    auto pBtnWifi = MQ(QPushButton)(parent);
    auto pBtnHotSpot = MQ(QPushButton)(parent);
    auto pBtnNetWork = MQ(QPushButton)(parent);
    auto pBtnChangePwd = MQ(QPushButton)(parent);

    m_vcBtns = { pBtnBaseInfo , pBtnWifi , pBtnHotSpot ,pBtnNetWork, pBtnChangePwd };
    std::vector<std::string> vcTexts = {"基本信息", "Wifi设置", "热点设置", "网络设置", "修改密码"};
    int i = 0;
    int nBaseID = BtnIDS::BaseID;
    for (auto pBtn: m_vcBtns)
    {
        pBtnLayout->addWidget(pBtn);
        pBtn->setText(QString::fromStdString(vcTexts[i++]));
        pBtn->setObjectName("ListGroupBtn");
        pBtn->setProperty("ID", ++nBaseID);
        pBtn->setProperty("SelStatu", "Off");
        connect(pBtn, &QPushButton::clicked, this, &ConfigDlg::OnBtnClicked);
    }
    pBtnBaseInfo->setProperty("SelStatu", "On");
    pBtnBaseInfo->setStyle(QApplication::style());
    pBtnLayout->addStretch();
    pBtnLayout->setContentsMargins(0, 0, 0, 0);
    pBtnLayout->setSpacing(0);
}

void ConfigDlg::InitStackWnds()
{
    if (!m_pStackWidget)
    {
        return;
    }
    m_pBaseInfoWnd = new ConfigBaseInfoWnd(m_pDevNode, this);
    m_pConfigChangePwd = new ConfigChangePwd(m_pDevNode, this);
    m_pConfigHotSpot = new ConfigHotSpot(m_pDevNode, this);
    m_pConfigNetWork = new ConfigNetWork(m_pDevNode, this);
    m_pConfigWifi = new ConfigWifi(m_pDevNode, this);
    
    m_pStackWidget->addWidget(m_pBaseInfoWnd);
    m_pStackWidget->addWidget(m_pConfigWifi);
    m_pStackWidget->addWidget(m_pConfigHotSpot);
    m_pStackWidget->addWidget(m_pConfigNetWork);
    m_pStackWidget->addWidget(m_pConfigChangePwd);
}
