#include "DataStruct.h"
#include "utils.h"

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

DevNode::DevNode(const std::string& strUid, const std::string& strPwd, const std::string& strName, int nGroupID)
    : strUID(strUid)
    , strPwd(strPwd)
    , strCustomName(strName)
{
    emNodeType = DevTreeNodeType::Device;
    nParentId = nGroupID;
    nNodeId = MAKEDEVID(nGroupID, ++s_nDevCount);
}

std::string DevNode::GetName()
{
    if (strCustomName.size())
    {
        return strCustomName;
    }
    if (stDevice.name.empty())
    {
        return strUID;
    }
    return stDevice.name;
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

    s_nDevCount = max(GETDEVID(nNodeId), s_nDevCount);
}

void DevNode::UpdateDevData(const DeviceData& data)
{
    stDevice = data;
}

bool DevNode::IsDevLoaded()
{
    return stDevice.video_input.size();
}

ChannelNode::Ptr DevNode::GetChannelData()
{
    if (stDevice.video_input.size())
    {
        return std::make_shared<ChannelNode>(nNodeId, strUID, stDevice.video_input[0]);
    }
    return nullptr;
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

ChannelNode::ChannelNode(int nDevID, const std::string& strDevUid, VideoChannel& stData)
    :channelData(stData)
{
    strUID = strDevUid;
    emNodeType = DevTreeNodeType::Channel;
    nParentId = nDevID;
    nNodeId = MAKECHANNELID(nDevID, stData.ch_no);
}

std::string ChannelNode::GetName()
{
    return channelData.chann_name;
}

QJsonObject ChannelNode::GenerateJsonObj()
{
    QJsonObject obj;
    return obj;
}

void ChannelNode::ReadDataJsonObj(QJsonObject& obj)
{
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
