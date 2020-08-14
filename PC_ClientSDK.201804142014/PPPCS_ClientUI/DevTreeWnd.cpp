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

DevTreeWnd::DevTreeWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(45, 45);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);

    auto pTopWnd = InitTopBar();
    auto pBottomWnd = InitBottomBar();
    auto pCenterLayout = InitCenterCtl();
   
    ui.mainLayout->addWidget(pTopWnd);
    ui.mainLayout->addLayout(pCenterLayout);
    ui.mainLayout->addWidget(pBottomWnd);

    m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
    if (m_CallBackFunc)
    {
        m_CallBackFunc->funcOnDeviceConnected = std::bind(&DevTreeWnd::OnDeviceConnectedCB, this, std::placeholders::_1);
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->Register(m_CallBackFunc);
        }
    }

    connect(this, &DevTreeWnd::OnAddNewGroup, this, [this](const QString& strGroupName, QStandardItemPtr pParent) {
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

    connect(this, &DevTreeWnd::OnAddNewDevice, this, [this](const QString& strDeviceUid, const QString& strDevicePwd, QStandardItemPtr pParent) {
        int nParentID = TREEROOTID;
        if (pParent)
        {
            nParentID = pParent->data().toInt();
        }
        TreeNode::Ptr pData = std::make_shared<DevNode>(strDeviceUid.toStdString(), strDevicePwd.toStdString(), nParentID);
        if (pData)
        {
            this->AddItemToTree(pData, pParent);
        }
    });


    connect(m_pTree, &QTreeView::pressed, this, [this](const QModelIndex &index) {
        this->m_pCurSelTreeItem = this->GetTreeItem(index);
    });

    emit OnLoadedDevNumChange(0);
    emit OnTotalDevNumChange(m_nTotalDevNum);
}

DevTreeWnd::~DevTreeWnd()
{
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

            connect(this, &DevTreeWnd::OnLoadedDevNumChange, this, [pLabelLoaded](int nLoadedDevNum) {
                if (pLabelLoaded)
                {
                    pLabelLoaded->setText(QString(QStringLiteral("已加载  %1 ")).arg(nLoadedDevNum));
                }
            });

            connect(this, &DevTreeWnd::OnTotalDevNumChange, this, [pLabelTotal](int nTotalDevNum) {
                if (pLabelTotal)
                {
                    pLabelTotal->setText(QString(QStringLiteral("总数  %1 ")).arg(nTotalDevNum));
                }
            });
            pTopLayout->addWidget(pLabelLoaded);
            pTopLayout->addWidget(pLabelTotal);
            pTopLayout->addStretch();
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

            pBtnAddDevice->setText(QStringLiteral("新增设备"));
            pBtnAddGroup->setText(QStringLiteral("新增分组"));
            pBtnDelete->setText(QStringLiteral("删除"));
            pLayout->addStretch();
            pLayout->addWidget(pBtnAddDevice);
            pLayout->addWidget(pBtnAddGroup);
            pLayout->addWidget(pBtnDelete);

            connect(pBtnAddDevice, &QPushButton::clicked, this, &DevTreeWnd::OnClicked);
            connect(pBtnAddGroup, &QPushButton::clicked, this, &DevTreeWnd::OnClicked);

            m_pAddDeviceBtn = pBtnAddDevice;
            m_pAddGroupBtn = pBtnAddGroup;
        }
    }
    return pBarWidget;
}

QLayoutPtr DevTreeWnd::InitCenterCtl()
{
    auto pCenterLayout = MQ(QVBoxLayout)(this);
    auto pSubLayout = MQ(QHBoxLayout)(this);

    auto pInout = MQ(QLineEdit)(this);
    auto pBtnSearch = MQ(QPushButton)(this);
    m_pTree = MQ(QTreeView)(this);
    auto model = MQ(QStandardItemModel)(m_pTree);

    pInout->setFixedHeight(30);
    pBtnSearch->setFixedSize(50,30);
    pSubLayout->addWidget(pInout);
    pSubLayout->addWidget(pBtnSearch);
    pCenterLayout->addLayout(pSubLayout);
    pCenterLayout->addWidget(m_pTree);
    pCenterLayout->setContentsMargins(10, 5, 10, 5);
    pCenterLayout->setSpacing(10);

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
            m_nTotalDevNum++;
            ConectDevice(std::dynamic_pointer_cast<DevNode>(pSubData));
        }
    }
}

