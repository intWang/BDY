#include "HTTPSupport.h"



namespace ls
{
    IHttpSupport::Ptr CreateHttpSupport()
    {
        return std::make_shared<HTTPSupport>();
    }


    HTTPSupport::HTTPSupport()
    {
    }


    HTTPSupport::~HTTPSupport()
    {
      
    }

    std::string HTTPSupport::Get(std::string& strUrl, CallBack& funcCB)
    {
        std::string strRetBuf = "";
        return strRetBuf;
    }

}