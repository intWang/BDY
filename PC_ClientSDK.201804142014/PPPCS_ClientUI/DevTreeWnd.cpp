#include "DevTreeWnd.h"
#include "QtDefine.h"
#include "IServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QStandardItem>
#include <queue>
#include "utils.h"
#include "IServer.h"
#include "ConfigWidget.h"
#include "MessageBoxWnd.h"
#include "SEP2P_Error.h"
DevTreeWnd::DevTreeWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(45, 45);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);

    m_mapIcon = {
        {"group_b", QIcon(QStringLiteral(":/Black/res/group_b.png"))},
        {"group_g", QIcon(QStringLiteral(":/Black/res/group_g.png"))},
        {"play_b", QIcon(QStringLiteral(":/Black/res/play_b.png"))},
        {"play_g", QIcon(QStringLiteral(":/Black/res/play_g.png"))},
        {"pause_b", QIcon(QStringLiteral(":/Black/res/pause_b.png"))},
        {"pause_g", QIcon(QStringLiteral(":/Black/res/pause_g.png"))},
        {"connecting_b", QIcon(QStringLiteral(":/Black/res/connecting_b.png"))},
        {"connecting_g", QIcon(QStringLiteral(":/Black/res/connecting_g.png"))},
    };

    auto pTopWnd = InitTopBar();
    auto pBottomWnd = InitBottomBar();
    auto pCenterLayout = InitCenterCtl();
   
    ui.mainLayout->addWidget(pTopWnd);
    ui.mainLayout->addLayout(pCenterLayout);
    ui.mainLayout->addWidget(pBottomWnd);
    ui.mainLayout->setSpacing(0);

    m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
    if (m_CallBackFunc)
    {
        m_CallBackFunc->funcOnDeviceConnected = std::bind(&DevTreeWnd::OnDeviceConnectedCB, this, std::placeholders::_1);
        m_CallBackFunc->funcOnDeviceStatuChanged = std::bind(&DevTreeWnd::OnDeviceStatuChanged, this, std::placeholders::_1, std::placeholders::_2);
        m_CallBackFunc->funconGetClarityData = std::bind(&DevTreeWnd::OnStreamInfo, this, std::placeholders::_1, std::placeholders::_2);
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->Register(m_CallBackFunc);
        }
    }

    connect(this, &DevTreeWnd::AddNewGroup, this, [this](const QString& strGroupName, QStandardItemPtr pParent) {
        int nParentID = TREEROOTID;
        if (pParent)
        {
            nParentID = pParent->data().toInt();
        }
        TreeNode::Ptr pData = std::make_shared<GroupNode>(strGroupName.toStdString(), nParentID);
        if (pData)
        {
            this->AddItemToTree(pData, pParent);
        }
    });

    connect(this, &DevTreeWnd::AddNewDevice, this, [this](const QString& strDeviceUid, const QString& strDevicePwd
        , const QString& strDeviceName, QStandardItemPtr pParent) {
        int nParentID = TREEROOTID;
        if (pParent)
        {
            nParentID = pParent->data().toInt();
        }

        auto pCheckDev = GetTreeItemByUid(strDeviceUid.toStdString());
        if (pCheckDev)
        { 
            QString strInfo = QStringLiteral("设备 ") + strDeviceName + "[" + strDeviceUid + "]"
                + QStringLiteral(" 已添加, 本次操作无效！");
            msg::showInformation(this, QStringLiteral("警告"), strInfo);
            return;
        }
        TreeNode::Ptr pData = std::make_shared<DevNode>(strDeviceUid.toStdString(), strDevicePwd.toStdString()
            , strDeviceName.toStdString(), nParentID);
        if (pData)
        {
            pData->SetStatu(DevTreeNodeStatu::Connecting);
            this->AddItemToTree(pData, pParent);
            emit this->TotalDevNumChange(++m_nTotalDevNum);
            ConectDevice(std::dynamic_pointer_cast<DevNode>(pData));
        }
    });

    connect(m_pTree, &QTreeView::doubleClicked, this, &DevTreeWnd::OnTreeDBClicked);

    emit LoadedDevNumChange(0);
}

