#include "ConfigCenter.h"
#include "utils.h"
#include <QDateTime>
ConfigCenter& ConfigCenter::GetInstance()
{
    static ConfigCenter m_Instance(nullptr);
    return m_Instance;
}

ConfigCenter::ConfigCenter(QObject *parent)
    : QObject(parent)
{
}

ConfigCenter::~ConfigCenter()
{
}

QString ConfigCenter::GetSnapShotSavepath()
{
    return m_strSnapShotSavePath;
}

QString ConfigCenter::GetRecordSavepath()
{
    return m_strRecordSavePath;
}

QString ConfigCenter::GetRecordSaveFileName(std::string& _strName)
{
    auto strPath = m_strRecordSavePath;
    auto strName = QString::fromStdString(_strName);
    utils::MakePathExist(strPath);
    QString strFileName = strPath + strName + QDateTime::currentDateTime().toString("-yyyyMMdd-hhmmss") + ".mp4";
    return strFileName;
}
