#pragma once
#include "QtDefine.h"
#include <initializer_list>
#include "LogManager.h"
#include <QJsonObject>
#include <QJsonValue>
namespace utils
{
    bool CheckPointer(std::initializer_list<void*> list);
    bool PtinWnd(QWidgetPtr pWidget);
    std::string GetUUID();
    std::string GetTmpPath();
    QJsonValue GetValueFromJsonObj(const QJsonObject& obj, const QString& param);
    void MakePathExist(QString strPath);

    template<typename T>
    void TravelQJsonArray(QJsonArray arrayData, T& func)
    {
        int nSize = arrayData.size();
        for (int i = 0; i < nSize; ++i)
        {
            func(arrayData.at(i));
        }
    }

    template<typename V, typename T>
    void TravelVector(std::vector<V> vc, T& func)
    {
        for (auto item: vc )
        {
            if (func(item))
            {
                return;
            }
        }
    }
}


