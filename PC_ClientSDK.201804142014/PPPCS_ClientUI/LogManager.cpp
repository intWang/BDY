#include "LogManager.h"
#include <QMutex>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <process.h>

#define MAX_LOG_FILE_SIZE 2
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    static int curProcessid = getpid();
    mutex.lock();
    if (auto pLogManager = LogManager::GetInstance())
    {
        pLogManager->OnMessage(curProcessid, type, context, msg);
    }
    mutex.unlock();
}

LogManager::Ptr LogManager::GetInstance()
{
    static LogManager m_Instance;
    return &m_Instance;
}

void LogManager::InstallHandler()
{
    qInstallMessageHandler(outputMessage);
}

void LogManager::CheckHistoryFiles()
{
    QString logFilePath = m_logFilePath;
    QDir dir(logFilePath);
    if (!dir.exists())
    {
        return;
    }
    QStringList filter;
    filter<<"*.log";
    dir.setNameFilters(filter);
    QList<QFileInfo> fileList(QList<QFileInfo>(dir.entryInfoList(filter)));
    qSort(fileList.begin(), fileList.end(), [](QFileInfo& left, QFileInfo& right) {
        return left.birthTime() < right.birthTime();
    });
    while (fileList.size() > MAX_LOG_FILE_SIZE)
    {
        QFile file(fileList.begin()->absoluteFilePath());
        file.remove();
        fileList.erase(fileList.begin());
    }
}

bool LogManager::Init()
{
    m_logFilePath = QDir::tempPath() + "/PPCS_Client/";
    CheckHistoryFiles();
    InstallHandler();
    return true;
}

bool LogManager::Unit()
{
    return true;
}

void LogManager::OnMessage(qint64 processID, QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    QString message = QString("%1 %2 %3 %4").arg(current_date_time).arg(text).arg(context_info).arg(msg);
    QString log_file_path = m_logFilePath;
    QDir dir(log_file_path);
    if (!dir.exists())
    {
        dir.mkdir(log_file_path);
    }
    QString log_file = log_file_path + current_date + "-" + QVariant(processID).toString() + ".log";
    
    QFile file(log_file);

    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}

LogManager::LogManager()
{
}


LogManager::~LogManager()
{
    Unit();
}