DevTreeWnd::~DevTreeWnd()
{
    if (m_CallBackFunc)
    {
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->UnRegister(m_CallBackFunc);
        }
    }
    SaveTreeData();
}

/*{
    "dev_info": {
        "name": "ipcam000",
        "p2p_uuid": "BDY00000BNYJEE",
        "app_ver": "2008041945.148",
        "sys_ver": "2008041945.148",
        "pid": "yixin-hi3518e_v200_sdio_wifi_4G_5150",
        "video_input": [{
            "ch_no": 0,
            "audio_code_type": 21,
            "audio_sample_freq": 0,
            "Resl": [{
                "width": 720,
                "height": 576
            }, {
                "width": 352,
                "height": 288
            }],
            "chann_name": "IPCam00001"
        }],
        "HotKey": [],
        "FuncList": []
    }
}*/


BarWidget::Ptr DevTreeWnd::InitTopBar( )
{
    auto pBarWidget = GetTopWnd();
    if (pBarWidget)
    {
        if (auto pTopLayout = pBarWidget->GetLayout())
        {
            auto pLabelLoaded = MQ(QLabel)(this);
            auto pLabelTotal = MQ(QLabel)(this);
            auto pBtnNextPag = MQ(QPushButton)(this);
            auto pBtnLastPag = MQ(QPushButton)(this);
            connect(this, &DevTreeWnd::LoadedDevNumChange, this, [pLabelLoaded](int nLoadedDevNum) {
                if (pLabelLoaded)
                {
                    pLabelLoaded->setText(QString(QStringLiteral("已加载  %1 ")).arg(nLoadedDevNum));
                }
            });

            connect(this, &DevTreeWnd::TotalDevNumChange, this, [pLabelTotal](int nTotalDevNum) {
                if (pLabelTotal)
                {
                    pLabelTotal->setText(QString(QStringLiteral("总数  %1 ")).arg(nTotalDevNum));
                }
            });

            connect(pBtnLastPag, &QPushButton::clicked, this, [this]() {
                SelNext(true);
            });

            connect(pBtnNextPag, &QPushButton::clicked, this, [this]() {
                SelNext();
            });

            pBtnNextPag->setObjectName("btnNext");
            pBtnLastPag->setObjectName("btnLast");
            pTopLayout->addWidget(pLabelLoaded);
            pTopLayout->addWidget(pLabelTotal);
            pTopLayout->addWidget(pBtnNextPag);
            pTopLayout->addWidget(pBtnLastPag);
            pTopLayout->addStretch();
            pTopLayout->setContentsMargins(10, 0, 10, 0);

            m_pNextBtn = pBtnNextPag;
            m_pLastBtn = pBtnLastPag;
            m_pNextBtn->setEnabled(false);
            m_pLastBtn->setEnabled(false);
        }
    }
    return pBarWidget;
}

BarWidget::Ptr  DevTreeWnd::InitBottomBar()
{
    auto pBarWidget = GetBottomWnd();
    if (pBarWidget)
    {
        if (auto pLayout = pBarWidget->GetLayout())
        {
            auto pBtnAddDevice = MQ(QPushButton)(this);
            auto pBtnAddGroup = MQ(QPushButton)(this);
            auto pBtnDelete = MQ(QPushButton)(this);

            pBtnAddDevice->setFixedSize({40,40});
            pBtnAddGroup->setFixedSize({ 40,40 });
            pBtnDelete->setFixedSize({ 40,40 });

            pBtnAddDevice->setObjectName("AddDevice");
            pBtnAddGroup->setObjectName("AddGroup");
            pBtnDelete->setObjectName("DeleteItem");

            pLayout->addStretch();
            pLayout->addWidget(pBtnAddDevice);
            pLayout->addWidget(pBtnAddGroup);
            pLayout->addWidget(pBtnDelete);
            pLayout->setContentsMargins(0,0,20,5);

            connect(pBtnAddDevice, &QPushButton::clicked, this, &DevTreeWnd::OnClicked);
            connect(pBtnAddGroup, &QPushButton::clicked, this, &DevTreeWnd::OnClicked);
            connect(pBtnDelete, &QPushButton::clicked, this, &DevTreeWnd::OnClicked);

            m_pAddDeviceBtn = pBtnAddDevice;
            m_pAddGroupBtn = pBtnAddGroup;
            m_pDeleteBtn = pBtnDelete;
        }
    }
    return pBarWidget;
}

