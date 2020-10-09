#include "SubPreviewWnd.h"
#include "QtDefine.h"
#include "utils.h"

SubPreviewWnd::SubPreviewWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    SetArea(50, 30);
    SetAreaBk(s_qcl444858, s_qcl292C39, s_qcl292C39);
    auto pMainLayout = MQ(QVBoxLayout)(this);
    m_pPreviewWnd = new PreviewRealWnd(-1, this);
    auto pDevideLine = MQ(QLabel)(this);

    pDevideLine->setObjectName("DevideLine");
    pDevideLine->setFrameShape(QFrame::HLine);
    pDevideLine->setFrameShadow(QFrame::Raised);
    pDevideLine->setFixedHeight(2);
    pMainLayout->addWidget(InitTopBar());
    pMainLayout->addWidget(m_pPreviewWnd);
    pMainLayout->addWidget(pDevideLine);
    pMainLayout->addWidget(InitBottomBar());
    pMainLayout->setContentsMargins(0,0,0,0);
    setLayout(pMainLayout);

    connect(m_pPreviewWnd, &PreviewRealWnd::PreviewWndStopPreview, this, &SubPreviewWnd::OnPreviewStoped);
    connect(m_pPreviewWnd, &PreviewRealWnd::PreviewWndStartPreview, this, &SubPreviewWnd::OnPreviewStarted);
    EnableBottom(false);
}

SubPreviewWnd::~SubPreviewWnd()
{
}

void SubPreviewWnd::StopPreview()
{
    if (m_pPreviewWnd)
    {
        m_pPreviewWnd->StopPreview();
        emit SelectPreviewWnd(nullptr);
    }
}

void SubPreviewWnd::OnStartPreview(DevNode::Ptr pChannel)
{
    if (m_bEnterSnapMode)
    {
        if (m_pPreviewWnd)
        {
            m_pPreviewWnd->StartPreview(pChannel);
            emit SelectPreviewWnd(m_pPreviewWnd);
        }
    }
}

void SubPreviewWnd::OnEnterSnapMode()
{
    m_bEnterSnapMode = true;
}

void SubPreviewWnd::OnLeaveSnapMode()
{
    if (m_pPreviewWnd)
    {
        m_pPreviewWnd->StopPreview();
    }
    m_bEnterSnapMode = false;
}

void SubPreviewWnd::OnPreviewStarted(const QString& strUid, PreviewRealWnd::Ptr pWnd)
{
    EnableBottom(true);
    emit PreviewStatuChanged(strUid, true);
}

void SubPreviewWnd::OnPreviewStoped(const QString& strUid, PreviewRealWnd::Ptr pWnd)
{
    EnableBottom(false);
    emit PreviewStatuChanged(strUid, false);
}

void SubPreviewWnd::OnBtnClicked()
{
    auto pBtnSender = qobject_cast<QPushButtonPtr>(sender());
    PtzCommand emCmd = PtzCommand::ZoomIn;
    if (pBtnSender == m_pBtnZoomIn)
    {
        emCmd = PtzCommand::ZoomIn;
    }
    else if (pBtnSender == m_pBtnZoomOut)
    {
        emCmd = PtzCommand::ZoomOut;
    }
    else if (pBtnSender == m_pBtnLeft)
    {
        emCmd = PtzCommand::MoveL;
    }
    else if (pBtnSender == m_pBtnRight)
    {
        emCmd = PtzCommand::MoveR;
    }
    else if (pBtnSender == m_pBtnUp)
    {
        emCmd = PtzCommand::MoveT;
    }
    else if (pBtnSender == m_pBtnDown)
    {
        emCmd = PtzCommand::MoveD;
    }
    else
    {
        return;
    }

    if (m_pPreviewWnd)
    {
        m_pPreviewWnd->OnPtzCtrl(emCmd, 0);
    }
}

BarWidget::Ptr SubPreviewWnd::InitBottomBar()
{
    auto pBottom = GetBottomWnd();
    if (pBottom)
    {
        if (auto pLayout = pBottom->GetLayout())
        {
            m_pBtnZoomIn = MQ(QPushButton)(pBottom);
            m_pBtnZoomOut = MQ(QPushButton)(pBottom);
            m_pBtnLeft = MQ(QPushButton)(pBottom);
            m_pBtnRight = MQ(QPushButton)(pBottom);
            m_pBtnUp = MQ(QPushButton)(pBottom);
            m_pBtnDown = MQ(QPushButton)(pBottom);

            std::vector<QPushButtonPtr> vcBtns = { m_pBtnZoomIn , m_pBtnZoomOut, m_pBtnLeft, m_pBtnRight, m_pBtnUp, m_pBtnDown };
            std::vector<QString> vcBtnNames = { "放大", "缩小", "左移", "右移", "上移", "下移" };
            pLayout->addStretch();
            for (int i = 0; i < vcBtns.size(); i++)
            {
                vcBtns[i]->setText(vcBtnNames[i]);
                vcBtns[i]->setFixedWidth(55);

                vcBtns[i]->setAutoRepeat(true); //启用长按
                vcBtns[i]->setAutoRepeatDelay(400);//触发长按的时间
                vcBtns[i]->setAutoRepeatInterval(100);//长按时click信号间隔

                connect(vcBtns[i], &QPushButton::clicked, this, &SubPreviewWnd::OnBtnClicked);
                pLayout->addWidget(vcBtns[i]);
            }
            pLayout->setContentsMargins(5, 0, 5, 0);
            pLayout->setSpacing(5);
        }
    }

    return pBottom;
}

BarWidget::Ptr SubPreviewWnd::InitTopBar()
{
    auto pTopBar = GetTopWnd();
    auto pTopLayout = pTopBar->GetLayout();
    auto pLabelTitle = MQ(QLabel)(this);
    pLabelTitle->setText("实时预览");

    pTopLayout->addWidget(pLabelTitle);
    pTopLayout->setContentsMargins(20,0,0,0);
    return pTopBar;
}

