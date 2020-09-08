#include "ConfigCenter.h"
#include "utils.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

#define CONFIG_FILE_NAME "config.db"
#define JSON_KEY_OBJ_GOLBAL "Global"
#define JSON_KEY_OBJ_SNAPPARAM "SnapParam"


#define JSON_KEY_VALUE_RECORDPATH "RecordPath"
#define JSON_KEY_VALUE_SNAPPATH "SnapPath"

#define JSON_KEY_VALUE_NSELRECTWIDTH "nSelRectWidth"
#define JSON_KEY_VALUE_NSELRECTHEIGHT "nSelRectHeight"
#define JSON_KEY_VALUE_NSYNCSPPED "nSyncSpped"
#define JSON_KEY_VALUE_NWNDROWS "nWndRows"
#define JSON_KEY_VALUE_NWNDCOLUMNS "nWndColumns"
#define JSON_KEY_VALUE_NAUTOSTOP "nAutoStop"
#define JSON_KEY_VALUE_NMAXAUTOSTOP "nMaxAutoStop"
#define JSON_KEY_VALUE_BLOCKPROP "bLockProp"
#define JSON_KEY_VALUE_BROWFIRST "bRowFirst"
#define JSON_KEY_VALUE_BPOPFULL "bPopFull"

ConfigCenter& ConfigCenter::GetInstance()
{
    static ConfigCenter m_Instance(nullptr);
    return m_Instance;
}

void ConfigCenter::SetSnapShotSavepath(const QString& strPath)
{
    if (strPath[strPath.size()] != '/')
    {
        m_strSnapShotSavePath = strPath + '/';
    }
    else
    {
        m_strSnapShotSavePath = strPath;
    }
}

void ConfigCenter::SetRecordSavepath(const QString& strPath)
{
    if (strPath[strPath.size()] != '/')
    {
        m_strRecordSavePath = strPath + '/';
    }
    else
    {
        m_strRecordSavePath = strPath;
    }
}

void ConfigCenter::LoadConfig()
{
    QString strDBFile = GetDBFilePath() + CONFIG_FILE_NAME;
    QFile dataFile(strDBFile);
    if (!dataFile.open(QIODevice::ReadOnly))
    {
        LogError("Open failed %s", strDBFile.toStdString().c_str());
        return;
    }
    QByteArray byteArray = dataFile.readAll();
    dataFile.flush();
    dataFile.close();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!doc.isNull() && doc.isObject())
    {
        QJsonObject obj = doc.object();
        QJsonObject objGolbal = utils::GetValueFromJsonObj(obj, JSON_KEY_OBJ_GOLBAL).toObject();

        if (!objGolbal.isEmpty())
        {
            m_strRecordSavePath = utils::GetValueFromJsonObj(objGolbal, JSON_KEY_VALUE_RECORDPATH).toString();
            m_strSnapShotSavePath = utils::GetValueFromJsonObj(objGolbal, JSON_KEY_VALUE_SNAPPATH).toString();

            utils::MakePathExist(m_strRecordSavePath);
            utils::MakePathExist(m_strSnapShotSavePath);
        }
        QJsonObject objSnapParam = utils::GetValueFromJsonObj(obj, JSON_KEY_OBJ_SNAPPARAM).toObject();
        if (!objSnapParam.isEmpty() && m_pSnapModeParam)
        {
            m_pSnapModeParam->nSelRectWidth.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NSELRECTWIDTH).toInt());
            m_pSnapModeParam->nSelRectHeight.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NSELRECTHEIGHT).toInt());
            m_pSnapModeParam->nSyncSpped.store((SyncSpeed)utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NSYNCSPPED).toInt());
            m_pSnapModeParam->nWndRows.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NWNDROWS).toInt());
            m_pSnapModeParam->nWndColumns.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NWNDCOLUMNS).toInt());
            m_pSnapModeParam->nAutoStop.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NAUTOSTOP).toInt());
            m_pSnapModeParam->nMaxAutoStop.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_NMAXAUTOSTOP).toInt());
            m_pSnapModeParam->bLockProp.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_BLOCKPROP).toBool());
            m_pSnapModeParam->bRowFirst.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_BROWFIRST).toBool());
            m_pSnapModeParam->bPopFull.store(utils::GetValueFromJsonObj(objSnapParam, JSON_KEY_VALUE_BPOPFULL).toBool());
        }
    }
}

void ConfigCenter::SaveConfig()
{
    QJsonDocument doc;
    QJsonObject root;
    QJsonObject objGolbal;
    QJsonObject objSnapParam;

    objGolbal.insert(JSON_KEY_VALUE_RECORDPATH, m_strRecordSavePath);
    objGolbal.insert(JSON_KEY_VALUE_SNAPPATH, m_strSnapShotSavePath);

    if (m_pSnapModeParam)
    {
        objSnapParam.insert(JSON_KEY_VALUE_NSELRECTWIDTH, m_pSnapModeParam->nSelRectWidth.load());
        objSnapParam.insert(JSON_KEY_VALUE_NSELRECTHEIGHT, m_pSnapModeParam->nSelRectHeight.load());
        objSnapParam.insert(JSON_KEY_VALUE_NSYNCSPPED, m_pSnapModeParam->nSyncSpped.load());
        objSnapParam.insert(JSON_KEY_VALUE_NWNDROWS, m_pSnapModeParam->nWndRows.load());
        objSnapParam.insert(JSON_KEY_VALUE_NWNDCOLUMNS, m_pSnapModeParam->nWndColumns.load());
        objSnapParam.insert(JSON_KEY_VALUE_NAUTOSTOP, m_pSnapModeParam->nAutoStop.load());
        objSnapParam.insert(JSON_KEY_VALUE_NMAXAUTOSTOP, m_pSnapModeParam->nMaxAutoStop.load());
        objSnapParam.insert(JSON_KEY_VALUE_BLOCKPROP, m_pSnapModeParam->bLockProp.load());
        objSnapParam.insert(JSON_KEY_VALUE_BROWFIRST, m_pSnapModeParam->bRowFirst.load());
        objSnapParam.insert(JSON_KEY_VALUE_BPOPFULL, m_pSnapModeParam->bPopFull.load());
    }

    root.insert(JSON_KEY_OBJ_GOLBAL, objGolbal);
    root.insert(JSON_KEY_OBJ_SNAPPARAM, objSnapParam);
    doc.setObject(root);
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);

    QString strDBFile = GetDBFilePath() + CONFIG_FILE_NAME;
    QFile dataFile(strDBFile);
    if (!dataFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
    {
        LogError("Open failed %s", strDBFile.toStdString().c_str());
        return;
    }
    QTextStream out(&dataFile);
    out.setCodec("UTF-8");
    out << byteArray;
    dataFile.flush();
    dataFile.close();

    QString strJson(byteArray);
    LogDebug("Save Data %s", strJson.toStdString().c_str());
}

ConfigCenter::ConfigCenter(QObject *parent)
    : QObject(parent)
{
    LoadConfig();
}

ConfigCenter::~ConfigCenter()
{
    SaveConfig();
}

QString ConfigCenter::GetSnapShotSavepath()
{
    return m_strSnapShotSavePath;
}

QString ConfigCenter::GetDBFilePath()
{
    utils::MakePathExist(m_strDBSavePath);
    return m_strDBSavePath;
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

SnapModeParam::Ptr ConfigCenter::GetSnapModeParam()
{
    return m_pSnapModeParam;
}