QLayoutPtr DevTreeWnd::InitCenterCtl()
{
    auto pCenterLayout = MQ(QVBoxLayout)(this);
    auto pSubLayout = MQ(QHBoxLayout)(this);

    auto pInput = MQ(QLineEdit)(this);
    auto pBtnSearch = MQ(QPushButton)(this);
    m_pTree = ui.treeView;
    auto model = MQ(QStandardItemModel)(m_pTree);

    pInput->setPlaceholderText(QStringLiteral("请输入搜索名称关键词"));
    pInput->setObjectName("searchInput");
    pInput->setFixedHeight(30);

    pBtnSearch->setObjectName("btnSearch");
    pBtnSearch->setFixedSize(30,30);
    pSubLayout->addWidget(pInput);
    pSubLayout->addWidget(pBtnSearch);
    pCenterLayout->addLayout(pSubLayout);
    pCenterLayout->addWidget(m_pTree);
    pCenterLayout->setContentsMargins(10, 5, 10, 5);
    pCenterLayout->setSpacing(10);
    
    m_pSearchBtn = pBtnSearch;
    connect(pBtnSearch, &QPushButton::clicked, this, [this, pInput]() {
        QString strKeyWord = pInput->text();
        if (strKeyWord.isEmpty())
        {
            msg::showInformation(this, QStringLiteral("提示"), QStringLiteral("请输入关键词查询"));
            return;
        }

        DoSearch(strKeyWord);
    });

    m_pTree->setIconSize({ 20,20 });
    m_pTree->setModel(model);
    m_pTree->setHeaderHidden(true);
    m_pTree->setAutoFillBackground(true);
    BuildTree(model);

    return pCenterLayout;
}

void DevTreeWnd::BuildTree(QStandardItemModelPtr pParent)
{
    if (pParent)
    {
        LoadTreeData();
        auto vcChildren = GetTreeIteByGroup(TREEROOTID);
        for (auto pSubData : vcChildren)
        {
            QStandardItemPtr pSubItem = new QStandardItem(QString::fromStdString(pSubData->GetName()));
            pSubItem->setData(QVariant::fromValue(pSubData->GetNodeID()));
            BuildSubTree(pSubItem);
            pSubItem->setIcon(m_mapIcon["group_g"]);
            pParent->appendRow(pSubItem);
        }

    }
}

void DevTreeWnd::BuildSubTree(QStandardItemPtr pParent)
{
    int nParentID = pParent->data().toInt();
    auto vcChildren = GetTreeIteByGroup(nParentID);
    for (auto pSubData : vcChildren)
    {
        QStandardItemPtr pSubItem = new QStandardItem(QString::fromStdString(pSubData->GetName()));
        pSubItem->setData(QVariant::fromValue(pSubData->GetNodeID()));
        BuildSubTree(pSubItem);
        pParent->appendRow(pSubItem);
        if (pSubData->GetDataType() == DevTreeNodeType::Device)
        {
            pSubItem->setIcon(m_mapIcon["connecting_g"]);
            ConectDevice(std::dynamic_pointer_cast<DevNode>(pSubData));
            ++m_nTotalDevNum;
        }
        else
        {
            pSubItem->setIcon(m_mapIcon["group_g"]);
        }
    }
    emit this->TotalDevNumChange(m_nTotalDevNum);
}

