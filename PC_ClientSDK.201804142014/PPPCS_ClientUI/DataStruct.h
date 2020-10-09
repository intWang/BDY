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
    virtual std::string GetStatuStr()=0;
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
    std::string strDevID;
    std::string strCustomName;
    std::string strPwd;
    static int s_nDevCount;
    int nPreviewCount;
    VideoParamData stVideoParam;

    volatile bool bActivation = 0;
    volatile bool bLockType = 0;

    IPCNetWifiAplist::Ptr pWifiList = nullptr;
    IPCNetWiFiAPInfo_t::Ptr pHotSpot = nullptr;
    IPCNetStreamInfo::Ptr pStreamInfo = nullptr;
    DevNode() = default;
    DevNode(const std::string& strUid, const std::string& strPwd
        , const std::string& strName, const std::string& strShortID, int nGroupID);
        
    virtual ~DevNode();
    virtual std::string GetName() override;
    virtual std::string GetStatuStr() override;
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

    void StartRecord();
    void StopRecord();

    void SetWifiList(const IPCNetWifiAplist::Ptr& pData);
    IPCNetWifiAplist::Ptr GetWifiList();

    void SetHotSpotData(const IPCNetWiFiAPInfo_t::Ptr& pData);
    IPCNetWiFiAPInfo_t::Ptr GetHotSpotData();

    void SetStreamData(const IPCNetStreamInfo::Ptr& pData);
    IPCNetStreamInfo::Ptr GetStreamData();

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

    bool GetClarity();
    bool SetClarity(int nClarity);

    bool GetFlipMirror();
    bool SetFlipMirror(int nFilp, int nMirror);

    bool SwitchStream(int nStream);

    bool IsActivated();
    bool IsLocked();
    void Activate(bool bValue);
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
    virtual std::string GetStatuStr() override;

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

struct SnapData
{
    using Ptr = std::shared_ptr<SnapData>;
    FrameData::Ptr pFrame = nullptr;
    int x = 0;
    int y = 0;
    int nWidth = 0;
    int nHeight = 0;

    SnapData(FrameData::Ptr _pFrame);
};


struct SnapModeParam
{
    using Ptr = std::shared_ptr<SnapModeParam>;
    std::atomic<int> nSelRectWidth = 200;
    std::atomic<int> nSelRectHeight = 100;
    std::atomic<SyncSpeed> nSyncSpped = SyncSpeed::Sync_1;
    std::atomic<int> nWndRows = 2;
    std::atomic<int> nWndColumns = 2;
    std::atomic<int> nAutoStop = 120;
    std::atomic<int> nMaxAutoStop = 180;
    std::atomic<bool> bLockProp = false;
    std::atomic<bool> bRowFirst = false;
    std::atomic<bool> bPopFull = false;

    void SetSpeed(SyncSpeed emSpeed);
    int GetSpeedCoe();
};
