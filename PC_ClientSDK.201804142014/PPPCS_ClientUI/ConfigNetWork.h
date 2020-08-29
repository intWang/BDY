#pragma once

#include <QWidget>
#include "ui_ConfigNetWork.h"
#include "DataStruct.h"
class ConfigNetWork : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ConfigNetWork * ;
    ConfigNetWork(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigNetWork();
    void SetStrategyData(const IPCNetNetworkStrategy::Ptr& pNetStrategy);

signals:
    void DataReady();

protected slots:
    void RefreshUI();
    void ApplySetting();
    void OnRestartDev();
    void OnCheckStatuChanged();
private:
    Ui::ConfigNetWork ui;
    DevNode::Ptr m_pDevNode = nullptr;
    IPCNetNetworkStrategy_st::Ptr m_pNetStrategy = nullptr;
};