void DevTreeWnd::AddItemToTree(TreeNode::Ptr pNew, QStandardItemPtr pParent /*= nullptr*/)
{
    if (pNew)
    {
        QStandardItemPtr pNewItem = nullptr;
        if (pParent)
        {
            if (pNew->GetDataType() == DevTreeNodeType::Group)
            {
                pNewItem = new QStandardItem(m_mapIcon["group_g"], QString::fromStdString(pNew->GetName()));
                pParent->insertRow(0, pNewItem);
            }
            else
            {
                pNewItem = new QStandardItem(m_mapIcon["connecting_g"], QString::fromStdString(pNew->GetName()));
                pParent->appendRow(pNewItem);
            }
        }
        else
        {
            if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
            {
                pNewItem = new QStandardItem(m_mapIcon["group_g"], QString::fromStdString(pNew->GetName()));
                treeModel->appendRow(pNewItem);
            }
        }

        if (pNewItem)
        {
            SetSelItem(pNewItem);
            pNewItem->setData(QVariant::fromValue(pNew->GetNodeID()));
            AddTreeData(pNew);

        }
    }
}

void DevTreeWnd::DeleteTreeItem(QStandardItemPtr pItem)
{
    int nItemID = pItem->data().toInt();
    auto pTreeItem = GetTreeItemByNodeId(nItemID);
    if (pTreeItem)
    {
        if (pTreeItem->GetDataType() == DevTreeNodeType::Group)
        {
            if (HasChildRunDev(pItem))
            {
                msg::showWarning(this, QStringLiteral("警告"), QStringLiteral("请先停止当前分组下设备预览再执行此操作！"));
                return;
            }
        }
        else if (pTreeItem->GetDataType() == DevTreeNodeType::Device)
        {
            if (pTreeItem->GetStatu() == DevTreeNodeStatu::Play)
            {
                msg::showWarning(this, QStringLiteral("警告"), QStringLiteral("请先停止当前设备预览再执行此操作！"));
                return;
            }
        }
        QStandardItemPtr pParent = pItem->parent();
        DeleteGroup(pParent, pItem);
    }
}

bool DevTreeWnd::HasChildRunDev(QStandardItemPtr pItem)
{
    if (pItem)
    {
        for (auto i = 0; i < pItem->rowCount(); i++)
        {
            auto pSubItem = pItem->child(i);
            if (HasChildRunDev(pSubItem))
            {
                return true;
            }
        }

        auto pTreeData = GetTreeItemByNodeId(pItem->data().toInt());
        if (pTreeData)
        {
            if ((pTreeData->GetDataType() == DevTreeNodeType::Device) && (pTreeData->GetStatu() == DevTreeNodeStatu::Play))
            {
                return true;
            }
        }

    }
    return false;
}

void DevTreeWnd::DeleteGroup(QStandardItemPtr pParent, QStandardItemPtr pItem)
{
    if (pItem)
    {
        for (auto i = 0; i < pItem->rowCount(); i++)
        {
            auto pSubItem = pItem->child(i);
            if (pSubItem->hasChildren())
            {
                DeleteGroup(pItem, pSubItem);
            }
            else
            {
                DeleteItem(pItem, pSubItem);
            }
        }
        DeleteItem(pParent, pItem);
    }
}

void DevTreeWnd::DeleteItem(QStandardItemPtr pParent, QStandardItemPtr pItem)
{
    if (pItem)
    {
        //delete data
        auto pTreeItem = GetTreeItemByNodeId(pItem->data().toInt());
        RemoveTreeData(pTreeItem);
        //delete tree
        if (m_FindResult.contains(pItem))
        {
            m_FindResult.removeAt(m_FindResult.indexOf(pItem));
        }

        if (pParent)
        {
            pParent->removeRow(pItem->row());
        }
        else if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
        {
            treeModel->removeRow(pItem->row());
        }
    }
}

void DevTreeWnd::SetTotalDevNum(int nNum)
{
    emit TotalDevNumChange(nNum);
    m_nTotalDevNum = nNum;
}

