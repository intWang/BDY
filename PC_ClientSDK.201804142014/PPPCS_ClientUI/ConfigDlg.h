#pragma once

#include "BaseDialog.h"
#include "DataStruct.h"
#include <QStackedWidget>
#include "IServer.h"

class ConfigBaseInfoWnd;
class ConfigChangePwd;
class ConfigHotSpot;
class ConfigNetWork;
class ConfigWifi;

class ConfigDlg : public BaseDialog
{
    Q_OBJECT

public:

    enum BtnIDS
    {
        BaseID = -1,
        BaseInfo = 0,
        Wifi,
        HotSpot,
        NetWork,
        ChangePwd,
    };

    ConfigDlg(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigDlg();

    ////on local server call back
    void OnDevTimeData(const std::string& strUId, const IPCNetTimeCfg_st& stData);
    void OnWifiData(const std::string& strUId, const IPCNetWifiAplist::Ptr& pData);
    void OnHotSpotData(const std::string& strUId, const IPCNetWiFiAPInfo_t::Ptr& pData);
    void OnNetStrategyData(const std::string& strUId, const IPCNetNetworkStrategy::Ptr& pData);

signals:
    void BtnSel(BtnIDS emID);

protected slots:
    void OnBtnClicked();
    void OnTabSel(BtnIDS emID);
protected:

    void InitBtnList(QVBoxLayoutPtr pBtnLayout, QWidget* pParent);
    void InitStackWnds();
private:
    DevNode::Ptr m_pDevNode = nullptr;
    QStackedWidget* m_pStackWidget = nullptr;
    std::vector<QPushButtonPtr> m_vcBtns;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;

    ConfigBaseInfoWnd* m_pBaseInfoWnd = nullptr;
    ConfigChangePwd* m_pConfigChangePwd = nullptr;
    ConfigHotSpot* m_pConfigHotSpot = nullptr;
    ConfigNetWork* m_pConfigNetWork = nullptr;
    ConfigWifi* m_pConfigWifi = nullptr;
};
