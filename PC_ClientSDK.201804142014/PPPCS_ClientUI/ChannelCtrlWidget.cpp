#include "ChannelCtrlWidget.h"
#include "QtDefine.h"
#include <QApplication>

ChannelCtrlWidget::ChannelCtrlWidget(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    SetArea(m_nTopHeight, 0);
    SetAreaBk(s_qcl444858, s_qcl292C39, s_qcl292C39);
    auto pLayout = MQ(QVBoxLayout)(this);
    m_pStackWidget = MQ(QStackedWidget)(this);
    auto pPTZWnd = MQ(PtzCtrlWnd)(this);
    auto pParamWnd = MQ(ParamWnd)(this);
    auto pDecodeWnd = MQ(DecodeWnd)(this);
    m_pStackWidget->addWidget(pPTZWnd);
    m_pStackWidget->addWidget(pParamWnd);
    m_pStackWidget->addWidget(pDecodeWnd);

    pLayout->addWidget(InitTopBar());
    pLayout->addWidget(m_pStackWidget);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    connect(this, &ChannelCtrlWidget::SelTab, this, [this](TabWnd emTab) {
        int nIndex = (int)emTab;
        if (m_pStackWidget)
        {
            m_pStackWidget->setCurrentIndex(nIndex);
        }
    });

    m_pPtzWnd = pPTZWnd;
    m_pParamWnd = pParamWnd;
}

ChannelCtrlWidget::~ChannelCtrlWidget()
{
}

PtzCtrlWnd::Ptr ChannelCtrlWidget::GetPtzCtrlWnd()
{
    return m_pPtzWnd;
}

void ChannelCtrlWidget::BindPreviewWnd(PreviewRealWnd::Ptr pWnd)
{
    m_pPreviewpWnd = pWnd;
    UpdatePtzWnd();
    UpdateParamWnd();
}

BarWidget::Ptr ChannelCtrlWidget::InitTopBar()
{
    auto pTop = GetTopWnd();
    auto pTopLayout = pTop->GetLayout();

    auto pBtnPtz = MQ(QPushButton)(this);
    auto pBtnParam = MQ(QPushButton)(this);
    auto pBtnDecode = MQ(QPushButton)(this);

    pBtnPtz->setFixedSize(60, m_nTopHeight);
    pBtnParam->setFixedSize(60, m_nTopHeight);
    pBtnDecode->setFixedSize(60, m_nTopHeight);

    pBtnPtz->setObjectName("TabBtn");
    pBtnPtz->setProperty("SelStatu", "On");
    pBtnPtz->setText("PTZ");
    pBtnParam->setObjectName("TabBtn");
    pBtnParam->setProperty("SelStatu", "Off");
    pBtnParam->setText("Param1");
    pBtnDecode->setObjectName("TabBtn");
    pBtnDecode->setProperty("SelStatu", "Off");
    pBtnDecode->setText("Decoder");

    pTopLayout->addWidget(pBtnPtz);
    pTopLayout->addWidget(pBtnParam);
    pTopLayout->addWidget(pBtnDecode);
    pTopLayout->addStretch();
    pTopLayout->setSpacing(0);

    auto btnClickFunc = [this, pBtnPtz, pBtnParam, pBtnDecode]() {
        if (sender() == pBtnPtz)
        {
            emit this->SelTab(TabWnd::Ptz);
            pBtnPtz->setProperty("SelStatu", "On");
            pBtnParam->setProperty("SelStatu", "Off");
            pBtnDecode->setProperty("SelStatu", "Off");
        }
        else if (sender() == pBtnParam)
        {
            emit this->SelTab(TabWnd::Param);
            pBtnParam->setProperty("SelStatu", "On");
            pBtnPtz->setProperty("SelStatu", "Off");
            pBtnDecode->setProperty("SelStatu", "Off");
        }
        else if (sender() == pBtnDecode)
        {
            emit this->SelTab(TabWnd::Decode);
            pBtnDecode->setProperty("SelStatu", "On");
            pBtnParam->setProperty("SelStatu", "Off");
            pBtnPtz->setProperty("SelStatu", "Off");
        }

        pBtnPtz->setStyle(QApplication::style());
        pBtnParam->setStyle(QApplication::style());
        pBtnDecode->setStyle(QApplication::style());
    };

    connect(pBtnPtz, &QPushButton::clicked, this, btnClickFunc);
    connect(pBtnParam, &QPushButton::clicked, this, btnClickFunc);
    connect(pBtnDecode, &QPushButton::clicked, this, btnClickFunc);

    return pTop;
}

void ChannelCtrlWidget::UpdateParamWnd()
{
    if (m_pParamWnd)
    {
        if (m_pPreviewpWnd)
        {
            m_pParamWnd->BindDevNode(m_pPreviewpWnd->GetDevNode());
        }
        else
        {
            m_pParamWnd->Untie();
        }
    }
}

void ChannelCtrlWidget::UpdatePtzWnd()
{
    if (!m_pPtzWnd)
    {
        return;
    }

    if (!m_pPreviewpWnd || !m_pPreviewpWnd->IsInPreview())
    {
        m_pPtzWnd->BindPreviewWnd(nullptr);
    }
    else if (m_pPreviewpWnd->IsInPreview())
    {
        m_pPtzWnd->BindPreviewWnd(m_pPreviewpWnd);
    }
}
