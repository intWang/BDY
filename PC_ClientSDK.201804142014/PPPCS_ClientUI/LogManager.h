#pragma once
#include <QDebug>
#include "ILogger.h"

class LogManager :public ILogger
{
public:
    using Ptr = LogManager*;
    static Ptr GetInstance();

    virtual bool Init() override;
    virtual bool Unit() override;
    void OnMessage(qint64 processID ,QtMsgType type, const QMessageLogContext &context, const QString &msg);
protected:
    void InstallHandler();
    void CheckHistoryFiles();

private:
    LogManager();
    LogManager(const LogManager& other) = delete;
    ~LogManager();

private:
    QString m_logFilePath;
};


#define LogError(format, ...) qCritical(QString("%s %1").arg(format).toStdString().c_str(), __FUNCTION__,  __VA_ARGS__);
#define LogInfo(format, ...) qInfo(QString("%s %1").arg(format).toStdString().c_str(), __FUNCTION__,  __VA_ARGS__);
#define LogDebug(format, ...) qDebug(QString("%s %1").arg(format).toStdString().c_str(), __FUNCTION__,  __VA_ARGS__);
#define LogWarning(format, ...) qWarning(QString("%s %1").arg(format).toStdString().c_str(), __FUNCTION__,  __VA_ARGS__);