void DevTreeWnd::SaveTreeData()
{
    QJsonDocument doc;
    QJsonObject level1;
    QJsonArray treeArray;
    for (auto item : m_Group)
    {
        QJsonObject obj;
        obj.insert("GroupID", item.first);
        QJsonArray children;
        for (auto subGroup : item.second)
        {
            children.append(subGroup);
        }
        obj.insert("SubGroups", children);
        treeArray.append(obj);
    }

    level1.insert("TreeID", treeArray);
    QJsonArray dataArray;
    for (auto item : m_TreeData)
    {
        dataArray.append(item->GenerateJsonObj());
    }
    level1.insert("TreeData", dataArray);

    doc.setObject(level1);
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);

    QString strDBFile = "storage.db";
    QFile dataFile(strDBFile);
    if (!dataFile.open(QIODevice::Truncate |QIODevice::WriteOnly | QIODevice::Text))
    {
        LogError("Open failed %s", strDBFile.toStdString().c_str());
        return ;
    }
    QTextStream out(&dataFile);
    out.setCodec("UTF-8");
    out << byteArray;
    dataFile.flush();
    dataFile.close();

    QString strJson(byteArray);
    LogDebug("Save Data %s", strJson.toStdString().c_str());
}

void DevTreeWnd::LoadTreeData()
{
    QString strDBFile = "storage.db";
    QFile dataFile(strDBFile);
    if (!dataFile.open(QIODevice::ReadOnly))
    {
        LogError("Open failed %s", strDBFile.toStdString().c_str());
        return;
    }
    QByteArray byteArray = dataFile.readAll();
    dataFile.flush();
    dataFile.close();

    ClearTree();
    ClearTreeData();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!doc.isNull() && doc.isObject())
    {
        QJsonObject obj = doc.object();
        QJsonArray groupArray = utils::GetValueFromJsonObj(obj, "TreeID").toArray();
        utils::TravelQJsonArray(groupArray, [this](const QJsonValue& value) {
            if (value.isObject())
            {
                std::pair<int, std::vector<int>> tmpData;
                QJsonObject groupObj = value.toObject();
                int nGroupID = utils::GetValueFromJsonObj(groupObj, "GroupID").toInt();
                tmpData.first = nGroupID;
                auto& vcSubGroup = tmpData.second;
                QJsonArray subGroupArray = utils::GetValueFromJsonObj(groupObj, "SubGroups").toArray();
                utils::TravelQJsonArray(subGroupArray, [&vcSubGroup](const QJsonValue& value) {
                    vcSubGroup.push_back(value.toInt());
                });

                this->m_Group.insert(tmpData);
            }
        });

        QJsonArray dataArray = utils::GetValueFromJsonObj(obj, "TreeData").toArray();
        utils::TravelQJsonArray(dataArray, [this](const QJsonValue& value) {
            if (value.isObject())
            {
                QJsonObject dataObj = value.toObject();
                int nDataType = utils::GetValueFromJsonObj(dataObj, "emNodeType").toInt();
                TreeNode::Ptr pTreeNode = nullptr;
                if (nDataType == DevTreeNodeType::Device)
                {
                    pTreeNode = std::make_shared<DevNode>();
                }
                else if(nDataType == DevTreeNodeType::Group)
                {
                    pTreeNode = std::make_shared<GroupNode>();
                }

                if (pTreeNode)
                {
                    pTreeNode->ReadDataJsonObj(dataObj);
                    this->m_TreeData.push_back(pTreeNode);
                }
            }
        });
    }

}

QStandardItemPtr DevTreeWnd::GetTreeItem(const QModelIndex &index)
{
    if (m_pTree && index.isValid())
    {
        QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model());
        return treeModel->itemFromIndex(index);
    }
    return nullptr;
}

QStandardItemPtr DevTreeWnd::GetSelItem()
{
    if (m_pTree)
    {
        if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
        {
            return treeModel->itemFromIndex(m_pTree->currentIndex());
        }
    }
    return nullptr;
}

void DevTreeWnd::SetSelItem(QStandardItemPtr pItem)
{
    if (m_pTree)
    {
        m_pTree->setCurrentIndex(pItem->index());
    }
}

void DevTreeWnd::ClearTree()
{
    if (m_pTree)
    {
        QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model());
        if (treeModel)
        {
            treeModel->clear();
        }
    }
}

