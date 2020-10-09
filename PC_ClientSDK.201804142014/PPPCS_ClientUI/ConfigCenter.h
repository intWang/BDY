#pragma once

#include <QObject>
#include <QDir>

#include "DataStruct.h"
class ConfigCenter : public QObject
{
    Q_OBJECT

public:
    static ConfigCenter& GetInstance();

    void SetSnapShotSavepath(const QString& strPath);
    void SetRecordSavepath(const QString& strPath);

    QString GetSnapShotSavepath();
    QString GetDumpSavepath();
    QString GetDBFilePath();
    QString GetRecordSavepath();
    QString GetRecordSaveFileName(std::string& strName);

    SnapModeParam::Ptr GetSnapModeParam();

signals:
    void InstanceCreated();

protected:
    void LoadConfig();
    void SaveConfig();
protected:
    ConfigCenter(QObject *parent);
    ~ConfigCenter();


protected:

    QString m_strSnapShotSavePath = QDir::tempPath() + TMPDATA_FOLDER + "SnapShot/";
    QString m_strDumpSavePath = QDir::tempPath() + TMPDATA_FOLDER + "Dumps/";
    QString m_strRecordSavePath = QDir::tempPath() + TMPDATA_FOLDER + "Record/";
    QString m_strDBSavePath = QDir::tempPath() + TMPDATA_FOLDER + "UserData/";

    SnapModeParam::Ptr m_pSnapModeParam = std::make_shared<SnapModeParam>();
};
