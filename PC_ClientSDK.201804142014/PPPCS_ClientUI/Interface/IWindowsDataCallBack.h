#pragma once

#include <string>

class IWindowsDataCallBack
{
public:
    enum LockStatu
    {
        WindowLocked,
        WindowUnLocked,
    };
    enum DeviceStatu
    {
        Connecting,
        ConnectFailed,
        ConnectSucceed,
    };

    virtual void OnLockStatuChanged(LockStatu newStatu) {};
    virtual void OnDeviceStatuChanged(DeviceStatu newStatu) {};
    virtual void OnJsonData(const std::string& strJson) {};
protected:
private:
};