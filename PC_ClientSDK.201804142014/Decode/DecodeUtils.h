#pragma once
#include <memory>
struct BufferData
{
    using Ptr = std::shared_ptr<BufferData>;
    BufferData() = default;
    BufferData(const unsigned char* data, int len);
    void AllocateBuf(int len);
    virtual ~BufferData();
    unsigned char* pBufData = nullptr;
    int nBuffLen;
};