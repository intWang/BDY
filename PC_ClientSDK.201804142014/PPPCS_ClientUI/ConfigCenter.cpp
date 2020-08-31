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

QString ConfigCenter::GetRecordSaveFileName(std::string& strUID)
{
    auto strPath = m_strRecordSavePath;
    auto strName = QString::fromStdString(strUID);
    utils::MakePathExist(strPath);
    QString strFileName = strPath + strName + QDateTime::currentDateTime().toString("_yyMMdd_hh_mm_ss") + ".mp4";
    return strFileName;
}