void DevTreeWnd::CallConfigWnd(ConfigType type)
{
    auto pConfigWnd = new ConfigWidget(type);
    connect(pConfigWnd, &ConfigWidget::OnDataConfiged, this, &DevTreeWnd::OnDataConfiged);
    pConfigWnd->setAttribute(Qt::WA_ShowModal);
    pConfigWnd->show();
}

void DevTreeWnd::UpdateTreeItem(QString strName, TreeNode::Ptr pNewData)
{
    if (m_pTree)
    {
        if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
        {
            auto findResult = treeModel->findItems(strName, Qt::MatchContains | Qt::MatchRecursive);
            for (auto item: findResult)
            {
                int nID = item->data().toInt();
                if (nID == pNewData->GetNodeID())
                {
                    //////update tree ui
                    if (pNewData->GetStatu() == DevTreeNodeStatu::Pause)
                    {
                        item->setIcon(m_mapIcon["play_g"]);
                    }
                    else if(pNewData->GetStatu() == DevTreeNodeStatu::Play)
                    {
                        item->setIcon(m_mapIcon["pause_g"]);
                    }
                    else if (pNewData->GetStatu() == DevTreeNodeStatu::Connecting)
                    {
                        item->setIcon(m_mapIcon["connecting_g"]);
                    }

                    m_pTree->update();
                }
            }
        }
    }
}

void DevTreeWnd::DoSearch(const QString& stKeyWords)
{
    m_FindResult.clear();
    if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
    {
        auto findResult = treeModel->findItems(stKeyWords, Qt::MatchContains | Qt::MatchRecursive);
        m_FindResult.swap(findResult);
    }

    if (m_FindResult.isEmpty())
    {
        msg::showInformation(this, QStringLiteral("信息"), QStringLiteral("查询结果为空！"));
        m_pNextBtn->setEnabled(false);
        m_pLastBtn->setEnabled(false);
    }
    else
    {
        SelNext();
        m_pNextBtn->setEnabled(true);
        m_pLastBtn->setEnabled(true);
    }
}

void DevTreeWnd::SelNext(bool bLastest /*= false*/)
{
    if (m_FindResult.isEmpty())
    {
        msg::showInformation(this, QStringLiteral("信息"), QStringLiteral("查询结果为空！"));
        m_pNextBtn->setEnabled(false);
        m_pLastBtn->setEnabled(false);
        return;
    }

    QStandardItem* pCurItem = nullptr;
    if (bLastest)
    {
        m_nSearchIndex = m_FindResult.size() - 1;
    }
    if (m_nSearchIndex < m_FindResult.size() && m_nSearchIndex >= 0)
    {
        pCurItem = m_FindResult.at(m_nSearchIndex);
        m_nSearchIndex++;
    }
    else
    {
        pCurItem = m_FindResult.at(0);
        m_nSearchIndex = 1;
    }

    if (m_pTree)
    {
        m_pTree->expand(pCurItem->index());
        SetSelItem(pCurItem);
    }
}

std::vector<TreeNode::Ptr> DevTreeWnd::GetTreeIteByGroup(int nGroupID)
{
    std::lock_guard<std::mutex> guard(m_mxTreeData);
    std::vector<TreeNode::Ptr> ret;
    for(auto item : m_TreeData)
    { 
        if (item && item->GetParent() == nGroupID)
        {
            ret.push_back(item);
        }
    }

    std::sort(ret.begin(), ret.end(), [](TreeNode::Ptr pA, TreeNode::Ptr pB) {
        return pA->emNodeType < pB->emNodeType;
    });

    return ret;
}

TreeNode::Ptr DevTreeWnd::GetTreeItemByUid(const std::string& strUid)
{
    std::lock_guard<std::mutex> guard(m_mxTreeData);
    TreeNode::Ptr pSearch = nullptr;
    utils::TravelVector(m_TreeData, [&strUid, &pSearch](TreeNode::Ptr pData) {
        if (pData->GetDataType() == DevTreeNodeType::Device)
        {
            auto pDev = std::dynamic_pointer_cast<DevNode>(pData);
            if (pDev && pDev->strUID == strUid)
            {
                pSearch = pData;
                return true;
            }
        }
        return false;
    });
    return pSearch;
}

