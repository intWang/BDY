#pragma once

#include <QObject>
#include <QDir>
class ConfigCenter : public QObject
{
    Q_OBJECT

public:
    static ConfigCenter& GetInstance();

    QString GetSnapShotSavepath();
    QString GetRecordSavepath();

protected:
    ConfigCenter(QObject *parent);
    ~ConfigCenter();


protected:

    QString m_strSnapShotSavePath = QDir::tempPath() + "/PPCS_Client/SnapShot/";
    QString m_strRecordSavePath = QDir::tempPath() + "/PPCS_Client/Record/";
};
