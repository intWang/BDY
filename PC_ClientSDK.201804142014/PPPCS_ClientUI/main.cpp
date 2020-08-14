#include "PPPCS_ClientUI.h"
#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "LogManager.h"
#include <process.h>
#include <QFile>
#include "IServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (auto pLogManager = LogManager::GetInstance())
    {
        pLogManager->Init();
    }

    QFile qss(":/qss/style_blk.qss");
    if (qss.open(QFile::ReadOnly))
    {
        LogDebug("open qss success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        LogWarning("open qss failed");
    }

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
    LogInfo("Quit Process %d", processid);
    return a.exec();
}
