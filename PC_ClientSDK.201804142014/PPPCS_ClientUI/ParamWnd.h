#pragma once

#include <QWidget>
#include "ui_ParamWnd.h"
#include "IServer.h"

class ParamWnd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ParamWnd * ;
    ParamWnd(QWidget *parent = Q_NULLPTR);
    ~ParamWnd();
    void BindDevNode(DevNode::Ptr pDevNode);
    void Untie();

    void OnVideoParamData(const std::string& strUid, const IPCNetCamColorCfg_st& stParam);

protected:
    void UpdateData();
    void Applay();
    void Restore();
private:
    Ui::ParamWnd ui;
    ls::IIPCNetServer::Ptr m_pServer = nullptr;
    std::mutex m_mxLockDevNode;
    DevNode::Ptr m_pDevNode = nullptr;
    VideoParamData m_origionalData = {0};
};
