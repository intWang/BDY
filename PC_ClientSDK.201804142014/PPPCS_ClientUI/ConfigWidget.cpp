#include "ConfigWidget.h"
#include "utils.h"


ConfigWidget::ConfigWidget(ConfigType type, QWidget* parent)
    :AreableWidget<QWidget>(parent)
{
    QVBoxLayoutPtr pLayout = MQ(QVBoxLayout)(this);
    QFormLayoutPtr pCenterLayout = MQ(QFormLayout)(this);
    QHBoxLayoutPtr pReturnLayout = MQ(QHBoxLayout)(this);
    SetArea(50, 0);
    SetAreaBk(s_qcl444858, s_qcl444858, s_qcl444858);
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
    auto pLabelPwd = MQ(QLabel)(this);
    auto pLayoutPwd = MQ(QHBoxLayout)(this);
    auto pinputUID = MQ(QLineEdit)(this);
    auto pinputPwd = MQ(QLineEdit)(this);
    auto pchkShowPwd = MQ(QCheckBox)(this);

    pLabelUID->setText(QStringLiteral("设备编号"));
    pLabelPwd->setText(QStringLiteral("设备密码"));
    pinputPwd->setEchoMode(QLineEdit::Password);
    pchkShowPwd->setText(QStringLiteral("显示密码"));

    connect(pchkShowPwd, &QCheckBox::clicked, this, [pinputPwd](bool checked) {
        pinputPwd->setEchoMode(checked?QLineEdit::Normal: QLineEdit::Password);
    });

    pLayoutPwd->addWidget(pinputPwd);
    pLayoutPwd->addWidget(pchkShowPwd);

    pLayout->addRow(pLabelUID, pinputUID);
    pLayout->addRow(pLabelPwd, pLayoutPwd);

    pLayout->setContentsMargins(40, 40, 40, 20);

    m_pInputUID = pinputUID;
    m_pInputPwd = pinputPwd;
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

    connect(btnOK, &QPushButton::clicked, this, [&]() {
        this->OnOk();
    });

    connect(btnCancel, &QPushButton::clicked, this, [&]() {
        this->OnCancel();
    });
}

void ConfigWidget::CollectData4Device()
{
    if (!utils::CheckPointer({ m_pInputUID, m_pInputPwd}))
    {
        LogError("param error %d %d", m_pInputUID, m_pInputPwd);
    }

    auto pRealData = std::dynamic_pointer_cast<AddDeviceData>(m_pCurData);
    pRealData->strUID = m_pInputUID->text().toStdString();
    pRealData->strPwd = m_pInputPwd->text().toStdString();
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
