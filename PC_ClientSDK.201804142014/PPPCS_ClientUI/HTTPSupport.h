#pragma once
#include "IHttpSupport.h"
#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

namespace ls
{

    class HTTPSupport :public IHttpSupport
    {
    public:
        using Ptr = std::shared_ptr<HTTPSupport>;
        HTTPSupport();
        ~HTTPSupport();

        virtual std::string Get(std::string& strUrl, CallBack& funcCB) override;

    protected:
       
    };

}

