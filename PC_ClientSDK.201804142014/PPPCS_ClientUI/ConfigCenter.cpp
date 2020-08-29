#include "ConfigCenter.h"

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
