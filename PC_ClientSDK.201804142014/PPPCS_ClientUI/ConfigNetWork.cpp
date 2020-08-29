#include "ConfigNetWork.h"
#include "MessageBoxWnd.h"
#define  PORI_WIFI "WiFi"
#define  PORI_WIRED "Wired"
#define  PORI_MOBILE "Mobile"

ConfigNetWork::ConfigNetWork(DevNode::Ptr pDevNode, QWidget *parent)
    : QWidget(parent)
    , m_pDevNode(pDevNode)
{
    ui.setupUi(this);
    connect(this, &ConfigNetWork::DataReady, this, &ConfigNetWork::RefreshUI);
    connect(ui.chkMobile, &QCheckBox::clicked, this, &ConfigNetWork::OnCheckStatuChanged);
    connect(ui.chkWifi, &QCheckBox::clicked, this, &ConfigNetWork::OnCheckStatuChanged);
    connect(ui.btnApply, &QPushButton::clicked, this, &ConfigNetWork::ApplySetting);
    connect(ui.btnRestartDev, &QPushButton::clicked, this, &ConfigNetWork::OnRestartDev);
    if (m_pDevNode)
    {
        m_pDevNode->GetNetStrategy();
    }
}

ConfigNetWork::~ConfigNetWork()
{
}

void ConfigNetWork::SetStrategyData(const IPCNetNetworkStrategy::Ptr& pNetStrategy)
{
    m_pNetStrategy = pNetStrategy;
    emit DataReady();
}

void ConfigNetWork::RefreshUI()
{
    if (m_pNetStrategy)
    {
        ui.lblCurrent->setText(QString::fromStdString(m_pNetStrategy->CurNetwork));
        for (int i = 0 ;i< m_pNetStrategy->PrioSeqNum; i++)
        {
            if (m_pNetStrategy->PrioSeq[i] == PORI_WIFI)
            {
                ui.chkWifi->setChecked(true);
                break;
            }

            if (m_pNetStrategy->PrioSeq[i] == PORI_MOBILE)
            {
                ui.chkMobile->setChecked(true);
                break;
            }
        }
    }
}

void ConfigNetWork::ApplySetting()
{
    if (m_pDevNode && m_pNetStrategy)
    {
        std::string strTmpPori[3];
        if (ui.chkMobile->isChecked())
        {
            strTmpPori[0] = PORI_MOBILE;
            strTmpPori[1] = PORI_WIFI;
            strTmpPori[2] = PORI_WIRED;
        }
        else
        {
            strTmpPori[0] = PORI_WIFI;
            strTmpPori[1] = PORI_MOBILE;
            strTmpPori[2] = PORI_WIRED;
        }

        bool bDiff = false;
        for (int i = 0; i < 3; i++)
        {
            if (strTmpPori[i] != m_pNetStrategy->PrioSeq[i])
            {
                m_pNetStrategy->PrioSeq[i] = strTmpPori[i];
                bDiff = true;
            }
        }

        if (bDiff)
        {
            m_pNetStrategy->PrioSeqNum = 3;
            std::string strParm;
            m_pNetStrategy->toJSONString(strParm);
            m_pDevNode->SetNetStrategy(strParm);
        }
        else
        {
            msg::showInformation(this, "提示", "策略无变化，操作无效！");
        }
    }
}

void ConfigNetWork::OnRestartDev()
{
    if (msg::showQuestion(this, QStringLiteral("请确认"), QStringLiteral("重启设备会使设备断开连接，是否执行？")) == QMessageBox::Ok)
    {
        m_pDevNode->RestartDevice();
    }
}

void ConfigNetWork::OnCheckStatuChanged()
{
    auto pChk = qobject_cast<QCheckBoxPtr>(sender());
    if ((pChk == ui.chkMobile) && pChk->isChecked())
    {
        ui.chkWifi->setChecked(false);
    }
    if ((pChk == ui.chkWifi) && (pChk->isChecked()))
    {
        ui.chkMobile->setChecked(false);
    }
}
