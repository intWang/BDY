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

    if (!CheckPointer({ m_pIconLabel, m_pTitleLabel, m_pMinimizeButton, m_pMaximizeButton, m_pCloseButton, pLayout }))
    {
        qCritical("Error param %d %d %d %d %d %d", m_pIconLabel, m_pTitleLabel, m_pMinimizeButton, m_pMaximizeButton, m_pCloseButton, pLayout);
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

    m_pMinimizeButton->setToolTip("Minimize");
    m_pMaximizeButton->setToolTip("Maximize");
    m_pCloseButton->setToolTip("Close");

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
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void CmdBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
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

// void CmdBar::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     Q_UNUSED(event);
// 
//     emit m_pMaximizeButton->clicked();
// }
