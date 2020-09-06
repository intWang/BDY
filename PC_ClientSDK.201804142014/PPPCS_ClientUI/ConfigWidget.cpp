#include "ConfigWidget.h"
#include "utils.h"
#include "IServer.h"
#include "MessageBoxWnd.h"
#include "JSONStructProtocal.h"
#include "JSONObject.hpp"
#include "ConfigCenter.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <QtNetwork/QSsl>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

#include <QFileDialog>
ConfigWidget::ConfigWidget(ConfigType type, QWidget* parent, TreeNode::Ptr pNode)
    :BaseDialog(parent)
{
    QVBoxLayoutPtr pLayout = MQ(QVBoxLayout)(this);
    QFormLayoutPtr pCenterLayout = MQ(QFormLayout)(this);
    QHBoxLayoutPtr pReturnLayout = MQ(QHBoxLayout)(this);
    setMinimizeVisible(false);
    setMaximizeVisible(false);
    setWidgetResizable(false);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    SetArea(30, 0);
    SetBorder(4);
    SetNoBottomBar();

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
        setWindowTitle("添加设备");
        resize(400, 300);
        break;
    case AddGroup:
        m_pCurData = std::make_shared<AddGroupData>();
        CreateWidget4Group(pCenterLayout);
        setWindowTitle("添加分组");
        resize(400, 300);
        break;
    case StorageConfig:
        m_pCurData = std::make_shared<StorageData>();
        CreateWidget4Storage(pCenterLayout);
        setWindowTitle("存储设置");
        resize(600, 300);
        break;
    case ModifyDevice:
        m_pCurData = std::make_shared<ModDeviceData>(std::dynamic_pointer_cast<DevNode>(pNode));
        CreateWidget4Device(pCenterLayout, true);
        setWindowTitle("修改设备");
        resize(400, 300);
        break;
    case ModifyGroup:
        m_pCurData = std::make_shared<ModGroupData>(std::dynamic_pointer_cast<GroupNode>(pNode));
        CreateWidget4Group(pCenterLayout, true);
        setWindowTitle("修改分组");
        resize(400, 300);
        break;
        default:
        break;
    }

    pCenterLayout->setSpacing(10);
    pLayout->addLayout(pCenterLayout);
    CreateReturnBtn(pReturnLayout);
    pLayout->addLayout(pReturnLayout);
    pLayout->setContentsMargins(20, 30, 20, 30);

    GetLayout()->addLayout(pLayout);
}


ConfigWidget::~ConfigWidget()
{
}

void ConfigWidget::CreateWidget4Device(QFormLayoutPtr pLayout, bool bModify )
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

    pLayout->addRow(pLabelName, pinputDevName);
    pLayout->addRow(pLabelUID, pinputUID);
    pLayout->addRow(pLabelPwd, pLayoutPwd);

    //pLayout->setContentsMargins(40, 40, 40, 20);
    m_pInputUID = pinputUID;
    m_pInputPwd = pinputPwd;
    m_pInputName = pinputDevName;

    m_pManager = new QNetworkAccessManager(this);
    if (m_pManager)
    {
        connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnHttpReplyFinished(QNetworkReply*)));
    }

    if (bModify)
    {
        if (auto pModData = std::dynamic_pointer_cast<ModDeviceData>(m_pCurData))
        {
            if (pModData->strShortUID.size())
            {
                m_pInputUID->setText(QString::fromStdString(pModData->strShortUID));
            }
            else
            {
                m_pInputUID->setText(QString::fromStdString(pModData->strUID));
            }
            m_pInputPwd->setText(QString::fromStdString(pModData->strPwd));
            m_pInputName->setText(QString::fromStdString(pModData->strDevName));
            if (!(pModData->nModLevel & ModDeviceData::MOD_NAME))
            {
                m_pInputName->setEnabled(false);
            }
            if (!(pModData->nModLevel & ModDeviceData::MOD_UID))
            {
                m_pInputUID->setEnabled(false);
            }
            if (!(pModData->nModLevel & ModDeviceData::MOD_PWD))
            {
                m_pInputPwd->setEnabled(false);
            }
        }
    }
}

void ConfigWidget::CreateWidget4Group(QFormLayoutPtr pLayout, bool bModify)
{
    if (!pLayout)
    {
        LogError("pLayout is empty!");
    }

    auto pLabelGroupName = MQ(QLabel)(this);
    auto pinputGroupName = MQ(QLineEdit)(this);

    pLabelGroupName->setText(QStringLiteral("分组名称"));

    pLayout->addRow(pLabelGroupName, pinputGroupName);
    //pLayout->setContentsMargins(40, 40, 40, 20);
    m_pInputGroupName = pinputGroupName;

    if (bModify)
    {
        if (auto pModData = std::dynamic_pointer_cast<ModGroupData>(m_pCurData))
        {
            m_pInputGroupName->setText(QString::fromStdString(pModData->strGroupName));
        }
    }
}

