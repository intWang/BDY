#include "IPCNetServer.h"
#include "QtDefine.h"
#include "LogManager.h"
#include "utils.h"
#include "DataStruct.h"
#include <mutex>
#include "SEP2P_Error.h"
#include "JSONStructProtocal.h"
#include "JSONObject.hpp"
#include "ConfigCenter.h"
#include <QDateTime>
namespace ls
{
    std::string strUID = utils::GetUUID();

    void SaveBuf(const unsigned char* pData, int len, const char* filename)
    {
        FILE *f;
        int i;
        fopen_s(&f, filename, "a");
        if (f)
        {
            fwrite(pData, len, 1, f);
            fclose(f);
        }
    }

    IIPCNetServer::WeakPtr g_pIPCServer;
    IIPCNetServerCallBack::WeakPtr g_pIPCServerCB;
    void onStatus(const char* uuid, int status)
    {
        LogInfo("onStatus %s %d", uuid, status);
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->onStatus(uuid, status);
        }
    }

    void onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        //LogInfo("onVideoData %s %d %d %d", uuid, type, len, timestamp);
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->onVideoData(uuid, type, data, len, timestamp);
        }
//         static std::string strFilename = strTmpPath + strUID;
//         SaveBuf(data, len, strFilename.c_str());
    }

    void onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        LogInfo("onAudioData %s %d %d %d", uuid, type, len, timestamp);
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->onAudioData(uuid, type, data, len, timestamp);
        }
    }

    void onJSONString(const char* uuid, int msg_type, const char* jsonstr)
    {
        //LogInfo("OnJsonData %s %d %s", uuid, msg_type, jsonstr);
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->onJSONString(uuid, msg_type, jsonstr);
        }
    }

    void OnCmdResult(int cmd, const char*uuid, const char*json)
    {
        LogInfo("OnCmdResult %s %d %s", uuid, cmd, json);
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->OnNetOperteResult(cmd, uuid, json);
        }
    }

    IPCNetServer::Ptr CreateIPCNetServer()
    {
        return std::make_shared<IPCNetServer>();
    }

    IIPCNetServerCallBack::Ptr CreateIPCNetServerCB()
    {
        return std::make_shared<IPCNetServerCallBack>();
    }


    IPCNetServer::IPCNetServer()
    {
        m_IPCNetEventHandler.onAudioData = ls::onAudioData;
        m_IPCNetEventHandler.onJSONString = ls::onJSONString;
        m_IPCNetEventHandler.onStatus = ls::onStatus;
        m_IPCNetEventHandler.onVideoData = ls::onVideoData;
    }


    IPCNetServer::~IPCNetServer()
    {
        destroy();
    }

    bool IPCNetServer::initialize()
    {
        m_Quit.store(false);
        m_thWork = std::thread(&IPCNetServer::Work, this);
        g_pIPCServer = std::dynamic_pointer_cast<IIPCNetServer>(shared_from_this());
        IPCNetInitialize("");

        return true;
    }

    void IPCNetServer::destroy()
    {
        m_Quit.store(true);

        LogInfo("IPCNetServer::destroy");
        {
            m_covQuit.notify_all();
        }
        //m_thWork.detach();
        if (m_thWork.joinable())
        {
            m_thWork.join();
        }
        //IPCNetDeInitial();
    }

    void IPCNetServer::ConnectDevice(std::string& strUid, std::string& strPwd)
    {
        ITask::Ptr pTask = std::make_shared<ConnectTask>(strUid, strPwd, &m_IPCNetEventHandler);
        AddTask(pTask);
    }

    void IPCNetServer::DisconnectDevice(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<DisconnectTask>(strUid);
        AddTask(pTask);
    }

    void IPCNetServer::VideoControl(std::string& strUid, bool bStatu, int param1 /*= 0*/)
    {
        bool bRunTask = false;
        int& nRunCount = m_RuningVideoTask[strUid];
        if (bStatu)
        {
            if (++nRunCount == 1)
            {
                PrepareDecoder(strUid);
                bRunTask = true;
            }
        }
        else
        {
            if (--nRunCount == 0)
            {
                RecycleDecoder(strUid);
                bRunTask = true;
            }
        }

        if (bRunTask)
        {
            ITask::Ptr pTask = std::make_shared<VideoTask>(strUid, bStatu, OnCmdResult, param1);
            AddTask(pTask);
        }
    }

    void IPCNetServer::AudioControl(std::string& strUid, bool bStatu, int param1 /*= 0*/)
    {
        ITask::Ptr pTask = std::make_shared<AudioTask>(strUid, bStatu);
        AddTask(pTask);
    }

    void IPCNetServer::onStatus(const char* uuid, int status)
    {
        std::string struid(uuid);
        switch (status) {
        case ERROR_SEP2P_SUCCESSFUL:
            //IPCNetGetBrightness(uuid);
            break;
        case ERROR_SEP2P_INVALID_ID:
            break;
        case ERROR_SEP2P_STATUS_INVALID_PASSWD:
            break;
        default:
            break;
        }

        fireNotification(std::bind(&IIPCNetServerCallBack::OnDeviceStatuChanged, std::placeholders::_1, struid, status));
    }

    void IPCNetServer::onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        //throw std::logic_error("The method or operation is not implemented.");
        //h264 decode
        auto strUuid = std::string(uuid);
        {
            std::lock_guard<std::mutex> guard(m_mxDecoder);
            if (m_videoDecoder.find(strUuid) != m_videoDecoder.end())
            {
                auto pDecoder = m_videoDecoder[strUuid];
                if (pDecoder)
                {
                    pDecoder->InputData(data, len);
                }
            }
        }
    }

    void IPCNetServer::onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    void IPCNetServer::onJSONString(const char* uuid, int msg_type, const char* jsonstr)
    {
        auto strUid = std::string(uuid);
        switch (msg_type)
        {
        case REMOTE_MSG_RESP_LOGIN:
            fireNotification(std::bind(&IIPCNetServerCallBack::OnDeviceConnected, std::placeholders::_1, std::string(jsonstr)));
            break;
        default:
            break;
        }
    }

    void DecodeCallBack(const char* pUserCode, const unsigned char* pBuf, int width, int height, int len)
    {
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->OnDecodeCallBack(pUserCode, pBuf, width, height, len);
        }
    }


    void DecodeStatuCallBack(const char* pUserCode, DecodeStatus emStatus, void* pParam)
    {
        if (auto pIPCServer = g_pIPCServer.lock())
        {
            pIPCServer->OnDecodeStatuCallBack(pUserCode, emStatus, pParam);
        }
    }


    void IPCNetServer::OnDecodeCallBack(const char* pUserCode, const unsigned char* pBuf, int width, int height, int len)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::OnVideo, std::placeholders::_1, pUserCode, pBuf, width, height, len, 0));
    }


    void IPCNetServer::OnDecodeStatuCallBack(const char* pUserCode, DecodeStatus emStatus, void* pParam)
    {
        std::string strHint = "";
        std::string strDevUid = pUserCode;
        switch (emStatus)
        {
        case Start_Record:
            fireNotification(std::bind(&IIPCNetServerCallBack::OnRecordStatuNotify, std::placeholders::_1, strDevUid, true));
            strHint = "录像开始！";
            break;
        case Stop_Record:
            fireNotification(std::bind(&IIPCNetServerCallBack::OnRecordStatuNotify, std::placeholders::_1, strDevUid, false));
            strHint = "录像结束！";
            break;
        case Start_Transfer:
            strHint = "录像正在转码！";
            break;
        case Complete_Transfer:
        {
            std::string strInfo = (const char*)pParam;
            strHint = strInfo;
        }
            break;
        default:
            break;
        }
        OnHintMsg(strHint);
    }

    void IPCNetServer::onParamCmd(std::string& strUid, IPCNetCamColorCfg_st& stIPCNetCamColorCfg)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onParamCmd, std::placeholders::_1, strUid, stIPCNetCamColorCfg));
    }

    void IPCNetServer::onDevTimeCmd(std::string& strUid, IPCNetTimeCfg_st& stIPCNetTimeCfg)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onDevTimeCmd, std::placeholders::_1, strUid, stIPCNetTimeCfg));
    }

    void IPCNetServer::onSearchWifiCmd(std::string& strUid, IPCNetWifiAplist::Ptr& stNetWirelessConfig)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onSearchWifiCmd, std::placeholders::_1, strUid, stNetWirelessConfig));
    }

    void IPCNetServer::onGetWifiCmd(std::string& strUid, IPCNetWirelessConfig_st::Ptr& pData)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onGetWifiCmd, std::placeholders::_1, strUid, pData));
    }

    void IPCNetServer::onGetHotSpotCmd(std::string& strUid, IPCNetWiFiAPInfo_t::Ptr& pData)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onGetHotSpotCmd, std::placeholders::_1, strUid, pData));
    }

    void IPCNetServer::onGetNetStrategy(std::string& strUid, IPCNetNetworkStrategy::Ptr& pData)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onGetNetStrategy, std::placeholders::_1, strUid, pData));
    }

    void IPCNetServer::onGetClarityCmd(std::string& strUid, IPCNetStreamInfo::Ptr& pData)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onGetClarityCmd, std::placeholders::_1, strUid, pData));
    }

    void IPCNetServer::onGetFlipMirrorCmd(std::string& strUid, IPCNetPicOverTurn::Ptr& pData)
    {
        fireNotification(std::bind(&IIPCNetServerCallBack::onGetFlipMirrorCmd, std::placeholders::_1, strUid, pData));
    }

    void IPCNetServer::OnHintMsg(std::string& strHint, ls::HintLevel emLevel)
    {
        if (g_pEngine)
        {
            auto pHintServer = g_pEngine->GetHintServer();
            if (pHintServer)
            {
                pHintServer->OnUserOperateHint(strHint, emLevel);
            }
        }
    }

    void IPCNetServer::OnNetOperteResult(int cmd, const char*uuid, const char*json)
    {
        PJson::JSONObject jsdata(json);
        std::string strUid = uuid;
        switch (cmd)
        {
        case IPCNET_NETWORK_WIFI_SEARCH_GET_RESP:
        {
            auto pWirelessNetList = std::make_shared<IPCNetWifiAplist>();
            if (pWirelessNetList && pWirelessNetList->parseJSON(jsdata))
            {
                onSearchWifiCmd(strUid, pWirelessNetList);
            }
        }
        break;
        case IPCNET_NETWORK_WIFI_GET_RESP:
        {
            auto pData = std::make_shared<IPCNetWirelessConfig_st>();
            if (pData && pData->parseJSON(jsdata))
            {
                onGetWifiCmd(strUid, pData);
            }
        }
        break;
        case IPCNET_GET_WIFI_AP_INFO_RESP:
        {
            auto pData = std::make_shared<IPCNetWiFiAPInfo_t>();
            if (pData && pData->parseJSON(jsdata))
            {
                onGetHotSpotCmd(strUid, pData);
            }
        }
        break;
        case IPCNET_GET_TIME_RESP:
        {
            IPCNetTimeCfg_st ipcNetTimeCfg_st;
            if (ipcNetTimeCfg_st.parseJSON(jsdata))
            {
                onDevTimeCmd(strUid, ipcNetTimeCfg_st);
                return;
            }
        }
        break;
        case IPCNET_GET_NETWORK_STRATEGY_RESP:
        {
            auto pData = std::make_shared<IPCNetNetworkStrategy>();
            if (pData && pData->parseJSON(jsdata))
            {
                onGetNetStrategy(strUid, pData);
            }
        }
        break;
        case IPCNET_GET_OVERTURN_RESP:
        {
            auto pData = std::make_shared<IPCNetPicOverTurn>();
            if (pData && pData->parseJSON(jsdata))
            {
                onGetFlipMirrorCmd(strUid, pData);
            }
        }
        break;
        case IPCNET_GET_CAM_PIC_CFG_RESP:
        {
            IPCNetCamColorCfg_st stIPCNetCamColorCfg;
            if (stIPCNetCamColorCfg.parseJSON(jsdata))
            {
                onParamCmd(strUid, stIPCNetCamColorCfg);
                return;
            }
        }
        case IPCNET_GET_STREAM_REQ:
        {
            auto pData = std::make_shared<IPCNetStreamInfo>();
            if (pData && pData->parseJSON(jsdata))
            {
                onGetClarityCmd(strUid, pData);
            }
        }
        break;
        default:
        {
            IPCNetRetsult result;
            if (result.parseJSON(jsdata))
            {
                std::string strHintMsg;
                switch (cmd)
                {
                case IPCNET_SET_WIFI_AP_INFO_RESP:
                    strHintMsg = "设置热点操作";
                break;
                case IPCNET_NETWORK_WIFI_SET_RESP:
                    strHintMsg = "连接Wifi操作";
                break;
                case IPCNET_SET_NETWORK_STRATEGY_RESP:
                    strHintMsg = "设置网络策略";
                    break;
                case IPCNET_SET_REBOOT_RESP:
                    strHintMsg = "设备重启";
                    break;
                case IPCNET_USER_SET_RESP:
                    strHintMsg = "用户设置(密码)";
                    break;
                case IPCNET_SET_OVERTURN_RESQ:
                    strHintMsg = "图像翻转设置";
                    break;
                default:
                    break;
                }
                if (strHintMsg.size())
                {
                    std::string strResult = (result.ret == 0?"成功！":"失败！");
                    OnHintMsg(strHintMsg + strResult);
                }
            }
        }
        break;
        }
    }

    void IPCNetServer::RestartDevice(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<DeviceRestartTask>(strUid,OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::ResetDevice(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<DeviceResetTask>(strUid, OnCmdResult);
        AddTask(pTask);
    }

    int IPCNetServer::VideoParamCtrl(std::string& strUid, bool bSet, VideoParam tagParam, int nParamValue)
    {
        ITask::Ptr pTask = std::make_shared<VideoParamTask>(strUid, bSet, tagParam, OnCmdResult, nParamValue);
        AddTask(pTask);
        return 0;
    }

    void IPCNetServer::PrepareDecoder(const std::string& strUid)
    {
        std::lock_guard<std::mutex> guard(m_mxDecoder);
        if (m_videoDecoder.find(strUid) == m_videoDecoder.end())
        {
            auto pDecoder = Decode_CreateDecoder();
            if (pDecoder != 0)
            {
                pDecoder->Init(DecodeCallBack);
                pDecoder->SetUserCode(strUid.c_str());
                m_videoDecoder[strUid] = pDecoder;
            }
        }
    }

    void IPCNetServer::RecycleDecoder(const std::string& strUid)
    {
        std::lock_guard<std::mutex> guard(m_mxDecoder);
        if (m_videoDecoder.find(strUid) != m_videoDecoder.end())
        {
            auto pDecoder = m_videoDecoder[strUid];
            m_videoDecoder.erase(strUid);
        }
    }

    void IPCNetServer::AddTask(ITask::Ptr pTask)
    {
        std::lock_guard<std::mutex> guard(m_mxTaskList);
        m_queWorkList.push(pTask);
    }

    ls::ITask::Ptr IPCNetServer::PopTask()
    {
        std::lock_guard<std::mutex> guard(m_mxTaskList);
        ITask::Ptr pTask = m_queWorkList.front();
        m_queWorkList.pop();
        return pTask;
    }

    void IPCNetServer::Work()
    {
        //LogInfo("IPCNetServer Start Work");
        std::mutex tmpMutex;
        std::unique_lock <std::mutex> lck(tmpMutex);
        while (true)
        {
            auto waitREt = m_covQuit.wait_for(lck, std::chrono::milliseconds(1000));
            if (m_Quit.load())
            {
                break;
            }
            while (m_queWorkList.size())
            {
                ITask::Ptr pCurTask = PopTask();
                if (pCurTask)
                {
                    pCurTask->Run();
                }
            }
        }

        //LogInfo("IPCNetServer End Work");
    }

    void IPCNetServer::GetDevTime(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<GetDeviceTimeTask>(strUid, OnCmdResult);
        AddTask(pTask);
    }


    void IPCNetServer::SearchWifi(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<SearchWifiTask>(strUid, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SetWifi(std::string& strUid, std::string& strSSID, std::string& strPwd, std::string& strEncType)
    {
        ITask::Ptr pTask = std::make_shared<SetWifiTask>(strUid, strSSID, strPwd, strEncType, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::ChangeDevPwd(std::string& strUid, std::string& strPwd)
    {
        ITask::Ptr pTask = std::make_shared<ChangeDevPwdTask>(strUid, strPwd, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::GetHotSpot(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<HotSpotTask>(strUid, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SetHotSpot(std::string& strUid, std::string& strJsonParam)
    {
        ITask::Ptr pTask = std::make_shared<HotSpotTask>(strUid, OnCmdResult, strJsonParam.c_str());
        AddTask(pTask);
    }

    void IPCNetServer::GetNetStrategy(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<NetStrategyTask>(strUid, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SetNetStrategy(std::string& strUid, std::string& strJsonParam)
    {
        ITask::Ptr pTask = std::make_shared<NetStrategyTask>(strUid, OnCmdResult, strJsonParam.c_str());
        AddTask(pTask);
    }

    void IPCNetServer::GetClarity(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<ResolutionTask>(strUid, false, 0, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SetClarity(std::string& strUid, int nClarity)
    {
        ITask::Ptr pTask = std::make_shared<ResolutionTask>(strUid, true, nClarity, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::GetFlipMirror(std::string& strUid)
    {
        ITask::Ptr pTask = std::make_shared<FlipMirrorTask>(strUid, false, 0, 0, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SetFlipMirror(std::string& strUid, int nFilp, int nMirror)
    {
        ITask::Ptr pTask = std::make_shared<FlipMirrorTask>(strUid, true, nFilp, nMirror, OnCmdResult);
        AddTask(pTask);
    }

    void IPCNetServer::SwitchStream(std::string& strUid, int nStream)
    {
        ITask::Ptr pTask = std::make_shared<SwitchStreamTask>(strUid, nStream, OnCmdResult);
        //AddTask(pTask);
        RecycleDecoder(strUid);
        PrepareDecoder(strUid);
        pTask->Run();
    }

    void IPCNetServer::RecordControl(std::string& strUiD,std::string& strName, bool bStart)
    {
        std::string strHint = "";
        std::lock_guard<std::mutex> guard(m_mxDecoder);
        auto &pRecoder = m_videoDecoder[strUiD];
        if (!pRecoder)
        {
            strHint = "解码器错误！";
            OnHintMsg(strHint);
            return;
        }

        if (bStart)
        {
            pRecoder->StartRecord(ConfigCenter::GetInstance().GetRecordSaveFileName(strName).toStdString().c_str(), strName.c_str(), DecodeStatuCallBack);
        }
        else
        {
            QString tmNow = QDateTime::currentDateTime().toString("~hhmmss");
            pRecoder->StopRecord(tmNow.toStdString().c_str());
        }
    }

    void IPCNetServerCallBack::OnDeviceConnected(const std::string& strDevJsonInfo)
    {
        DeviceData stDeviceData;
        PJson::JSONObject jsdata(strDevJsonInfo.c_str());
        if (jsdata.isValid())
        {
            PJson::JSONObject *jsroot = jsdata.getJSONObject("dev_info");
            if (jsroot)
            {
                jsroot->getString("name", stDeviceData.name);
                jsroot->getString("p2p_uuid", stDeviceData.p2p_uuid);
                jsroot->getString("app_ver", stDeviceData.app_ver);
                jsroot->getString("sys_ver", stDeviceData.sys_ver);
                jsroot->getString("pid", stDeviceData.pid);
                PJson::JSONArray*jaVideoEncode = jsroot->getJSONArray("video_input");
                if (jaVideoEncode)
                {
                    int nSize = jaVideoEncode->getLength();
                    for (int i = 0; i < nSize; ++i)
                    {
                        PJson::JSONObject *video_input = jaVideoEncode->getJSONObject(i);
                        if (video_input)
                        {
                            VideoChannel videoChannel;
                            video_input->getInt("ch_no", videoChannel.ch_no);
                            video_input->getInt("audio_code_type", videoChannel.audio_code_type);
                            video_input->getInt("audio_sample_freq", videoChannel.audio_sample_freq);

                            PJson::JSONArray*reslArray = video_input->getJSONArray("Resl");
                            if (reslArray)
                            {
                                if (reslArray->getLength() == 2)
                                {
                                    PJson::JSONObject* maxSize = reslArray->getJSONObject(0);
                                    PJson::JSONObject* minSize = reslArray->getJSONObject(1);
                                    if (maxSize)
                                    {
                                        maxSize->getInt("width", videoChannel.width_max);
                                        maxSize->getInt("height", videoChannel.height_max);
                                        delete maxSize;
                                    }
                                    if (minSize)
                                    {
                                        minSize->getInt("width", videoChannel.width_min);
                                        minSize->getInt("height", videoChannel.height_min);
                                        delete minSize;
                                    }
                                }
                                delete reslArray;
                            }
                            video_input->getString("chann_name", videoChannel.chann_name);
                            stDeviceData.video_input.push_back(videoChannel);
                            delete video_input;
                        }
                    }
                    delete jaVideoEncode;
                }
                delete jsroot;
                jsroot = nullptr;
            }
        }
        else
        {
            LogError("Parse Json Error %s", strDevJsonInfo.c_str());
        }
        if (stDeviceData.p2p_uuid.size())
        {
            std::lock_guard<std::mutex> guard(m_mxLockCB);
            utils::TravelVector(m_CBFunc, [&stDeviceData](auto func) {
                if (func && func->funcOnDeviceConnected)
                {
                    func->funcOnDeviceConnected(stDeviceData);
                }
                return false;
            });


            {
                std::lock_guard<std::mutex> guard(m_mxLostTrack);
                auto iterFind = std::find(m_LostDevice.begin(), m_LostDevice.end(), stDeviceData.p2p_uuid);
                if (iterFind != m_LostDevice.end())
                {
                    auto strUid = stDeviceData.p2p_uuid;
                    auto nStatu = CSTATU_RECONNECTED;
                    utils::TravelVector(m_CBFunc, [&strUid, &nStatu](auto func) {
                        if (func && func->funcOnDeviceStatuChanged)
                        {
                            func->funcOnDeviceStatuChanged(strUid, nStatu);
                        }
                        return false;
                    });
                    m_LostDevice.erase(iterFind);
                }
            }
        }
    }

    void IPCNetServerCallBack::Register(CB::Ptr func)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        auto pCBFunc = std::dynamic_pointer_cast<IIPCNetServerCallBack::CallBackFunc>(func);
        if (pCBFunc)
        {
            m_CBFunc.push_back(pCBFunc);
        }
    }

    void IPCNetServerCallBack::UnRegister(CB::Ptr func)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        auto pCBFunc = std::dynamic_pointer_cast<IIPCNetServerCallBack::CallBackFunc>(func);
        for (auto iterFunc = m_CBFunc.begin(); iterFunc != m_CBFunc.end(); ++iterFunc)
        {
            if (pCBFunc == *iterFunc)
            {
                m_CBFunc.erase(iterFunc);
                break;
            }
        }
    }

    void IPCNetServerCallBack::OnDeviceStatuChanged(const std::string& strUid, int nStatu)
    {
        if (nStatu == ERROR_SEP2P_STATUS_CONNECTION_LOST)
        {
            std::lock_guard<std::mutex> guard(m_mxLostTrack);
            m_LostDevice.push_back(strUid);
        }
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &nStatu](auto func) {
            if (func && func->funcOnDeviceStatuChanged)
            {
                func->funcOnDeviceStatuChanged(strUid, nStatu);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::OnVideo(const std::string& strUid, const unsigned char*data, int width, int height, int len, long timestamp)
    {
        //throw std::logic_error("The method or operation is not implemented.");
        DispatchVideoData(strUid, data, width, height, len);
    }


    void IPCNetServerCallBack::onParamCmd(const std::string& strUid, const IPCNetCamColorCfg_st& stParam)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &stParam](auto func) {
            if (func && func->funcOnVideoParamData)
            {
                func->funcOnVideoParamData(strUid, stParam);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onDevTimeCmd(const std::string& strUid, const IPCNetTimeCfg_st& stData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &stData](auto func) {
            if (func && func->funcOnDevTimeData)
            {
                func->funcOnDevTimeData(strUid, stData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onSearchWifiCmd(std::string& strUid, const IPCNetWifiAplist::Ptr& stNetWirelessConfig)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &stNetWirelessConfig](auto func) {
            if (func && func->funcOnSearchWifiData)
            {
                func->funcOnSearchWifiData(strUid, stNetWirelessConfig);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onGetWifiCmd(std::string& strUid, const IPCNetWirelessConfig_st::Ptr& pData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &pData](auto func) {
            if (func && func->funcOnGetWifiData)
            {
                func->funcOnGetWifiData(strUid, pData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onGetHotSpotCmd(std::string& strUid, const IPCNetWiFiAPInfo_t::Ptr& pData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &pData](auto func) {
            if (func && func->funcOnGetHotSpotData)
            {
                func->funcOnGetHotSpotData(strUid, pData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onGetNetStrategy(std::string& strUid, const IPCNetNetworkStrategy::Ptr& pData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &pData](auto func) {
            if (func && func->funcOnGetNetStrategyData)
            {
                func->funcOnGetNetStrategyData(strUid, pData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onGetClarityCmd(std::string& strUid, const IPCNetStreamInfo::Ptr& pData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &pData](auto func) {
            if (func && func->funconGetClarityData)
            {
                func->funconGetClarityData(strUid, pData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::onGetFlipMirrorCmd(std::string& strUid, const IPCNetPicOverTurn::Ptr& pData)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &pData](auto func) {
            if (func && func->funconGetFlipMirrorData)
            {
                func->funconGetFlipMirrorData(strUid, pData);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::OnRecordStatuNotify(const std::string& strUid, bool bStart)
    {
        std::lock_guard<std::mutex> guard(m_mxLockCB);
        utils::TravelVector(m_CBFunc, [&strUid, &bStart](auto func) {
            if (func && func->funcOnRecordNotify)
            {
                func->funcOnRecordNotify(strUid, bStart);
            }
            return false;
        });
    }

    void IPCNetServerCallBack::DispatchVideoData(const std::string& strUid, const unsigned char*data, int width, int height, int len)
    {
        FrameData::Ptr pFrameData = std::make_shared<FrameData>(data, width, height, len);
        utils::TravelVector(m_CBFunc, [&strUid, pFrameData](auto func) {
            if (func && func->funcOnFrameData)
            {
                func->funcOnFrameData(strUid, pFrameData);
            }
            return false;
        });
    }

}
