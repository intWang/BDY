#pragma once

enum DecodeStatus
{
    Start_Record,
    Stop_Record,
    Start_Transfer,
    Complete_Transfer,  //pParam -> const char * savePath
};

typedef void(*pfnDecodeCallBack)(const char* pUserCode, const unsigned char* pBuf, int width, int height, int len);
typedef void(*pfnDecodeStatuCallBack)(const char* pUserCode, DecodeStatus emStatus, void* pParam);
