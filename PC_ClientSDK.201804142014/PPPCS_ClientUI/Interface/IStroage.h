#pragma once
#include <memory>
#include <string>

class IStroage
{
public:
    using Ptr = std::shared_ptr<IStroage> ;
    virtual void Write() = 0;
    virtual void Read() = 0;
private:
};
