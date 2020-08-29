#pragma once

#include <QListWidget>
#include "JSONStructProtocal.h"
class ConfigWifiWidget;

class ConfigWifiList : public QListWidget
{
    Q_OBJECT

public:
    ConfigWifiList(QWidget *parent = Q_NULLPTR);
    ~ConfigWifiList();

    void SetWifiData(const IPCNetWifiAplist::Ptr& stData);
    void RefreshList();
    void SetConnectedWif(const IPCNetWirelessConfig_st::Ptr& stData);

signals:
    void LinkClicked(int nIndex);
    void DataReady();
protected:
    void ClearList();
    ConfigWifiWidget* CreateItemWidget(const IPCNetWifiApItem_st& stData, int nIndex);
private:
    IPCNetWifiAplist::Ptr m_pData;
    IPCNetWirelessConfig_st::Ptr m_pConnectedWifi;
};
