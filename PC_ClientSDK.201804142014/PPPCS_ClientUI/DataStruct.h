#pragma once
#include <memory>
#include <string>
#include <vector>
#include "QtDefine.h"
typedef struct VideoChannel
{
    int ch_no;
    int audio_code_type;
    int audio_sample_freq;
    int width_min;
    int width_max;
    int height_min;
    int height_max;
    std::string chann_name;
}*PVideoChannel;

typedef struct HotKey
{

}*PHotKey;

typedef struct FuncList
{

}*PFuncList;

typedef struct DeviceData
{
    struct LoginInfo
    {
        std::string strDevUid;
        std::string strDevPwd;
    };

    std::string name;
    std::string p2p_uuid;
    std::string app_ver;
    std::string sys_ver;
    std::string pid;

    std::vector<VideoChannel> video_input;
    std::vector<HotKey> hot_key;
    std::vector<FuncList> funclist;
}*PDeviceData;

struct TreeNode
{
    using Ptr = std::shared_ptr<TreeNode>;
    int nNodeId = -1;
    int nParentId = -1;
    DevTreeNodeType emNodeType;
    TreeNode() = default;
    virtual std::string GetName() = 0;
    virtual QJsonObject GenerateJsonObj() = 0;
    virtual void ReadDataJsonObj(QJsonObject& obj) = 0;
    virtual ~TreeNode() {}
    int GetNodeID();
    int GetParent();
    DevTreeNodeType GetDataType();

};

struct DevNode :public TreeNode
{
    using Ptr = std::shared_ptr<DevNode>;
    DeviceData stDevice;
    std::string strUID;
    std::string strPwd;
    static int s_nDevCount;
    DevNode() = default;
    DevNode(const std::string& strUid, const std::string& strPwd, int nGroupID);
        
    virtual ~DevNode() {}

    virtual std::string GetName() override;

    virtual  QJsonObject GenerateJsonObj() override;

    virtual void ReadDataJsonObj(QJsonObject& obj) override;

    void UpdateDevData(const DeviceData& data);
};

struct ChannelNode:public TreeNode
{
    VideoChannel channelData;
    std::string strUID;

    using Ptr = std::shared_ptr<ChannelNode>;
    ChannelNode() = default;
    ChannelNode(int nDevID, const std::string& strDevUid , VideoChannel& stData);

    virtual ~ChannelNode() {}
    virtual std::string GetName() override;
    virtual QJsonObject GenerateJsonObj() override;
    virtual void ReadDataJsonObj(QJsonObject& obj) override;
};

struct GroupNode :public TreeNode
{
    using Ptr = std::shared_ptr<GroupNode>;
    std::string strGroupName;
    static int s_GroupCount;

    GroupNode() = default;
    GroupNode(const std::string& strGroupName, int nGroupId, int nId = -1);
       
    virtual ~GroupNode() {}

    virtual std::string GetName() override;

    virtual QJsonObject GenerateJsonObj() override;

    virtual void ReadDataJsonObj(QJsonObject& obj) override;
};

enum StreamDataType
{
    Video,
    Audio,
};
struct StreamData
{
    using Ptr = std::shared_ptr<StreamData>;
    StreamData() = default;
    virtual ~StreamData() {};
    StreamDataType emDataType;

};

struct FrameData :public StreamData
{
    using Ptr = std::shared_ptr<FrameData>;
    FrameData() = default;
    FrameData(unsigned char* data, int len);
    void AllocateBuf(int len);
    virtual ~FrameData();
    unsigned char* pBufData = nullptr;
    int nBuffLen;


};
