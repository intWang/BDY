#include "utils.h"
namespace utils
{
    bool CheckPointer(std::initializer_list<void*> list)
    {
        for (auto ptr : list)
        {
            if (!ptr)
            {
                return false;
            }
        }
        return true;
    }
    bool PtinWnd(QWidgetPtr pWidget)
    {
        if (pWidget)
        {
            QRect rcWnd = pWidget->geometry();
            QPoint ptCursor = pWidget->mapFromGlobal(QCursor::pos());
            return rcWnd.contains(ptCursor);
        }
        return false;
    }

    QJsonValue GetValueFromJsonObj(const QJsonObject& obj, const QString& param)
    {
        if (obj.contains(param))
        {
            QJsonValue value = obj.value(param);
            return value;
        }
        return QJsonValue();
    }
}