void DevTreeWnd::AddItemToTree(TreeNode::Ptr pNew, QStandardItemPtr pParent /*= nullptr*/)
{
    if (pNew)
    {
        QStandardItemPtr pNewItem = new QStandardItem(QString::fromStdString(pNew->GetName()));
        pNewItem->setData(QVariant::fromValue(pNew->GetNodeID()));
        if (pParent)
        {
            pParent->appendRow(pNewItem);
        }
        else
        {
            if (QStandardItemModel* treeModel = dynamic_cast<QStandardItemModel*>(m_pTree->model()))
            {
                treeModel->appendRow(pNewItem);
            }
        }

        m_Group[pNew->GetParent()].push_back(pNew->GetNodeID());
        m_TreeData.push_back(pNew);
    }
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
    QByteArray byteArray;
    QTextStream out(&dataFile);
    out.setCodec("UTF-8");
    out >> byteArray;
    dataFile.flush();
    dataFile.close();

    m_Group.clear();
    m_TreeData.clear();
    m_nTotalDevNum = 0;
    m_nLoadedDevNum = 0;

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

    m_TreeData.clear();
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
            auto findResult = treeModel->findItems(strName);
            for (auto item: findResult)
            {
                int nID = item->data().toInt();
                if (nID == pNewData->GetNodeID())
                {
                    item->setText(QString::fromStdString(pNewData->GetName()));
                }
            }
        }
    }
}

std::vector<TreeNode::Ptr> DevTreeWnd::GetTreeIteByGroup(int nGroupID)
{
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

void DevTreeWnd::ConectDevice(DevNode::Ptr pNode)
{
    auto pICPServer = g_pEngine?g_pEngine->GetIPCNetServer():nullptr;
    if (pICPServer)
    {
        pICPServer->ConnectDevice(pNode->strUID, pNode->strPwd);
    }
}

void DevTreeWnd::DisconnectDevice()
{
    for (auto devUid: m_mapConnectedDev)
    {

    }
}

void DevTreeWnd::OnDeviceConnectedCB(const DeviceData& devData)
{
    TreeNode::Ptr pTreeNode = GetTreeItemByUid(devData.p2p_uuid);
    if (pTreeNode)
    {
        auto pDevNode = std::dynamic_pointer_cast<DevNode>(pTreeNode);
        if (pDevNode)
        {
            std::string strOldName = pDevNode->GetName();
            pDevNode->UpdateDevData(devData);

            UpdateTreeItem(QString::fromStdString(strOldName), pDevNode);
            if (m_mapConnectedDev.find(pDevNode->strUID) == m_mapConnectedDev.end())
            {
                m_mapConnectedDev[pDevNode->strUID] = pDevNode->stDevice.name;
                m_nLoadedDevNum++;
                emit OnLoadedDevNumChange(m_nLoadedDevNum);
            }
        }
    }
}

void DevTreeWnd::OnDataConfiged(ConfigData::Ptr pData)
{
    if (pData)
    {
        auto pParentItem = m_pCurSelTreeItem;
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
             emit OnAddNewDevice(QString::fromStdString(pRealData->strUID), QString::fromStdString(pRealData->strPwd), pParentItem);
        }
            break;
        case ConfigType::AddGroup:
        {
             auto pRealData = std::dynamic_pointer_cast<AddGroupData>(pData);
             emit OnAddNewGroup(QString::fromStdString(pRealData->strGroupName), pParentItem);
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
        if (!m_pCurSelTreeItem)
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
}
