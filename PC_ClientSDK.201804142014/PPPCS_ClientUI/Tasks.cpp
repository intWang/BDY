#include "IServer.h"
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

    VideoTask::VideoTask(std::string& strUID, bool bStatu, OnCmdResult_t pCB, int param1 /*= 0*/)
        :m_bStatu(bStatu)
        ,m_param1(param1)
        , m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void VideoTask::Run()
    {
        if (m_bStatu)
        {
            IPCNetStartVideoR(m_strUID.c_str(), m_param1, m_pCB);
        }
        else
        {
            IPCNetStopVideoR(m_strUID.c_str(), m_pCB);
        }
        LogInfo("Video Control %s %s", m_strUID.c_str(), m_bStatu ? "Strat" : "Stop");
    }

    VideoParamTask::VideoParamTask(std::string& strUID, bool bSet, IIPCNetServer::VideoParam tagParam, OnCmdResult_t pCB, int nParamValue /*= 0*/)
        : m_nParamValue(nParamValue)
        , m_tagParam(tagParam)
        , m_bSet(bSet)
    {
        m_strUID = strUID;
        m_pCB = pCB;
    }

    void VideoParamTask::Run()
    {
        Exec();
    }

    int VideoParamTask::Exec()
    {
        int nRet = 0;
        if (m_bSet)
        {
            switch (m_tagParam)
            {
            case ls::IIPCNetServer::Brightness:
                nRet = IPCNetSetBrightness(m_strUID.c_str(), m_nParamValue);
                break;
            case ls::IIPCNetServer::Tone:
                nRet = IPCNetSetHue(m_strUID.c_str(), m_nParamValue);
                break;
            case ls::IIPCNetServer::Contrast:
                nRet = IPCNetSetContrast(m_strUID.c_str(), m_nParamValue);
                break;
            case ls::IIPCNetServer::Saturation:
                nRet = IPCNetSetSaturation(m_strUID.c_str(), m_nParamValue);
                break;
            case ls::IIPCNetServer::Restore:
                nRet = IPCNetSetCameraColorSettingDefault(m_strUID.c_str());
                break;
            default:
                break;
            }
        }
        else
        {
            if (m_pCB)
            {
                switch (m_tagParam)
                {
                case ls::IIPCNetServer::Brightness:
                    nRet = IPCNetGetBrightnessR(m_strUID.c_str(), m_pCB);
                    break;
                case ls::IIPCNetServer::Tone:
                    nRet = IPCNetGetHueR(m_strUID.c_str(), m_pCB);
                    break;
                case ls::IIPCNetServer::Contrast:
                    nRet = IPCNetGetContrastR(m_strUID.c_str(), m_pCB);
                    break;
                case ls::IIPCNetServer::Saturation:
                    nRet = IPCNetGetSaturationR(m_strUID.c_str(), m_pCB);
                    break;
                default:
                    break;
                }
            }
        }

        LogInfo("VideoParam Exec %s %s %d %d Ret %d ", m_strUID.c_str(), m_bSet ? "Set" : "Get", m_tagParam, m_nParamValue, nRet);

        return nRet;
    }

    DeviceRestartTask::DeviceRestartTask(std::string& strUID, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void DeviceRestartTask::Run()
    {
        int nRet = IPCNetRebootDeviceR(m_strUID.c_str(), m_pCB);
        LogInfo("Exec IPCNetRebootDeviceR %s ret %d", m_strUID.c_str(), nRet);
    }

    DeviceResetTask::DeviceResetTask(std::string& strUID, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void DeviceResetTask::Run()
    {
        int nRet = IPCNetRestoreToFactorySettingR(m_strUID.c_str(), m_pCB);
        LogInfo("Exec IPCNetRestoreToFactorySettingR %s ret %d", m_strUID.c_str(), nRet);
    }

    GetDeviceTimeTask::GetDeviceTimeTask(std::string& strUID, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void GetDeviceTimeTask::Run()
    {
        int nRet = IPCNetGetTimeR(m_strUID.c_str(), m_pCB);
        LogInfo("Exec IPCNetGetTimeR %s ret %d", m_strUID.c_str(), nRet);
    }

    GetWifiTask::GetWifiTask(std::string& strUID, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void GetWifiTask::Run()
    {
        int nRet = IPCNetGetWiFiR(m_strUID.c_str(), m_pCB);
        LogInfo("Exec IPCNetGetWiFiR %s ret %d", m_strUID.c_str(), nRet);
    }

    SetWifiTask::SetWifiTask(std::string& strUid, std::string& strSSID, std::string& strPwd, std::string& strEncType, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUid;
        m_strSSID = strSSID;
        m_strPwd = strPwd;
        m_strEncType = strEncType;
    }

    void SetWifiTask::Run()
    {
        int nRet = IPCNetSetWiFiR(m_strUID.c_str(), m_strSSID.c_str(), m_strPwd.c_str(), m_strEncType.c_str(), m_pCB);
        LogInfo("Exec IPCNetSetWiFiR %s %s %s %s ret %d", m_strUID.c_str(), m_strSSID.c_str(), m_strPwd.c_str(), m_strEncType.c_str(), nRet);
    }

    ChangeDevPwdTask::ChangeDevPwdTask(std::string& strUID, std::string& strPwd, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
        m_strPwd = strPwd;
    }

    void ChangeDevPwdTask::Run()
    {
        int nRet = IPCNetChangeDevPwdR(m_strUID.c_str(), m_strPwd.c_str(), m_pCB);
        LogInfo("Exec IPCNetChangeDevPwdR %s %s ret %d", m_strUID.c_str(), m_strPwd.c_str(), nRet);
    }

    SearchWifiTask::SearchWifiTask(std::string& strUID, OnCmdResult_t pCB)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
    }

    void SearchWifiTask::Run()
    {
        int nRet = IPCNetSearchWiFiR(m_strUID.c_str(), m_pCB);
        LogInfo("Exec IPCNetSearchWiFiR %s ret %d", m_strUID.c_str(), nRet);
    }

    HotSpotTask::HotSpotTask(std::string& strUID, OnCmdResult_t pCB, const char* pJsonData /*= nullptr*/)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
        if (pJsonData)
        {
            m_strJsonParam = pJsonData;
        }
    }

    void HotSpotTask::Run()
    {
        int nRet = 0;
        if (m_strJsonParam.size())
        {
            nRet = IPCNetSetWiFiAPInfoR(m_strUID.c_str(), m_strJsonParam.c_str(), m_pCB);
            LogInfo("Exec IPCNetSetWiFiAPInfoR %s %s ret %d", m_strUID.c_str(), m_strJsonParam.c_str(), nRet);
        }
        else
        {
            nRet = IPCNetGetWiFiAPInfoR(m_strUID.c_str(), m_pCB);
            LogInfo("Exec IPCNetGetWiFiAPInfoR %s ret %d", m_strUID.c_str(), nRet);
        }
    }

    NetStrategyTask::NetStrategyTask(std::string& strUID, OnCmdResult_t pCB, const char* pJsonData /*= nullptr*/)
        :m_pCB(pCB)
    {
        m_strUID = strUID;
        if (pJsonData)
        {
            m_strJsonParam = pJsonData;
        }
    }

    void NetStrategyTask::Run()
    {
        int nRet = 0;
        if (m_strJsonParam.size())
        {
            nRet = IPCNetSetNetworkStrategyR(m_strUID.c_str(), m_strJsonParam.c_str(), m_pCB);
            LogInfo("Exec IPCNetSetNetworkStrategyR %s %s ret %d", m_strUID.c_str(), m_strJsonParam.c_str(), nRet);
        }
        else
        {
            nRet = IPCNetGetNetworkStrategyR(m_strUID.c_str(), m_pCB);
            LogInfo("Exec IPCNetGetNetworkStrategyR %s ret %d", m_strUID.c_str(), nRet);
        }
    }

    ResolutionTask::ResolutionTask(std::string& strUID, bool bSet, int nResolution, OnCmdResult_t pCB)
        :m_pCB(pCB)
        , m_bSet(bSet)
        , m_nResolution(nResolution)
    {
        m_strUID = strUID;
    }

    void ResolutionTask::Run()
    {
        int nRet = 0;
        if (m_bSet)
        {
            nRet = IPCNetSetResolutionR(m_strUID.c_str(), m_nResolution, m_pCB);
            LogInfo("Exec IPCNetSetResolutionR %s %d ret %d", m_strUID.c_str(), m_nResolution, nRet);
        }
        else
        {
            nRet = IPCNetGetResolutionR(m_strUID.c_str(), m_pCB);
            LogInfo("Exec IPCNetGetResolutionR %s ret %d", m_strUID.c_str(), nRet);
        }
    }

    FlipMirrorTask::FlipMirrorTask(std::string& strUID, bool bSet, int nFlip, int nMirror, OnCmdResult_t pCB)
        :m_pCB(pCB)
        , m_nFlip(nFlip)
        , m_nMirror(nMirror)
        , m_bSet(bSet)
    {
        m_strUID = strUID;
    }

    void FlipMirrorTask::Run()
    {
        int nRet = 0;
        if (m_bSet)
        {
            nRet = IPCNetSetFlipMirrorR(m_strUID.c_str(), m_nFlip, m_nMirror, m_pCB);
            LogInfo("Exec IPCNetSetFlipMirrorR %s %d %d ret %d", m_strUID.c_str(), m_nFlip, m_nMirror, nRet);
        }
        else
        {
            nRet = IPCNetGetFlipMirrorR(m_strUID.c_str(), m_pCB);
            LogInfo("Exec IPCNetGetFlipMirrorR %s ret %d", m_strUID.c_str(), nRet);
        }
    }

    SwitchStreamTask::SwitchStreamTask(std::string& strUID, int nStream, OnCmdResult_t pCB)
        :m_pCB(pCB)
        ,m_nStream(nStream)
    {
        m_strUID = strUID;
    }

    void SwitchStreamTask::Run()
    {
        int nRet = IPCNetStopVideo(m_strUID.c_str());
        LogInfo("Exec IPCNetStopVideo %s ret %d", m_strUID.c_str(), nRet);
        nRet = IPCNetStartVideoR(m_strUID.c_str(), m_nStream, m_pCB);
        LogInfo("Exec IPCNetStartVideoR %s ret %d", m_strUID.c_str(), nRet);
    }

}