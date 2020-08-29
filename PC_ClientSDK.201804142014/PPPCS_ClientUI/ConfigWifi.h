#pragma once

#include <QWidget>
#include "DataStruct.h"
#include "JSONStructProtocal.h"
#include <QStackedWidget>

class ConfigWifiList;
class ConfigWifiConfirm;

class ConfigWifi : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ConfigWifi * ;
    ConfigWifi(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigWifi();

    void SetWifiData(const IPCNetWifiAplist::Ptr& stData);
    void SetConnectedWif(const IPCNetWirelessConfig_st::Ptr& stData);

public slots:
    void StartLinkWifi(int nIndex);
    void LinkWifi(QString strPwd);
    void ReturnToLisk();
private:
    void InitStackWnd();
private:
    IPCNetWifiAplist::Ptr m_pWifiData = nullptr;;
    DevNode::Ptr m_pDevNode = nullptr;
    QStackedWidget* m_pStackWnd = nullptr;
    ConfigWifiList* m_pListWnd = nullptr;
    ConfigWifiConfirm* m_pConfirmWnd = nullptr;
    int m_nCurrentSelWifi = -1;
};
