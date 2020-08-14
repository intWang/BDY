#include "IPCNetServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "QtDefine.h"
#include "LogManager.h"
#include "utils.h"
#include "DataStruct.h"
#include <mutex>
namespace ls
{
    void onStatus(const char* uuid, int status)
    {
        LogInfo("onStatus %s %d", uuid, status);
    }

    void onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {

    }

    void onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp)
    {

    }

    void onJSONString(const char* uuid, int msg_type, const char* jsonstr)
    {
        LogInfo("OnJsonData %s %s", uuid, jsonstr);
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
        m_IPCNetEventHandler.onAudioData = onAudioData;
        m_IPCNetEventHandler.onJSONString = onJSONString;
        m_IPCNetEventHandler.onStatus = onStatus;
        m_IPCNetEventHandler.onVideoData = onVideoData;
    }


    IPCNetServer::~IPCNetServer()
    {
    }

    bool IPCNetServer::initialize()
    {
        m_thWork = std::thread(&IPCNetServer::Work, this);
        return true;
    }

    void IPCNetServer::destroy()
    {
        LogInfo("IPCNetServer::destroy");
        std::unique_lock <std::mutex> lck(m_mxTmp);
        m_covQuit.notify_all();
        if (m_thWork.joinable())
        {
            m_thWork.join();
        }
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

        auto waitREt = m_covQuit.wait_for(lck, std::chrono::milliseconds(1000));
        while (waitREt == std::cv_status::timeout)
        {
            while (m_queWorkList.size())
            {
                ITask::Ptr pCurTask = PopTask();
                if (pCurTask)
                {
                    pCurTask->Run();
                }
            }
            waitREt = m_covQuit.wait_for(lck, std::chrono::milliseconds(1000));
        }

        IPCNetDeInitial();
        LogInfo("IPCNetServer End Work");
    }

    void IPCNetServerCallBack::OnDeviceConnected(const std::string& strDevJsonInfo)
    {
        LogInfo("recv device info %s", strDevJsonInfo.c_str());
        DeviceData stDeviceData;
        QJsonParseError jsonError;
        QJsonDocument doc;

        doc.fromJson(QByteArray::fromStdString(strDevJsonInfo), &jsonError);
        if (!doc.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                stDeviceData.name = utils::GetValueFromJsonObj(obj, "name").toString().toStdString();
                stDeviceData.p2p_uuid = utils::GetValueFromJsonObj(obj, "p2p_uuid").toString().toStdString();
                stDeviceData.app_ver = utils::GetValueFromJsonObj(obj, "app_ver").toString().toStdString();
                stDeviceData.sys_ver = utils::GetValueFromJsonObj(obj, "sys_ver").toString().toStdString();
                stDeviceData.pid = utils::GetValueFromJsonObj(obj, "pid").toString().toStdString();
                QJsonArray videoArray = utils::GetValueFromJsonObj(obj, "video_input").toArray();
                int nSize = videoArray.size();
                for (int i = 0; i < nSize; ++i)
                {
                    QJsonValue value = videoArray.at(i);
                    if (value.isObject())
                    {
                        VideoChannel videoChannel;
                        QJsonObject videoObj = value.toObject();
                        videoChannel.ch_no = utils::GetValueFromJsonObj(videoObj, "ch_no").toInt();
                        videoChannel.audio_code_type = utils::GetValueFromJsonObj(videoObj, "audio_code_type").toInt();
                        videoChannel.audio_sample_freq = utils::GetValueFromJsonObj(videoObj, "audio_sample_freq").toInt();
                        QJsonArray reslArray = utils::GetValueFromJsonObj(videoObj, "Resl").toArray();
                        if (reslArray.size() == 2)
                        {
                            QJsonObject maxSize = reslArray.at(0).toObject();
                            QJsonObject minSize = reslArray.at(1).toObject();
                            videoChannel.width_max = utils::GetValueFromJsonObj(maxSize, "width").toInt();
                            videoChannel.height_max = utils::GetValueFromJsonObj(maxSize, "height").toInt();
                            videoChannel.width_min = utils::GetValueFromJsonObj(minSize, "height").toInt();;
                            videoChannel.height_min = utils::GetValueFromJsonObj(minSize, "height").toInt();;
                        }
                        videoChannel.chann_name = utils::GetValueFromJsonObj(obj, "chann_name").toString().toStdString();
                        stDeviceData.video_input.push_back(videoChannel);
                    }
                }
            }
        }
        else
        {
            LogWarning("Parse json failed error %d", jsonError.error);
        }

        utils::TravelVector(m_CBFunc, [&stDeviceData](auto func) {
            if (func && func->funcOnDeviceConnected)
            {
                func->funcOnDeviceConnected(stDeviceData);
            }
            return false;
        });
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
}
