#pragma once
#include <string>
#include <memory>
#include <functional>
namespace ls
{
    
    class IHttpSupport
    {
    public:
        using Ptr = std::shared_ptr<IHttpSupport>;
        using CallBack = std::function<void(const std::string& )>;
        virtual std::string Get(std::string& strUrl, CallBack& funcCB) = 0;


    };

}
