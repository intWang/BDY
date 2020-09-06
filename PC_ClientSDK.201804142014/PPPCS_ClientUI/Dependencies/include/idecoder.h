#pragma once
#include "DeocodeDataDefine.h"
#include <memory>
class IDecoder
{
public:
    using Ptr = std::shared_ptr<IDecoder>;
    virtual long GetHandle() = 0;
    virtual bool Init(pfnDecodeCallBack pfnCallBack) = 0;
    virtual bool Destory() = 0;
    virtual void InputData(unsigned char*data, int len) = 0;
    virtual void SetUserCode(const char* pStrCode) = 0;
    virtual const char* GetUserCode() = 0;

    virtual bool StartRecord(const char* pSavePath, const char* pUserName, pfnDecodeStatuCallBack pStatusCB) = 0;
    virtual bool StopRecord(const char* pEndTimeStr) = 0;
};