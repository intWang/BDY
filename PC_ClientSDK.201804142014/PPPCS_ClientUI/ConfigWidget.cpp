#include "ConfigWidget.h"
#include "utils.h"
#include "IServer.h"
#include "MessageBoxWnd.h"
#include "JSONStructProtocal.h"
#include "JSONObject.hpp"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <QtNetwork/QSsl>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

static std::map<std::string /*shortCode*/, std::string /*longCode*/> g_transferCode;
ConfigWidget::ConfigWidget(ConfigType type, QWidget* parent)
    :MovelabelWidget<AreableWidget<QWidget>>(parent)
{
    QVBoxLayoutPtr pLayout = MQ(QVBoxLayout)(this);
    QFormLayoutPtr pCenterLayout = MQ(QFormLayout)(this);
    QHBoxLayoutPtr pReturnLayout = MQ(QHBoxLayout)(this);
    SetArea(50, 0);
    SetBorder(4);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    auto pTopBar = InitTopBar();
    if (pTopBar)
    {
        pLayout->addWidget(pTopBar);
    }

    switch (type)
    {
    case AddDevice:
        m_pCurData = std::make_shared<AddDeviceData>();
        CreateWidget4Device(pCenterLayout);
        resize(600, 400);
        break;
    case AddGroup:
        m_pCurData = std::make_shared<AddGroupData>();
        CreateWidget4Group(pCenterLayout);
        resize(600, 300);
        break;
        default:
        break;
    }

    pLayout->addLayout(pCenterLayout);
    CreateReturnBtn(pReturnLayout);
    pLayout->addLayout(pReturnLayout);

    setLayout(pLayout);
}


ConfigWidget::~ConfigWidget()
{
}


BarWidget::Ptr ConfigWidget::InitTopBar()
{
    auto pBarWidget = GetTopWnd();
    if (pBarWidget)
    {
        if (auto pTopLayout = pBarWidget->GetLayout())
        {
            auto pBtnClos = MQ(QPushButton)(pBarWidget);
            pBtnClos->setFixedSize(30, 30);
            pBtnClos->setObjectName("closeButtonNor");
            pTopLayout->addStretch();
            pTopLayout->addWidget(pBtnClos);

            connect(pBtnClos, &QPushButton::clicked, this, [this]() {
                this->close();
            });
        }
    }
    return pBarWidget;

}

void ConfigWidget::CreateWidget4Device(QFormLayoutPtr pLayout)
{
    if (!pLayout)
    {
        LogError("pLayout is empty!");
    }

    auto pLabelUID = MQ(QLabel)(this);
    auto pLabelName = MQ(QLabel)(this);
    auto pLabelPwd = MQ(QLabel)(this);
    auto pLayoutPwd = MQ(QHBoxLayout)(this);
    auto pinputDevName = MQ(QLineEdit)(this);
    auto pinputUID = MQ(QLineEdit)(this);
    auto pinputPwd = MQ(QLineEdit)(this);
    auto pchkShowPwd = MQ(QCheckBox)(this);

    pLabelUID->setText(QStringLiteral("设备编号"));
    pLabelPwd->setText(QStringLiteral("设备密码"));
    pLabelName->setText(QStringLiteral("设备名称"));
    pinputPwd->setEchoMode(QLineEdit::Password);
    pchkShowPwd->setText(QStringLiteral("显示密码"));

    connect(pchkShowPwd, &QCheckBox::clicked, this, [pinputPwd](bool checked) {
        pinputPwd->setEchoMode(checked?QLineEdit::Normal: QLineEdit::Password);
    });

    pLayoutPwd->addWidget(pinputPwd);
    pLayoutPwd->addWidget(pchkShowPwd);

    pLayout->addRow(pLabelUID, pinputUID);
    pLayout->addRow(pLabelName, pinputDevName);
    pLayout->addRow(pLabelPwd, pLayoutPwd);

    pLayout->setContentsMargins(40, 40, 40, 20);

    m_pInputUID = pinputUID;
    m_pInputPwd = pinputPwd;
    m_pInputName = pinputDevName;

    m_pManager = new QNetworkAccessManager(this);
    if (m_pManager)
    {
        connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnHttpReplyFinished(QNetworkReply*)));
    }
}

void ConfigWidget::CreateWidget4Group(QFormLayoutPtr pLayout)
{
    if (!pLayout)
    {
        LogError("pLayout is empty!");
    }

    auto pLabelGroupName = MQ(QLabel)(this);
    auto pinputGroupName = MQ(QLineEdit)(this);

    pLabelGroupName->setText(QStringLiteral("分组名称"));

    pLayout->addRow(pLabelGroupName, pinputGroupName);
    pLayout->setContentsMargins(40, 40, 40, 20);
    m_pInputGroupName = pinputGroupName;
}

