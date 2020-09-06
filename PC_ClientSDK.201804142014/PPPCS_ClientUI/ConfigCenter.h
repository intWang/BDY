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
    QString GetRecordSavepath();
    QString GetRecordSaveFileName(std::string& strName);

    SnapModeParam::Ptr GetSnapModeParam();

protected:
    void LoadConfig();
    void SaveConfig();
protected:
    ConfigCenter(QObject *parent);
    ~ConfigCenter();


protected:

    QString m_strSnapShotSavePath = QDir::tempPath() + "/PPCS_Client/SnapShot/";
    QString m_strRecordSavePath = QDir::tempPath() + "/PPCS_Client/Record/";

    SnapModeParam::Ptr m_pSnapModeParam = std::make_shared<SnapModeParam>();
};
