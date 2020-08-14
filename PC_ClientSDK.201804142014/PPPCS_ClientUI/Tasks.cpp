#include "Tasks.h"
#include "LogManager.h"
namespace ls
{

    ConnectTask::ConnectTask(std::string& strUID, std::string& strPwd, IPCNetEventHandler* pIPCNetEventHandler)
        :m_strPwd(strPwd)
    {
        m_strUID = strUID;
        m_pIPCNetEventHandler = pIPCNetEventHandler;
    }

    void ConnectTask::Run()
    {
        LogInfo("Start Connect %s", m_strUID.c_str());
        IPCNetStartIPCNetSession(m_strUID.c_str(), m_strPwd.c_str(), m_pIPCNetEventHandler);
    }

    DisconnectTask::DisconnectTask(std::string& strUID)
    {
        m_strUID = strUID;
    }

    void DisconnectTask::Run()
    {
        LogInfo("DisConnect %s", m_strUID.c_str());
        IPCNetStopIPCNetSession(m_strUID.c_str());
    }

    AudioTask::AudioTask(std::string& strUID, bool bStatu)
        :m_bStatu(bStatu)
    {
        m_strUID = strUID;
    }

    void AudioTask::Run()
    {
        if (m_bStatu)
        {
            IPCNetStartAudio(m_strUID.c_str());
        }
        else
        {
            IPCNetStopAudio(m_strUID.c_str());
        }
        LogInfo("Audio Control %s %s", m_strUID.c_str(), m_bStatu ?"Strat":"Stop");
    }

    VideoTask::VideoTask(std::string& strUID, bool bStatu, int param1 /*= 0*/)
        :m_bStatu(bStatu)
        ,m_param1(param1)
    {
        m_strUID = strUID;
    }

    void VideoTask::Run()
    {
        if (m_bStatu)
        {
            IPCNetStartVideo(m_strUID.c_str(), m_param1);
        }
        else
        {
            IPCNetStopVideo(m_strUID.c_str());
        }
        LogInfo("Video Control %s %s", m_strUID.c_str(), m_bStatu ? "Strat" : "Stop");
    }

}