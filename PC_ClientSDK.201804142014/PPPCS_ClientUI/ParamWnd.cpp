#include "ParamWnd.h"
#include <vector>

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
    
    std::map<QSlider*, QLabel*> mapSlds =
    { 
        {ui.sldBrightness, ui.lblBrightness}, 
        {ui.sldTone , ui.lblTone},
        {ui.sldContrast, ui.lblContrast},
        {ui.sldSaturation, ui.lblSaturation}
    };

    for (auto pair: mapSlds)
    {
        auto pSld = pair.first;
        auto pLabel = pair.second;
        pSld->setMinimum(0);
        pSld->setMaximum(255);
        pSld->setSingleStep(1);
        pSld->setPageStep(10);

        connect(pSld, &QSlider::valueChanged, this, [pLabel](int nValue) {
            pLabel->setText(QVariant(nValue).toString());
        });
    }

    setEnabled(false);
}

ParamWnd::~ParamWnd()
{
    
}

void ParamWnd::BindDevNode(DevNode::Ptr pDevNode)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    m_pDevNode = pDevNode;
    UpdateData();
    if (m_pDevNode)
    {
        setEnabled(true);
    }
    else
    {
        setEnabled(false);
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
    ui.sldBrightness->setValue(m_origionalData.brightness);
    ui.sldContrast->setValue(m_origionalData.contrast);
    ui.sldSaturation->setValue(m_origionalData.saturation);
    ui.sldTone->setValue(m_origionalData.tone);
}

void ParamWnd::Applay()
{
    if (m_pDevNode && m_pServer)
    {
        int value = ui.sldBrightness->value();
        if (value != m_origionalData.brightness)
        {
            m_origionalData.brightness = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Brightness, value);
        }

        value = ui.sldContrast->value();
        if (value != m_origionalData.contrast)
        {
            m_origionalData.contrast = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Contrast, value);
        }

        value = ui.sldSaturation->value();
        if (value != m_origionalData.saturation)
        {
            m_origionalData.saturation = value;
            m_pServer->VideoParamCtrl(m_pDevNode->GetDevUid(), true, ls::IIPCNetServer::VideoParam::Saturation, value);
        }

        value = ui.sldTone->value();
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
