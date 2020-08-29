#pragma once

#include <QWidget>
#include "ui_ConfigHotSpot.h"
#include "DataStruct.h"
class ConfigHotSpot : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ConfigHotSpot * ;
    ConfigHotSpot(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigHotSpot();

    void SetHotSpotData(IPCNetWiFiAPInfo_t::Ptr pData);
signals:
    void DataReady();

protected slots:
    void RefreshUI();
    void ApplySetting();
private:
    Ui::ConfigHotSpot ui;
    DevNode::Ptr m_pDevNode = nullptr;
    IPCNetWiFiAPInfo_t::Ptr m_pData = nullptr;
};
