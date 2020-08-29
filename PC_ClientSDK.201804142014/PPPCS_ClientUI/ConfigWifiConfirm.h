#pragma once

#include <QWidget>
#include "ui_ConfigWifiConfirm.h"
#include "DataStruct.h"
#include "JSONStructProtocal.h"

class ConfigWifiConfirm : public QWidget
{
    Q_OBJECT

public:
    ConfigWifiConfirm( QWidget *parent = Q_NULLPTR);
    ~ConfigWifiConfirm();

signals:
    void ConfirmClicked(QString strPwd);
    void CancellClicked();
private:
    Ui::ConfigWifiConfirm ui;
    DevNode::Ptr m_pDevData = nullptr;
};
