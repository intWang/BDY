#include "MainFrame.h"
#include <QHBoxLayout>
#include "MessageBoxWnd.h"
#include <QDesktopWidget>
ls::IServiceEngine::Ptr g_pEngine = ls::CreateServiceEngine();
ls::ICallBackEngin::Ptr g_pCallBack = ls::CreateCallbackEngine();

MainFrame::MainFrame(QWidget *parent)
    :BaseDialog(parent)
{
    setWindowTitle("北斗鹰V1.1");

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

void MainFrame::OnCloseCmd()
{
    if (msg::showQuestion(this, "退出", "退出北斗鹰?") == QMessageBox::Ok)
    {
        BaseDialog::OnCloseCmd();
    }
}
