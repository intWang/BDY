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

    std::map<std::string, QIcon> m_mapIcon;
    std::map<int, std::vector<int>> m_Group;
    std::mutex m_mxTreeData;
    std::vector<TreeNode::Ptr> m_TreeData;
    QTreeViewPtr m_pTree = nullptr;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;

    QPushButtonPtr m_pAddDeviceBtn = nullptr;
    QPushButtonPtr m_pAddGroupBtn = nullptr;
    QPushButtonPtr m_pDeleteBtn = nullptr;
    QPushButtonPtr m_pSearchBtn = nullptr;
    QPushButtonPtr m_pNextBtn = nullptr;
    QPushButtonPtr m_pLastBtn = nullptr;

    QList<QStandardItem*> m_FindResult;
    int m_nSearchIndex = 0;
protected:
    virtual BarWidget::Ptr InitTopBar() override;
    virtual BarWidget::Ptr InitBottomBar() override;
    QLayoutPtr InitCenterCtl();
    void BuildTree(QStandardItemModelPtr pParent);
    void BuildSubTree(QStandardItemPtr pParent);
    //void BuildDevTree(QStandardItemPtr pParent);
    void AddItemToTree(TreeNode::Ptr pNew, QStandardItemPtr pParent = nullptr);
    void DeleteTreeItem(QStandardItemPtr pItem);
    bool HasChildRunDev(QStandardItemPtr pItem);
    void DeleteGroup(QStandardItemPtr pParent, QStandardItemPtr pItem);
    void DeleteItem(QStandardItemPtr pParent, QStandardItemPtr pItem);
    void SetTotalDevNum(int nNum);
    void SaveTreeData();
    void LoadTreeData();
    QStandardItemPtr GetTreeItem(const QModelIndex &index);
    QStandardItemPtr GetSelItem();
    void SetSelItem(QStandardItemPtr pItem);

    void ClearTree();
    void CallConfigWnd(ConfigType type);

    ////////Tree operate
    void UpdateTreeItem(QString strName, TreeNode::Ptr pNewData);
    void DoSearch(const QString& stKeyWords);
    void SelNext(bool bLastest = false);

    ////////data operate
    std::vector<TreeNode::Ptr> GetTreeIteByGroup(int nGroupID);
    TreeNode::Ptr GetTreeItemByUid(const std::string& strUid);
    TreeNode::Ptr GetTreeItemByNodeId(int nNodeID);
    
    void ClearTreeData();
    void AddTreeData(TreeNode::Ptr pData);
    void RemoveTreeData(TreeNode::Ptr pData);

    ////////device operate
    void ConectDevice(DevNode::Ptr pNode);
    void OnDeviceConnectedCB(const DeviceData& devData);
    void OnDeviceStatuChanged(const std::string& strUid, int nStatus);

signals:
    void LoadedDevNumChange(int nNum);
    void TotalDevNumChange(int nNum);
    void AddNewGroup(const QString& strGooupName, QStandardItemPtr pParent);
    void AddNewDevice(const QString& strDeviceUid, const QString& strDevicePwd, const QString& strDeviceName, QStandardItemPtr pParent);
    void ChannelNodeDBClick(DevNode::Ptr pNodeData);
    void DeviceLostConnect(const QString& strDeviceUid);

public slots:
    void OnDataConfiged(ConfigData::Ptr pData);
    void OnClicked();
    void OnTreeDBClicked(const QModelIndex& index);
    void OnPreviewStatuChanged(const QString& strUid, bool bStatu);
};
