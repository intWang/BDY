#include "DataStruct.h"
#include "utils.h"
#include "IServer.h"
int GroupNode::s_GroupCount = BASEID;
int DevNode::s_nDevCount = BASEID;

int TreeNode::GetNodeID()
{
    return nNodeId;
}

int TreeNode::GetParent()
{
    return nParentId;
}

DevTreeNodeType TreeNode::GetDataType()
{
    return emNodeType;
}

void TreeNode::SetStatu(DevTreeNodeStatu _emStatu)
{
    emStatu = _emStatu;
}

DevTreeNodeStatu TreeNode::GetStatu()
{
    return emStatu;
}

std::string TreeNode::GetStatuProperty()
{
    std::string strRet = "";
    switch (emStatu)
    {
    case Default:
        break;
    case Connecting:
        strRet = "Connecting";
        break;
    case Pause:
        strRet = "Pause";
        break;
    case Play:
        strRet = "Play";
        break;
    default:
        break;
    }
    return strRet;
}

DevNode::DevNode(const std::string& strUid, const std::string& strPwd
    , const std::string& strName, const std::string& _strShortID, int nGroupID)
    : strUID(strUid)
    , strPwd(strPwd)
    , strCustomName(strName)
    , strShortID(_strShortID)
{
    emNodeType = DevTreeNodeType::Device;
    nParentId = nGroupID;
    nNodeId = MAKEDEVID(nGroupID, ++s_nDevCount);
    nPreviewCount = 0;
}

DevNode::~DevNode()
{
    DisConnect();
}

std::string DevNode::GetName()
{
    std::string labelName = strCustomName;
    std::string labelID = strShortID.size() ? strShortID : strUID;
    if (labelName != labelID)
    {
        labelName = labelName + "(" + labelID + ")";
    }
    return labelName;
}

QJsonObject DevNode::GenerateJsonObj()
{
    QJsonObject obj;
    obj.insert("nNodeId", nNodeId);
    obj.insert("nParentId", nParentId);
    obj.insert("emNodeType", emNodeType);
    obj.insert("strCustomName", QString::fromStdString(strCustomName));
    obj.insert("strUID", QString::fromStdString(strUID));
    obj.insert("strPwd", QString::fromStdString(strPwd));
    obj.insert("strShortID", QString::fromStdString(strShortID));
    //TODO
    return obj;
}

void DevNode::ReadDataJsonObj(QJsonObject& obj)
{
    nNodeId = utils::GetValueFromJsonObj(obj, "nNodeId").toInt();
    nParentId = utils::GetValueFromJsonObj(obj, "nParentId").toInt();
    emNodeType = (DevTreeNodeType)utils::GetValueFromJsonObj(obj, "emNodeType").toInt();
    strCustomName = utils::GetValueFromJsonObj(obj, "strCustomName").toString().toStdString();
    strUID = utils::GetValueFromJsonObj(obj, "strUID").toString().toStdString();
    strPwd = utils::GetValueFromJsonObj(obj, "strPwd").toString().toStdString();
    strShortID = utils::GetValueFromJsonObj(obj, "strShortID").toString().toStdString();

    s_nDevCount = max(GETDEVID(nNodeId), s_nDevCount);
}

void DevNode::UpdateDevData(const DeviceData& data)
{
    stDevice = data;
}

bool DevNode::IsDevLoaded()
{
    return !stDevice.video_input.empty();
}

bool DevNode::CheckPwd(std::string strInputPwd)
{
    return strInputPwd == strPwd;
}

std::string DevNode::GetDevUid()
{
    return strUID;
}

std::string DevNode::GetLabelUid()
{
    return strShortID;
}

bool DevNode::GetDevTime()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->GetDevTime(strUID);
        return true;
    }
    return false;
}

bool DevNode::RestartDevice()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->RestartDevice(strUID);
        return true;
    }

    return false;
}

bool DevNode::RestoreDevice()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->ResetDevice(strUID);
        return true;
    }
    return false;
}

bool DevNode::SearchWifi()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SearchWifi(strUID);
        return true;
    }
    return false;
}

bool DevNode::SetWifi(std::string& strSSID, std::string& strPwd, std::string& strEncType)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SetWifi(strUID, strSSID, strPwd, strEncType);
        return true;
    }
    return false;
}

bool DevNode::ChangePwd(std::string strNewPwd)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->ChangeDevPwd(strUID,strNewPwd);
        strPwd = strNewPwd;
        return true;
    }
    return false;
}

bool DevNode::GetHotSpot()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->GetHotSpot(strUID);
        return true;
    }
    return false;
}

bool DevNode::SetHotSpot(std::string& strJson)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SetHotSpot(strUID, strJson);
        return true;
    }
    return false;
}

bool DevNode::GetNetStrategy()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->GetNetStrategy(strUID);
        return true;
    }
    return false;
}

bool DevNode::SetNetStrategy(std::string& strJson)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SetNetStrategy(strUID, strJson);
        return true;
    }
    return false;
}

bool DevNode::GetClarity()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->GetClarity(strUID);
        return true;
    }
    return false;
}

bool DevNode::SetClarity(int nClarity)
{
    return SwitchStream(nClarity);
}

bool DevNode::GetFlipMirror()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->GetFlipMirror(strUID);
        return true;
    }
    return false;
}

