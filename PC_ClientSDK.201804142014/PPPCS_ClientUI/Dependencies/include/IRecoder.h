#pragma once
#include <string>
#include <functional>
#include <memory>
class IRecoder
{
public:
    using Ptr = std::shared_ptr<IRecoder>;
    enum RecordStatu
    {
        Start,      //param 0
        End,        //param 0
    };
    virtual bool StartRecord(std::string& strUID, std::string& strSavePath) = 0;
    virtual bool StopRecord() = 0;
    virtual bool RegisteRecordStatuCB(std::function<void(const std::string&, RecordStatu)> func) = 0;
    virtual void InputData(unsigned char*data, int len) = 0;
};
