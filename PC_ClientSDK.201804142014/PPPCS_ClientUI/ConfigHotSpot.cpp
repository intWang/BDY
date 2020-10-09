#include "ConfigHotSpot.h"
#include "MessageBoxWnd.h"
ConfigHotSpot::ConfigHotSpot(DevNode::Ptr pDevNode, QWidget *parent)
    : QWidget(parent)
    , m_pDevNode(pDevNode)
{
    ui.setupUi(this);
    setEnabled(false);
    connect(this, &ConfigHotSpot::DataReady, this, &ConfigHotSpot::RefreshUI);
    connect(ui.btnConfirm, &QPushButton::clicked, this, &ConfigHotSpot::ApplySetting);
    connect(ui.checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        this->ui.lineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    if (m_pDevNode)
    {
        m_pDevNode->GetHotSpot();
    }
}

ConfigHotSpot::~ConfigHotSpot()
{
}

void ConfigHotSpot::SetHotSpotData(IPCNetWiFiAPInfo_t::Ptr pData)
{
    m_pData = pData;
    emit DataReady();
}

void ConfigHotSpot::RefreshUI()
{
    if (m_pData)
    {
        ui.lineEdit_2->setText(QString::fromStdString(m_pData->Ssid));
        ui.lineEdit->setText(QString::fromStdString(m_pData->Pwd));
        ui.chkHideAP->setChecked(!m_pData->HideSSID); //HideSSID":true的话就是关  false 就是开
        setEnabled(true);
    }
}

void ConfigHotSpot::ApplySetting()
{
    std::string ssid = ui.lineEdit_2->text().toStdString();
    std::string pwd = ui.lineEdit->text().toStdString();
    bool bHidden = ui.chkHideAP->isChecked();
    if (m_pData)
    {
        if (m_pData->Ssid != ssid || m_pData->Pwd != pwd|| m_pData->HideSSID != bHidden)
        {
            m_pData->Ssid = ssid;
            m_pData->Pwd = pwd;
            m_pData->HideSSID = bHidden;
            if (m_pDevNode)
            {
                std::string strParam;
                m_pData->toJSONString(strParam);
                m_pDevNode->SetHotSpot(strParam);
            }
        }
        else
        {
            msg::showInformation(this, "提示", "配置无变化，操作无效！");
        }
    }
}
