#include "ParamWnd.h"

ParamWnd::ParamWnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_pServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    LogInfo("Get IIPCNetServer %d", m_pServer.get());

    connect(ui.btnApply, &QPushButton::clicked, this, [this]() {
        this->Applay();
    });

    connect(ui.btnRestoreParam, &QPushButton::clicked, this, [this]() {
        this->Restore();
    });

    m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
    if (m_CallBackFunc)
    {
        m_CallBackFunc->funcOnVideoParamData = std::bind(&ParamWnd::OnVideoParamData, this, std::placeholders::_1, std::placeholders::_2);
    }
    auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
    if (pIPCCallBack)
    {
        pIPCCallBack->Register(m_CallBackFunc);
    }
}

ParamWnd::~ParamWnd()
{
    auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
    if (pIPCCallBack)
    {
        pIPCCallBack->UnRegister(m_CallBackFunc);
    }
}

void ParamWnd::BindDevNode(DevNode::Ptr pDevNode)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    m_pDevNode = pDevNode;
    UpdateData();
    if (m_pDevNode)
    {
        ui.spinBrightness->setEnabled(true);
        ui.spinContrast->setEnabled(true);
        ui.spinSaturation->setEnabled(true);
        ui.spinTone->setEnabled(true);
        ui.btnApply->setEnabled(true);
        ui.btnRestoreParam->setEnabled(true);
    }
    else
    {
        ui.spinBrightness->setEnabled(false);
        ui.spinContrast->setEnabled(false);
        ui.spinSaturation->setEnabled(false);
        ui.spinTone->setEnabled(false);
        ui.btnApply->setEnabled(false);
        ui.btnRestoreParam->setEnabled(false);
    }
}

void ParamWnd::Untie()
{
    BindDevNode(nullptr);
}

void ParamWnd::OnVideoParamData(const std::string& strUid, const IPCNetCamColorCfg_st& stParam)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    if (m_pDevNode && (strUid == m_pDevNode->GetDevUid()))
    {
        VideoParamData stData = { 0 };
        stData.brightness = stParam.Brightness;
        stData.saturation = stParam.Saturtion;
        stData.tone = stParam.Chroma;
        stData.contrast = stParam.Contrast;
        stData.tmLastUpdate = time(NULL);
        m_pDevNode->SetVideoParam(stData);
        UpdateData();
    }
}

void ParamWnd::UpdateData()
{
    if (m_pServer && m_pDevNode)
    {
        if (m_pDevNode->stVideoParam.isExpired())
        {
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), false, ls::IIPCNetServer::VideoParam::Brightness, 0);
//             m_origionalData.contrast = m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), false, ls::IIPCNetServer::VideoParam::Contrast, 0);
//             m_origionalData.saturation = m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), false, ls::IIPCNetServer::VideoParam::Saturation, 0);
//             m_origionalData.tone = m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), false, ls::IIPCNetServer::VideoParam::Tone, 0);
//             m_origionalData.tmLastUpdate = time(NULL);
//             m_pDevNode->SetVideoParam(m_origionalData);
        }
        else
        {

            m_origionalData = m_pDevNode->GetVideoParam();
        }
    }
    else
    {
        m_origionalData = { 0 };
    }
    ui.spinBrightness->setValue(m_origionalData.brightness);
    ui.spinContrast->setValue(m_origionalData.contrast);
    ui.spinSaturation->setValue(m_origionalData.saturation);
    ui.spinTone->setValue(m_origionalData.tone);
}

void ParamWnd::Applay()
{
    if (m_pDevNode && m_pServer)
    {
        int value = ui.spinBrightness->value();
        if (value != m_origionalData.brightness)
        {
            m_origionalData.brightness = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Brightness, value);
        }

        value = ui.spinContrast->value();
        if (value != m_origionalData.contrast)
        {
            m_origionalData.contrast = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Contrast, value);
        }

        value = ui.spinSaturation->value();
        if (value != m_origionalData.saturation)
        {
            m_origionalData.saturation = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Saturation, value);
        }

        value = ui.spinTone->value();
        if (value != m_origionalData.tone)
        {
            m_origionalData.tone = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Tone, value);
        }

        m_pDevNode->SetVideoParam(m_origionalData);
    }
}

void ParamWnd::Restore()
{
    if (m_pServer&& m_pDevNode)
    {
        m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Restore, 0);
        m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), false, ls::IIPCNetServer::VideoParam::Brightness, 0);
    }
}