TreeNode::Ptr DevTreeWnd::GetTreeItemByNodeId(int nNodeID)
{
    std::lock_guard<std::mutex> guard(m_mxTreeData);
    TreeNode::Ptr pSearch = nullptr;
    utils::TravelVector(m_TreeData, [&nNodeID, &pSearch](TreeNode::Ptr pData) {
        if (pData && pData->GetNodeID() == nNodeID)
        {
            pSearch = pData;
            return true;
        }
        return false;
    });
    return pSearch;
}

void DevTreeWnd::ClearTreeData()
{
    std::lock_guard<std::mutex> guard(m_mxTreeData);
    m_Group.clear();
    m_TreeData.clear();
    m_nTotalDevNum = 0;
    m_nLoadedDevNum = 0;
}

void DevTreeWnd::AddTreeData(TreeNode::Ptr pNew)
{
    std::lock_guard<std::mutex> guard(m_mxTreeData);
    m_Group[pNew->GetParent()].push_back(pNew->GetNodeID());
    m_TreeData.push_back(pNew);
}

void DevTreeWnd::RemoveTreeData(TreeNode::Ptr pData)
{
    if (pData)
    {
        std::lock_guard<std::mutex> guard(m_mxTreeData);
        auto& vcSlibing = m_Group[pData->GetParent()];
        auto iterFind = std::find(vcSlibing.begin(), vcSlibing.end(), pData->GetNodeID());
        if (iterFind != vcSlibing.end())
        {
            vcSlibing.erase(iterFind);
        }

        auto iterFind1 = std::find(m_TreeData.begin(), m_TreeData.end(), pData);
        if (iterFind1 != m_TreeData.end())
        {
            auto pData = *iterFind1;
            if (pData->GetDataType() == DevTreeNodeType::Device)
            {
                emit TotalDevNumChange(--m_nTotalDevNum);
                if (pData->GetStatu() == DevTreeNodeStatu::Pause)
                {
                    emit LoadedDevNumChange(--m_nLoadedDevNum);
                }
            }
            m_TreeData.erase(iterFind1);
        }
    }
}

void DevTreeWnd::ConectDevice(DevNode::Ptr pNode)
{
    pNode->Conect();
}

void DevTreeWnd::OnDeviceConnectedCB(const DeviceData& devData)
{
    TreeNode::Ptr pTreeNode = GetTreeItemByUid(devData.p2p_uuid);
    if (pTreeNode)
    {
        auto pDevNode = std::dynamic_pointer_cast<DevNode>(pTreeNode);
        if (pDevNode && !pDevNode->IsDevLoaded())
        {
            std::string strOldName = pDevNode->GetName();
            pDevNode->UpdateDevData(devData);
            pDevNode->SetStatu(Pause);
            UpdateTreeItem(QString::fromStdString(strOldName), pDevNode);
            emit LoadedDevNumChange(++m_nLoadedDevNum);
        }
    }
}

void DevTreeWnd::OnDeviceStatuChanged(const std::string& strUid, int nStatus)
{
    if (auto pDevNode = std::dynamic_pointer_cast<DevNode>(GetTreeItemByUid(strUid)))
    {
        switch (nStatus)
        {
        case ERROR_SEP2P_STATUS_CONNECTION_LOST:
        {
            if (pDevNode->IsDevLoaded())
            {
                pDevNode->OnLostConnect();
                UpdateTreeItem(QString::fromStdString(pDevNode->GetName()), pDevNode);
                emit LoadedDevNumChange(--m_nLoadedDevNum);
                emit DeviceLostConnect(QString::fromStdString(pDevNode->GetDevUid()));
            }
            break;
        }
        case CSTATU_RECONNECTED:
        {
            if (pDevNode->IsDevLoaded())
            {
                pDevNode->OnReConnect();
                UpdateTreeItem(QString::fromStdString(pDevNode->GetName()), pDevNode);
            }
            break;
        }
        default:
            break;
        }
    }
}

