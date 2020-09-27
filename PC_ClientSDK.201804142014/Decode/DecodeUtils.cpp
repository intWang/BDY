#include "stdafx.h"
#include "DecodeUtils.h"

BufferData::BufferData(const unsigned char* data, int len)
{
    AllocateBuf(len);
}

void BufferData::AllocateBuf(int len)
{
    pBufData = new unsigned char[len];
    nBuffLen = len;
}

BufferData::~BufferData()
{
    if (pBufData)
    {
        delete[] pBufData;
    }
}

