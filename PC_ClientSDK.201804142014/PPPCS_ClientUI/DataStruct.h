#pragma once
#include <memory>
#include <string>
#include <vector>
#include "QtDefine.h"
#include "JSONStructProtocal.h"
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

typedef struct VideoParamData
{
    short brightness;
    short tone;
    short contrast;
    short saturation;
    time_t tmLastUpdate;

    bool isExpired()
    {
        return (time(NULL) - tmLastUpdate) > 60 * 10;
    }
}*PVideoParamData;

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
    DevTreeNodeType emNodeType = Group;
    DevTreeNodeStatu emStatu = Default;
    TreeNode() = default;
    virtual std::string GetName() = 0;
    virtual QJsonObject GenerateJsonObj() = 0;
    virtual void ReadDataJsonObj(QJsonObject& obj) = 0;
    virtual ~TreeNode() {}
    int GetNodeID();
    int GetParent();
    DevTreeNodeType GetDataType();

    void SetStatu(DevTreeNodeStatu emStatu);
    DevTreeNodeStatu GetStatu();
    std::string GetStatuProperty();

};

struct DevNode :public TreeNode
{
    using Ptr = std::shared_ptr<DevNode>;
    DeviceData stDevice;
    std::string strUID;
    std::string strShortID;
    std::string strCustomName;
    std::string strPwd;
    static int s_nDevCount;
    int nPreviewCount;
    VideoParamData stVideoParam;

    IPCNetWifiAplist::Ptr pWifiList;
    IPCNetWiFiAPInfo_t::Ptr pHotSpot;

    DevNode() = default;
    DevNode(const std::string& strUid, const std::string& strPwd, const std::string& strName, int nGroupID);
        
    virtual ~DevNode();
    virtual std::string GetName() override;
    virtual  QJsonObject GenerateJsonObj() override;
    virtual void ReadDataJsonObj(QJsonObject& obj) override;
    void UpdateDevData(const DeviceData& data);
    bool IsDevLoaded();
    bool CheckPwd(std::string strInputPwd);
    std::string GetDevUid();
    std::string GetLabelUid();
    VideoParamData GetVideoParam();
    void SetVideoParam(VideoParamData& stParam);

    void StartPreview();
    void StopPreview();

    void SetWifiList(IPCNetWifiAplist::Ptr pData);
    IPCNetWifiAplist::Ptr GetWifiList();

    void SetHotSpotData(IPCNetWiFiAPInfo_t::Ptr pData);
    IPCNetWiFiAPInfo_t::Ptr GetHotSpotData();

    //////CB OPerate
    void OnLostConnect();
    void OnReConnect();

    //////IPCNetOperate
    bool Conect();
    bool DisConnect();
    bool GetDevTime();
    bool RestartDevice();
    bool RestoreDevice();

    bool SearchWifi();
    bool SetWifi(std::string& strSSID, std::string& strPwd, std::string& strEncType);
    bool ChangePwd(std::string strNewPwd);
    bool GetHotSpot();
    bool SetHotSpot(std::string& strJson);
    bool GetNetStrategy();
    bool SetNetStrategy(std::string& strJson);
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
    FrameData(const unsigned char* data, int width, int height, int len);
    void AllocateBuf(int len);
    virtual ~FrameData();
    unsigned char* pBufData = nullptr;
    int nBuffLen;
    int nPicWidth;
    int nPicHeight;
};