bool DevNode::SetFlipMirror(int nFilp, int nMirror)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SetFlipMirror(strUID, nFilp, nMirror);
        return true;
    }
    return false;
}

bool DevNode::SwitchStream(int nStream)
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->SwitchStream(strUID, nStream);
        return true;
    }
    return false;
}

bool DevNode::IsActivated()
{
    return bActivation;
}

bool DevNode::IsLocked()
{
    return bLockType;
}

void DevNode::Activate(bool bValue)
{
    bActivation = bValue;
}

VideoParamData DevNode::GetVideoParam()
{
    return stVideoParam;
}

void DevNode::SetVideoParam(VideoParamData& stParam)
{
    stVideoParam = stParam;
}

void DevNode::StartPreview()
{
    nPreviewCount++;
    if (nPreviewCount)
    {
        SetStatu(Play);
    }
}

void DevNode::StopPreview()
{
    nPreviewCount--;
    if (!nPreviewCount)
    {
        SetStatu(Pause);
    }
}


void DevNode::StartRecord()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->RecordControl(strUID, GetName(), true);
    }
}

void DevNode::StopRecord()
{
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->RecordControl(strUID, GetName(), false);
    }
}

void DevNode::SetWifiList(const IPCNetWifiAplist::Ptr& pData)
{
    pWifiList = pData;
}

IPCNetWifiAplist::Ptr DevNode::GetWifiList()
{
    return pWifiList;
}

void DevNode::SetHotSpotData(const IPCNetWiFiAPInfo_t::Ptr& pData)
{
    pHotSpot = pData;
}

IPCNetWiFiAPInfo_t::Ptr DevNode::GetHotSpotData()
{
    return pHotSpot;
}

void DevNode::SetStreamData(const IPCNetStreamInfo::Ptr& pData)
{
    pStreamInfo = pData;
}

IPCNetStreamInfo::Ptr DevNode::GetStreamData()
{
    return pStreamInfo;
}

void DevNode::OnLostConnect()
{
    memset(&stDevice, 0, sizeof(stDevice));
    SetStatu(Connecting);
}

void DevNode::OnReConnect()
{
    SetStatu(Play);
}

bool DevNode::Conect()
{
    if (IsDevLoaded())
    {
        return true;
    }
    auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pICPServer)
    {
        pICPServer->ConnectDevice(strUID, strPwd);
        return true;
    }
    return false;
}

bool DevNode::DisConnect()
{
    if (IsDevLoaded())
    {
        auto pICPServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pICPServer)
        {
            pICPServer->DisconnectDevice(strUID);
        }
    }
    return true;
}

GroupNode::GroupNode(const std::string& strGroupName, int nGroupId, int nId /*= -1*/)
    :strGroupName(strGroupName)
{
    emNodeType = DevTreeNodeType::Group;
    nParentId = nGroupId;
    if (nId != -1)
    {
        nNodeId = nId;
        s_GroupCount = max(s_GroupCount, nId);
    }
    else
    {
        nNodeId = ++s_GroupCount;
    }
}

std::string GroupNode::GetName()
{
    return strGroupName;
}

QJsonObject GroupNode::GenerateJsonObj()
{
    QJsonObject obj;
    obj.insert("nNodeId", nNodeId);
    obj.insert("nParentId", nParentId);
    obj.insert("emNodeType", emNodeType);
    obj.insert("strGroupName", QString::fromStdString(strGroupName));
    return obj;
}

void GroupNode::ReadDataJsonObj(QJsonObject& obj)
{
    nNodeId = utils::GetValueFromJsonObj(obj, "nNodeId").toInt();
    nParentId = utils::GetValueFromJsonObj(obj, "nParentId").toInt();
    emNodeType = (DevTreeNodeType)utils::GetValueFromJsonObj(obj, "emNodeType").toInt();
    strGroupName = utils::GetValueFromJsonObj(obj, "strGroupName").toString().toStdString();

    s_GroupCount = max(s_GroupCount, nNodeId);
}


FrameData::FrameData(const unsigned char* data, int width, int height, int len)
{
    AllocateBuf(len + 1);
    if (pBufData)
    {
        memcpy(pBufData, data, len * sizeof(unsigned char));
    }

    nPicWidth = width;
    nPicHeight = height;
}

void FrameData::AllocateBuf(int len)
{
    pBufData = new unsigned char[len];
    nBuffLen = len;
}

FrameData::~FrameData()
{
    if (pBufData)
    {
        delete[] pBufData;
        pBufData = nullptr;
    }
}

void SnapModeParam::SetSpeed(SyncSpeed emSpeed)
{
    nSyncSpped.store(emSpeed);
}

int SnapModeParam::GetSpeedCoe()
{
    int nCoe = 1;
    switch (nSyncSpped.load())
    {
    case Sync_1:
        nCoe = 1;
        break;
    case Sync_2:
        nCoe = 2;
        break;
    case Sync_4:
        nCoe = 4;
        break;
    case Sync_8:
        nCoe = 8;
        break;
    case Sync_16:
        nCoe = 16;
        break;
    default:
        break;
    }
    return nCoe;
}

SnapData::SnapData(FrameData::Ptr _pFrame)
{
    pFrame = _pFrame;
    if (_pFrame)
    {
        x = 0;
        y = 0;
        nWidth = _pFrame->nPicWidth;
        nHeight = _pFrame->nPicHeight;
    }
}
