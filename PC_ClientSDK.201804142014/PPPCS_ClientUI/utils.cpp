#include "utils.h"
#include <objbase.h>
#include <QDir>
#define GUID_LEN 64
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

    void MakePathExist(QString strPath)
    {
        QDir dir(strPath);
        if (!dir.exists())
        {
            dir.mkdir(strPath);
        }
    }

    void RegisterWER(QString strPath, unsigned long nDumpType, unsigned long nDumpCount)
    {
        using BYTE = unsigned char;
        using DWORD = unsigned long;
        HKEY hKey = NULL;
        if (ERROR_SUCCESS == RegCreateKeyExA(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\北斗鹰PC.exe)", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WOW64_64KEY | KEY_WRITE, NULL, &hKey, NULL))
        {
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpFolder", 0, REG_EXPAND_SZ, (BYTE*)strPath.toStdString().c_str(), strPath.size()))
            {
                LogError("write DumpFolder failed");
            }
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpType", 0, REG_DWORD, (BYTE*)&nDumpType, sizeof(DWORD)))
            {
                LogError("write DumpType failed");
            }
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpCount", 0, REG_DWORD, (BYTE*)&nDumpCount, sizeof(DWORD)))
            {
                LogError("write DumpType failed");
            }
            RegCloseKey(hKey);
            hKey = NULL;
        }

        if (ERROR_SUCCESS == RegCreateKeyExA(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\北斗鹰PC.exe)", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WOW64_32KEY | KEY_WRITE, NULL, &hKey, NULL))
        {
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpFolder", 0, REG_EXPAND_SZ, (BYTE*)strPath.toStdString().c_str(), strPath.size()))
            {
                LogError("write DumpFolder failed");
            }
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpType", 0, REG_DWORD, (BYTE*)&nDumpType, sizeof(DWORD)))
            {
                LogError("write DumpType failed");
            }
            if (ERROR_SUCCESS != RegSetValueExA(hKey, "DumpCount", 0, REG_DWORD, (BYTE*)&nDumpCount, sizeof(DWORD)))
            {
                LogError("write DumpType failed");
            }
            RegCloseKey(hKey);
            hKey = NULL;
        }
    }

    std::string GetUUID()
    {
        char buf[GUID_LEN] = { 0 };
        GUID guid;

        if (CoCreateGuid(&guid))
        {
            return std::move(std::string(""));
        }

        sprintf_s(buf,
            "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2],
            guid.Data4[3], guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);

        return std::move(std::string(buf));
    }
}
