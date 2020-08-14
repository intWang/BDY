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

DevNode::DevNode(const std::string& strUid, const std::string& strPwd, int nGroupID)
    : strUID(strUid)
    , strPwd(strPwd)
{
    emNodeType = DevTreeNodeType::Device;
    nParentId = nGroupID;
    nNodeId = MAKEDEVID(nGroupID, s_nDevCount++);
}

std::string DevNode::GetName()
{
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
    strUID = utils::GetValueFromJsonObj(obj, "strUID").toString().toStdString();
    strPwd = utils::GetValueFromJsonObj(obj, "strPwd").toString().toStdString();
}

void DevNode::UpdateDevData(const DeviceData& data)
{
    stDevice = data;
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
}