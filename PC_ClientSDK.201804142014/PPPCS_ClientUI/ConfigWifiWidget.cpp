#include "ConfigWifiWidget.h"

ConfigWifiWidget::ConfigWifiWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setFixedHeight(60);
    connect(ui.btnlink, &QPushButton::clicked, this, [this]() {
        emit this->OnBtnClick(nIndex);
    });
}

ConfigWifiWidget::~ConfigWifiWidget()
{
}

void ConfigWifiWidget::SetItemData(const IPCNetWifiApItem_st& stData, int index)
{
    ui.lblWifiName->setText(QString::fromStdString(stData.SSID));
    ui.lblSafety->setText(QString::fromStdString(stData.EncType));
    ui.lblStrange->setText(QVariant::fromValue(stData.RSSI).toString());
    if (stData.Flag)
    {
        ui.btnlink->setText("已连接");
        ui.btnlink->setEnabled(false);
    }
    nIndex = index;
}
