#include "IPCNetServer.h"
#include "QtDefine.h"
#include "LogManager.h"
#include "utils.h"
#include "DataStruct.h"
#include <mutex>
#include "SEP2P_Error.h"
#include "JSONStructProtocal.h"
#include "JSONObject.hpp"
namespace ls
{
    IPCNetServer::Ptr g_pIPCServer = nullptr;

    void onStatus(const char* uuid, int status)
    {
        LogInfo("onStatus %s %d", uuid, status);
        if (g_pIPCServer)
        {
            g_pIPCServer->onStatus(uuid, status);
        }
    }

    void onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        LogInfo("onVideoData %s %d %d %d", uuid, type, len, timestamp);
        if (g_pIPCServer)
        {
            g_pIPCServer->onVideoData(uuid, type, data, len, timestamp);
        }
    }

    void onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        LogInfo("onAudioData %s %d %d %d", uuid, type, len, timestamp);
        if (g_pIPCServer)
        {
            g_pIPCServer->onAudioData(uuid, type, data, len, timestamp);
        }
    }

    void onJSONString(const char* uuid, int msg_type, const char* jsonstr)
    {
        LogInfo("OnJsonData %s %d %s", uuid, msg_type, jsonstr);
        if (g_pIPCServer)
        {
            g_pIPCServer->onJSONString(uuid, msg_type, jsonstr);
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
    }

    bool IPCNetServer::initialize()
    {
        m_Quit.store(false);
        m_thWork = std::thread(&IPCNetServer::Work, this);
        g_pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        return true;
    }

    void IPCNetServer::destroy()
    {
        m_Quit.store(true);

        LogInfo("IPCNetServer::destroy");
        std::unique_lock <std::mutex> lck(m_mxTmp);
        m_covQuit.notify_all();
        m_thWork.detach();
//         if (m_thWork.joinable())
//         {
 //            m_thWork.join();
//         }
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
        ITask::Ptr pTask = std::make_shared<VideoTask>(strUid, bStatu, param1);
        AddTask(pTask);
    }

    void IPCNetServer::AudioControl(std::string& strUid, bool bStatu, int param1 /*= 0*/)
    {
        ITask::Ptr pTask = std::make_shared<AudioTask>(strUid, bStatu);
        AddTask(pTask);
    }

    void IPCNetServer::onStatus(const char* uuid, int status)
    {
        switch (status) {
        case ERROR_SEP2P_SUCCESSFUL:
            IPCNetGetBrightness(uuid);
            break;
        case ERROR_SEP2P_INVALID_ID:
            break;
        case ERROR_SEP2P_STATUS_INVALID_PASSWD:
            break;
        default:
            break;
        }

        fireNotification(std::bind(&IIPCNetServerCallBack::OnDeviceStatuChanged, std::placeholders::_1, std::string(uuid), status));
    }

    void IPCNetServer::onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        //throw std::logic_error("The method or operation is not implemented.");
        fireNotification(std::bind(&IIPCNetServerCallBack::OnVideo, std::placeholders::_1, std::string(uuid), data, len, timestamp));
    }

    void IPCNetServer::onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    void IPCNetServer::onJSONString(const char* uuid, int msg_type, const char* jsonstr)
    {
        switch (msg_type)
        {
        case REMOTE_MSG_RESP_LOGIN:
            fireNotification(std::bind(&IIPCNetServerCallBack::OnDeviceConnected, std::placeholders::_1, std::string(jsonstr)));
            break;
        default:
            break;
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
        LogInfo("IPCNetServer Start Work");
        IPCNetInitialize("");

        std::unique_lock <std::mutex> lck(m_mxTmp);
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

        IPCNetDeInitial();
        LogInfo("IPCNetServer End Work");
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
            }
            delete jsroot;
            jsroot = nullptr;
        }
        else
        {
            LogError("Parse Json Error %s", strDevJsonInfo.c_str());
        }
        if (stDeviceData.p2p_uuid.size())
        {
            utils::TravelVector(m_CBFunc, [&stDeviceData](auto func) {
                if (func && func->funcOnDeviceConnected)
                {
                    func->funcOnDeviceConnected(stDeviceData);
                }
                return false;
            });
        }
    }

    void IPCNetServerCallBack::Register(CB::Ptr func)
    {
        auto pCBFunc = std::dynamic_pointer_cast<IIPCNetServerCallBack::CallBackFunc>(func);
        if (pCBFunc)
        {
            m_CBFunc.push_back(pCBFunc);
        }
    }

    void IPCNetServerCallBack::UnRegister(CB::Ptr func)
    {
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
        //throw std::logic_error("The method or operation is not implemented.");
    }

    void IPCNetServerCallBack::OnVideo(const std::string& strUid, unsigned char*data, int len, long timestamp)
    {
        //throw std::logic_error("The method or operation is not implemented.");
        //h264 decode
    }

}
