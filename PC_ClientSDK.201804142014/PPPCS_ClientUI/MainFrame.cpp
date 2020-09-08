#include "MainFrame.h"
#include <QHBoxLayout>
#include <QDesktopWidget>
ls::IServiceEngine::Ptr g_pEngine = ls::CreateServiceEngine();
ls::ICallBackEngin::Ptr g_pCallBack = ls::CreateCallbackEngine();

MainFrame::MainFrame(QWidget *parent)
    :BaseDialog(parent)
{
    setWindowTitle("视频监控系统V1.0Bate 2020-09-30 过期");

    QDesktopWidget* desktopWidget = QApplication::desktop();
    setMaximumSize(desktopWidget->width(), desktopWidget->height());
    setMinimumSize(desktopWidget->width()/4 * 3, desktopWidget->height() / 4 *3);

    m_pMainViewWiget = MQ(MainViewWidget)(this);
    auto pLayout = GetLayout();
    pLayout->addWidget(m_pMainViewWiget);
    m_pMainViewWiget->show();
    pLayout->setContentsMargins(10, 0, 10, 0);
    pLayout->update();
    FullScreen();
}


MainFrame::~MainFrame()
{
    if (g_pCallBack)
    {
        g_pCallBack->destroy();
        g_pCallBack = nullptr;
    }
    if (g_pEngine)
    {
        g_pEngine->destroy();
        g_pEngine = nullptr;
    }
}
