#include "PPPCS_ClientUI.h"
#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "LogManager.h"
#include <process.h>
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
        qDebug("open qss success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qWarning("open qss failed");
    }

    int processid = getpid();
    qDebug("Start Process %d", processid);
    MainFrame w;
    w.show();
    qDebug("Quit Process %d", processid);
    return a.exec();
}