void ConfigWidget::CreateWidget4Storage(QFormLayoutPtr pLayout)
{
    auto pLayoutChooseFile1 = MQ(QHBoxLayout)(this);
    auto pLayoutChooseFile2 = MQ(QHBoxLayout)(this);
    auto pInpuDir1 = MQ(QLineEdit)(this);
    auto pInpuDir2 = MQ(QLineEdit)(this);
    auto pBtn1 = MQ(QPushButton)(this);
    auto pBtn2 = MQ(QPushButton)(this);
    pBtn1->setObjectName("choose_folder");
    pBtn2->setObjectName("choose_folder");

    pBtn1->setFixedSize({ 40,40 });
    pBtn2->setFixedSize({ 40,40 });

    pLayoutChooseFile1->addWidget(pInpuDir1);
    pLayoutChooseFile1->addWidget(pBtn1);

    pLayoutChooseFile2->addWidget(pInpuDir2);
    pLayoutChooseFile2->addWidget(pBtn2);

    pLayout->addRow("录像保存路径", pLayoutChooseFile1);
    pLayout->addRow("截图保存路径", pLayoutChooseFile2);

    pInpuDir1->setPlaceholderText("点击按钮选择录像文件保存路径");
    pInpuDir2->setPlaceholderText("点击按钮选择截图文件保存路径");

    pInpuDir1->setEnabled(false);
    pInpuDir2->setEnabled(false);

    pInpuDir1->setText(ConfigCenter::GetInstance().GetRecordSavepath());
    pInpuDir2->setText(ConfigCenter::GetInstance().GetSnapShotSavepath());
    auto funcChooseFile = [this, pBtn1, pBtn2, pInpuDir1, pInpuDir2]() {
        auto pBtn = qobject_cast<QPushButtonPtr>(this->sender());
        if (pBtn == pBtn1)
        {
            auto strSel = this->ChooseFilePath(ConfigCenter::GetInstance().GetRecordSavepath());
            if (!strSel.isEmpty())
            {
                ConfigCenter::GetInstance().SetRecordSavepath(strSel);
                pInpuDir1->setText(strSel + '/');
            }
        }
        else if(pBtn == pBtn2)
        {
            auto strSel = this->ChooseFilePath(ConfigCenter::GetInstance().GetSnapShotSavepath());
            if (!strSel.isEmpty())
            {
                ConfigCenter::GetInstance().SetSnapShotSavepath(strSel);
                pInpuDir2->setText(strSel + '/');
            }
        }
    };

    connect(pBtn1, &QPushButton::clicked, this, funcChooseFile);
    connect(pBtn2, &QPushButton::clicked, this, funcChooseFile);
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
    pLayout->setSpacing(10);

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
    pRealData->strShortUID = m_pInputUID->text().toStdString();
    pRealData->strPwd = m_pInputPwd->text().toStdString();
    pRealData->strDevName = m_pInputName->text().toStdString();

    if (!pRealData->strShortUID.size())
    {
        msg::showError(this, QStringLiteral("警告"), QStringLiteral("设备编号不能为空，请检查输入！"));
    }

    if (!pRealData->strDevName.size())
    {
        m_pInputName->setText(m_pInputUID->text());
        pRealData->strDevName = pRealData->strShortUID;
    }

    if (pRealData->strUID.size() <= 6 )
    {
        RequestLongCode(pRealData->strShortUID);
    }
    else
    {
        pRealData->strUID = pRealData->strShortUID;
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
        emit OnDataConfiged(pData);
        break;
    case ConfigType::AddGroup:
        CollectData4Group();
        emit OnDataConfiged(pData);
        break;
    case ConfigType::ModifyDevice:
        CollectData4Device();
        emit OnDataModified(pData);
        break;
    case ConfigType::ModifyGroup:
        CollectData4Group();
        emit OnDataModified(pData);
        break;
    default:
        break;
    }
    done(0);
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

QString ConfigWidget::ChooseFilePath(QString& strDefultPath)
{
    return QFileDialog::getExistingDirectory(this, "选择文件夹", strDefultPath);
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
            //m_pInputUID->setText(QString::fromStdString(strUid));
            if (auto pRealData = std::dynamic_pointer_cast<AddDeviceData>(m_pCurData))
            {
                pRealData->strUID = strUid;
            }
            emit OnDataConfiged(m_pCurData);

            done(0);
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

