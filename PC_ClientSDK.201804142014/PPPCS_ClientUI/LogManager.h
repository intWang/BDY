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


