#include "CmdBar.h"
#include "LogManager.h"
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

#include "utils.h"

CmdBar::CmdBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(30);

    m_pIconLabel = MQ(QLabel)(this);
    m_pTitleLabel = MQ(QLabel)(this);
    m_pMinimizeButton = MQ(QPushButton)(this);
    m_pMaximizeButton = MQ(QPushButton)(this);
    m_pCloseButton = MQ(QPushButton)(this);
    QHBoxLayoutPtr pLayout = MQ(QHBoxLayout)(this);

    if (!utils::CheckPointer({ m_pIconLabel, m_pTitleLabel, m_pMinimizeButton, m_pMaximizeButton, m_pCloseButton, pLayout }))
    {
        LogError("Error param %d %d %d %d %d %d", m_pIconLabel, m_pTitleLabel, m_pMinimizeButton, m_pMaximizeButton, m_pCloseButton, pLayout);
    }

    m_pIconLabel->setFixedSize(20, 20);
    m_pIconLabel->setScaledContents(true);
    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pMinimizeButton->setFixedSize(30, 30);
    m_pMaximizeButton->setFixedSize(30, 30);
    m_pCloseButton->setFixedSize(30, 30);

    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    m_pMinimizeButton->setToolTip("最小化");
    m_pMaximizeButton->setToolTip("最大化");
    m_pCloseButton->setToolTip("关闭");

    pLayout->addWidget(m_pIconLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 0, 0);

    setLayout(pLayout);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}


CmdBar::~CmdBar()
{
}

void CmdBar::setMinimizeVisible(bool bVisiable)
{
    if (m_pMinimizeButton)
    {
        m_pMinimizeButton->hide();
    }
}

void CmdBar::setMaximizeVisible(bool bVisiable)
{
    if (m_pMaximizeButton)
    {
        m_pMaximizeButton->hide();
    }
}

void CmdBar::setWidgetResizable(bool bVisiable)
{
    
}

void CmdBar::FullScreen()
{
    QWidget *pWindow = this->window();
    if (pWindow && pWindow->isTopLevel() && !pWindow->isMaximized())
    {
        emit m_pMaximizeButton->clicked(0);
    }
}

void CmdBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_pMaximizeButton)
        {
            if (pWindow->isMaximized())
            {
                pWindow->showNormal();
                m_bMax = false;
            }
            else
            {
                pWindow->showMaximized();
                m_bMax = true;
            }
        }
        else if (pButton == m_pCloseButton)
        {
            emit OnCloseBtnClicked();
        }
    }
}

void CmdBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = m_bMax;
        if (bMaximize)
        {
            m_pMaximizeButton->setToolTip(tr("Restore"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setToolTip(tr("Maximize"));
        }

        m_pMaximizeButton->setStyle(QApplication::style());
    }
}

bool CmdBar::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(watched);
        if (pWidget)
        {
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(watched);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }

    return QWidget::eventFilter(watched, event);
}

void CmdBar::mouseReleaseEvent(QMouseEvent *event)
{
    ReleaseCapture();
}

void CmdBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
#else
#endif
}

void CmdBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit m_pMaximizeButton->clicked();
}

HintBar::HintBar(QWidget *parent /*= 0*/)
    : QWidget(parent)
{
    setFixedHeight(30);
    auto pLayout = MQ(QHBoxLayout)(this);
    m_pLabel = MQ(QLabel)(this);
    if (!utils::CheckPointer({ pLayout , m_pLabel }))
    {
        LogError("Error param %d %d ", pLayout, m_pLabel);
    }
    m_pLabel->setObjectName("whiteLabel");
    //m_pLabel->setText("Bottom data");
    pLayout->addWidget(m_pLabel);
    pLayout->setContentsMargins(10, 0, 0, 0);
    setLayout(pLayout);

    connect(this, &HintBar::RecvHintData, this, &HintBar::SetHint, Qt::QueuedConnection);

    m_pCBFunc = std::make_shared<ls::IHintCallBack::CallBackFunc>();
    if (m_pCBFunc)
    {
        m_pCBFunc->funcOnUserHint = std::bind(&HintBar::OnUserHint, this, std::placeholders::_1, std::placeholders::_2);
        auto pHintCallBack = g_pCallBack ? g_pCallBack->GetHintCallBack() : nullptr;
        if (pHintCallBack)
        {
            pHintCallBack->Register(m_pCBFunc);
        }
    }
}

HintBar::~HintBar()
{
    if (m_pCBFunc)
    {
        auto pHintCallBack = g_pCallBack ? g_pCallBack->GetHintCallBack() : nullptr;
        if (pHintCallBack)
        {
            pHintCallBack->UnRegister(m_pCBFunc);
        }
    }
}

void HintBar::SetHint(const QString& strText, ls::HintLevel level)
{
    if (m_pLabel)
    {
        m_pLabel->setText(strText);
    }
}

void HintBar::OnUserHint(const std::string& strHintInfo, ls::HintLevel level)
{
    emit RecvHintData(QString::fromStdString(strHintInfo), level);
}