void ConfigWidget::CreateReturnBtn(QHBoxLayoutPtr pLayout)
{
    if (!pLayout)
    {
        LogError("pLayout is empty!");
    }

    auto btnOK = MQ(QPushButton)(this);
    auto btnCancel = MQ(QPushButton)(this);

    btnOK->setText(QStringLiteral("确定"));
    btnOK->setProperty("usage", "confirm");
    btnCancel->setText(QStringLiteral("取消"));
    pLayout->addStretch();
    pLayout->addWidget(btnOK);
    pLayout->addWidget(btnCancel);

    m_pbtnOK = btnOK;
    m_pbtnCancel = btnCancel;
    connect(btnOK, &QPushButton::clicked, this, [&]() {
        this->OnOk();
    });

    connect(btnCancel, &QPushButton::clicked, this, [&]() {
        this->OnCancel();
    });
}

void ConfigWidget::CollectData4Device()
{
    if (!utils::CheckPointer({ m_pInputUID, m_pInputPwd, m_pInputName }))
    {
        LogError("param error %d %d", m_pInputUID, m_pInputPwd, m_pInputName);
    }

    auto pRealData = std::dynamic_pointer_cast<AddDeviceData>(m_pCurData);
    pRealData->strUID = m_pInputUID->text().toStdString();
    pRealData->strPwd = m_pInputPwd->text().toStdString();
    pRealData->strDevName = m_pInputName->text().toStdString();

    if (!pRealData->strUID.size())
    {
        msg::showError(this, QStringLiteral("警告"), QStringLiteral("设备编号不能为空，请检查输入！"));
    }

    if (!pRealData->strDevName.size())
    {
        msg::showError(this, QStringLiteral("警告"), QStringLiteral("设备名称不能为空，请检查输入！"));
    }

    if (pRealData->strUID.size() == 6 && g_transferCode.find(pRealData->strUID) == g_transferCode.end())
    {
        RequestLongCode(pRealData->strUID);
    }
}

void ConfigWidget::CollectData4Group()
{
    if (!utils::CheckPointer({  m_pInputGroupName }))
    {
        LogError("param error %d", m_pInputGroupName);
    }

    auto pRealData = std::dynamic_pointer_cast<AddGroupData>(m_pCurData);
    pRealData->strGroupName = m_pInputGroupName->text().toStdString();
}

void ConfigWidget::OnOk()
{
    auto pData = GetConfigData();
    if (!pData)
    {
        return;
    }
    switch (pData->emType)
    {
    case ConfigType::AddDevice:
        CollectData4Device();
        if (m_bRequesting)
        {
            return;
        }
        break;
    case ConfigType::AddGroup:
        CollectData4Group();
        break;
    default:
        break;
    }
    emit OnDataConfiged(pData);
    close();
}

void ConfigWidget::OnCancel()
{
    this->close();
}

void ConfigWidget::RequestLongCode(std::string& strShortCode)
{
    if (m_pManager && !m_bRequesting)
    {
        QString strUrl = "https://lyzb.hubanpay.com/huban-camera/api/device/infoByUid/";
        strUrl += QString::fromStdString(strShortCode);
        QNetworkRequest request;
        QSslConfiguration config;

        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(config);

        request.setHeader(QNetworkRequest::UserAgentHeader
            , "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36");

        request.setUrl(QUrl(strUrl));
        auto replay = m_pManager->get(request);
        m_bRequesting = true;

        m_pbtnOK->setEnabled(false);
        m_pbtnCancel->setEnabled(false);
    }

}

void ConfigWidget::OnHttpReplyFinished(QNetworkReply* replay)
{
    m_bRequesting = false;
    if (replay->error() == QNetworkReply::NoError)
    {
        std::string strUid = "";
        QByteArray bytes = replay->readAll();

        PJson::JSONObject jsdata(bytes.toStdString().c_str());
        if (jsdata.isValid())
        {
            PJson::JSONObject *jsdevice = jsdata.getJSONObject("device");
            if (jsdevice && jsdevice->isValid())
            {
                jsdevice->getString("uid", strUid);

                delete jsdevice;
                jsdevice = nullptr;
            }
        }

        if (!strUid.size())
        {
            msg::showError(this, QStringLiteral("错误"), bytes);
        }
        else
        {
            m_pInputUID->setText(QString::fromStdString(strUid));
            OnOk();
        }
    }
    else
    {
        QString strTitle = QStringLiteral("错误") + "  " + QVariant::fromValue(replay->error()).toString();
        msg::showError(this, strTitle,QStringLiteral("输入的设备ID无效，请检查后重新输入！"));
        m_pbtnOK->setEnabled(true);
        m_pbtnCancel->setEnabled(true);
    }
}

