#include "ConfigBaseInfoWnd.h"
#include "MessageBoxWnd.h"
ConfigBaseInfoWnd::ConfigBaseInfoWnd(DevNode::Ptr pDevNode, QWidget *parent)
    : QWidget(parent)
    , m_pDevNode(pDevNode)
{
    ui.setupUi(this);

    if (m_pDevNode)
    {
        ui.lblDevName->setText(QString::fromStdString(m_pDevNode->GetName()));
        ui.lblUUID->setText(QString::fromStdString(m_pDevNode->GetDevUid()));
        m_pDevNode->GetDevTime();

        connect(ui.btnResatrtDev, &QPushButton::clicked, this, &ConfigBaseInfoWnd::OnBtnClicked);
        connect(ui.btnRestoreSet, &QPushButton::clicked, this, &ConfigBaseInfoWnd::OnBtnClicked);
    }

}

ConfigBaseInfoWnd::~ConfigBaseInfoWnd()
{
}

void ConfigBaseInfoWnd::SetTime(const QString& strTime)
{
    ui.lblDevTime->setText(strTime);
}

void ConfigBaseInfoWnd::OnBtnClicked()
{
    auto pSender = qobject_cast<QPushButtonPtr>(sender());
    if (pSender == ui.btnResatrtDev)
    {
        if (msg::showQuestion(this, QStringLiteral("请确认"), QStringLiteral("重启设备会使设备断开连接，是否执行？")) == QMessageBox::Ok)
        {
            m_pDevNode->RestartDevice();
        }
    }
    else if (pSender == ui.btnRestoreSet)
    {
        if (msg::showQuestion(this, QStringLiteral("请确认"), QStringLiteral("恢复出厂设置会使设备断开连接，是否执行？")) == QMessageBox::Ok)
        {
            m_pDevNode->RestoreDevice();
        }
    }
}
