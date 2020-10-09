#include "PPPCS_ClientUI.h"
#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "LogManager.h"
#include <process.h>
#include <QFile>
#include "IServer.h"
#include <QDateTime>
#include "MessageBoxWnd.h"
#include <QSettings>
#include "utils.h"
#include "ConfigCenter.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/qss/style_blk.qss");
    if (qss.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    
    if (auto pLogManager = LogManager::GetInstance())
    {
        pLogManager->Init();
    }

    utils::MakePathExist(ConfigCenter::GetInstance().GetDumpSavepath());
    utils::RegisterWER(ConfigCenter::GetInstance().GetDumpSavepath(), 2, 5);

    //     QString str = "2020-09-30 12:00:01";
//     QDateTime expired_time = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss");
//     time_t expired = expired_time.toTime_t();
//     time_t now = time(NULL);
//     if (now > expired)
//     {
//         msg::showError(nullptr, "版本已过期", "请联系作者更新版本");
//         return 0;
//     }

    int processid = getpid();
    LogInfo("Start Process %d", processid);

    if (g_pEngine && g_pCallBack)
    {
        g_pEngine->initialize();
        g_pCallBack->initialize();
        LogInfo("g_pEngine  g_pCallBack initialized");
    }
    else
    {
        LogError("engine error %d %d ", g_pEngine, g_pCallBack);
    }

    
    MainFrame w;
    w.show();
    return a.exec();
}