void DevTreeWnd::OnStreamInfo(const std::string& strUid, const IPCNetStreamInfo::Ptr& pData)
{
    auto pDevNode = std::dynamic_pointer_cast<DevNode>(GetTreeItemByUid(strUid));
    if (pDevNode)
    {
        pDevNode->SetStreamData(pData);
    }
}

void DevTreeWnd::OnDataConfiged(ConfigData::Ptr pData)
{
    if (pData)
    {
        auto pParentItem = GetTreeItem(m_pTree->currentIndex());
        if (pParentItem)
        {
            int nParentID = pParentItem->data().toInt();
            if (GETGROUPID(nParentID) != 0)
            {
                pParentItem = pParentItem->parent();
            }
        }
        switch (pData->emType)
        {
        case ConfigType::AddDevice:
        {
             auto pRealData = std::dynamic_pointer_cast<AddDeviceData>(pData);
             emit AddNewDevice(QString::fromStdString(pRealData->strUID), QString::fromStdString(pRealData->strPwd)
                 , QString::fromStdString(pRealData->strDevName), pParentItem);
        }
            break;
        case ConfigType::AddGroup:
        {
             auto pRealData = std::dynamic_pointer_cast<AddGroupData>(pData);
             emit AddNewGroup(QString::fromStdString(pRealData->strGroupName), pParentItem);
        }
            break;
        default:
            break;
        }
    }
}

void DevTreeWnd::OnClicked()
{
    QPushButtonPtr pButton = qobject_cast<QPushButton *>(sender());

    if (pButton == m_pAddDeviceBtn)
    {
        auto pSelItem = GetSelItem();
        if (!pSelItem)
        {
            if (g_pEngine)
            {
                auto pHintServer = g_pEngine->GetHintServer();
                if (pHintServer)
                {
                    pHintServer->OnUserOperateHint("请选择一个分组再添加设备!", ls::HintLevel::Info);
                }
            }
        }
        else
        {
            this->CallConfigWnd(ConfigType::AddDevice);
        }
    }
    else if (pButton == m_pAddGroupBtn)
    {
        this->CallConfigWnd(ConfigType::AddGroup);
    }
    else if(pButton == m_pDeleteBtn)
    {
        auto pSelItem = GetSelItem();
        if (!pSelItem)
        {
            msg::showInformation(this, QStringLiteral("信息"), QStringLiteral("请选择要删除的设备或分组！"));
            return;
        }
        if (msg::showQuestion(this, QStringLiteral("请确认"), QStringLiteral("确认删除吗?")) == QMessageBox::Ok)
        {
            DeleteTreeItem(pSelItem);
        }
    }
}

void DevTreeWnd::OnTreeDBClicked(const QModelIndex& index)
{
    auto pSelItem = GetTreeItem(index);
    if (pSelItem)
    {
        int nNodeID = pSelItem->data().toInt();
        if (auto pNode = GetTreeItemByNodeId(nNodeID))
        {
            switch (pNode->GetDataType())
            {
            case DevTreeNodeType::Device:
            {
                auto pDevNode = std::dynamic_pointer_cast<DevNode>(pNode);
                if (pDevNode && (pDevNode->GetStatu() != DevTreeNodeStatu::Default) && (pDevNode->GetStatu() != DevTreeNodeStatu::Connecting))
                {
                    emit ChannelNodeDBClick(pDevNode);
                }
                else
                {
                    msg::showWarning(this, QStringLiteral("警告"), QStringLiteral("设备未加载，请稍后再试！"));
                }
            }
            break;
            default:
                break;
            }
        }
    }
}

void DevTreeWnd::OnPreviewStatuChanged(const QString& strUid, bool bStatu)
{
    if (auto pTreeItem = GetTreeItemByUid(strUid.toStdString()))
    {
        UpdateTreeItem(QString::fromStdString(pTreeItem->GetName()), pTreeItem);
    }
}
