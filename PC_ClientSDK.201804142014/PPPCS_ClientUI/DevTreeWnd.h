#pragma once

#include <QWidget>
#include "ui_DevTreeWnd.h"
#include "AreableWidget.h"
#include "IServer.h"
#include "ConfigWidget.h"
#include <map>
#include <mutex>
#include "DataStruct.h"

class DevTreeWnd : public AreableWidget<QWidget>
{
    Q_OBJECT
public:
    using Ptr = DevTreeWnd * ;
    DevTreeWnd(QWidget *parent = Q_NULLPTR);
    ~DevTreeWnd();

private:
    Ui::DevTreeWnd ui;
    int m_nLoadedDevNum = 0;
    int m_nTotalDevNum = 0;

    std::map < std::string, std::string > m_mapConnectedDev;
    std::map<int, std::vector<int>> m_Group;
    std::vector<TreeNode::Ptr> m_TreeData;
    std::mutex m_mxChannelData;
    QTreeViewPtr m_pTree = nullptr;
    QStandardItemPtr m_pCurSelTreeItem = nullptr;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;

    QPushButtonPtr m_pAddDeviceBtn = nullptr;
    QPushButtonPtr m_pAddGroupBtn = nullptr;
    QPushButtonPtr m_pDeleteBtn = nullptr;
protected:
    virtual BarWidget::Ptr InitTopBar() override;
    virtual BarWidget::Ptr InitBottomBar() override;
    QLayoutPtr InitCenterCtl();
    void BuildTree(QStandardItemModelPtr pParent);
    void BuildSubTree(QStandardItemPtr pParent);
    //void BuildDevTree(QStandardItemPtr pParent);
    void AddItemToTree(TreeNode::Ptr pNew, QStandardItemPtr pParent = nullptr);
    void SetTotalDevNum(int nNum);
    void SaveTreeData();
    void LoadTreeData();
    QStandardItemPtr GetTreeItem(const QModelIndex &index);

    void ClearTree();
    void CallConfigWnd(ConfigType type);

    ////////Tree operate
    void UpdateTreeItem(QString strName, TreeNode::Ptr pNewData);

    ////////data operate
    std::vector<TreeNode::Ptr> GetTreeIteByGroup(int nGroupID);
    TreeNode::Ptr GetTreeItemByUid(const std::string& strUid);
    TreeNode::Ptr GetTreeItemByNodeId(int nNodeID);

    ////////device operate
    void ConectDevice(DevNode::Ptr pNode);
    void DisconnectDevice();
    void OnDeviceConnectedCB(const DeviceData& devData);

signals:
    void LoadedDevNumChange(int nNum);
    void TotalDevNumChange(int nNum);
    void AddNewGroup(const QString& strGooupName, QStandardItemPtr pParent);
    void AddNewDevice(const QString& strDeviceUid, const QString& strDevicePwd, const QString& strDeviceName, QStandardItemPtr pParent);
    void ChannelNodeDBClick(ChannelNode::Ptr pNodeData);

protected slots:
    void OnDataConfiged(ConfigData::Ptr pData);
    void OnClicked();
    void OnTreeDBClicked(const QModelIndex& index);
};
