#include "HttpHelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include "LogManager.h"
HttpHelper::HttpHelper(QObject *parent)
    : QObject(parent)
{
    m_pManager = new QNetworkAccessManager(this);
    if (m_pManager)
    {
        connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnHttpReplyFinished(QNetworkReply*)));
    }
}

HttpHelper::~HttpHelper()
{
}

void HttpHelper::RequestLongCode(std::string& strShortCode)
{
    if (m_pManager )
    {
        QString strUrl = "https://lyzb.hubanpay.com/huban-camera/api/device/infoByUid/";
        strUrl += QString::fromStdString(strShortCode);
        QNetworkRequest request;
        QSslConfiguration config;

        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(config);

        request.setHeader(QNetworkRequest::UserAgentHeader
            , "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36");

        request.setUrl(QUrl(strUrl));
        auto pReply = m_pManager->get(request);
        QReplyTimeout *pTimeout = new QReplyTimeout(pReply, 3000);
        connect(pTimeout, &QReplyTimeout::timeout, [&]() {
            emit RequestTimeout();
        });
    }
}

void HttpHelper::RequestActiveDev(std::string& strDevId)
{
    if (m_pManager)
    {
        QString strUrl = "https://lyzb.hubanpay.com/huban-camera/api/device/activity?id=%1&type=1";
        strUrl = strUrl.arg(QString::fromStdString(strDevId));
        QNetworkRequest request;
        QSslConfiguration config;

        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(config);

        request.setHeader(QNetworkRequest::UserAgentHeader
            , "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36");

        request.setUrl(QUrl(strUrl));
        auto pReply = m_pManager->get(request);
        QReplyTimeout *pTimeout = new QReplyTimeout(pReply, 3000);
        connect(pTimeout, &QReplyTimeout::timeout, [&]() {
            emit RequestTimeout();
        });
    }
}

void HttpHelper::RequestChangePwd(const std::string& strDevId, const std::string& strOldPwd, const std::string& strNewPwd)
{
    if (m_pManager)
    {
        QString strUrl = "https://lyzb.hubanpay.com/huban-camera/api/device/updatePassWord";
        QNetworkRequest request;
//         QSslConfiguration config;
// 
//         config.setPeerVerifyMode(QSslSocket::VerifyNone);
//         config.setProtocol(QSsl::TlsV1SslV3);
//         request.setSslConfiguration(config);

//         request.setHeader(QNetworkRequest::UserAgentHeader
//             , "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36");

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonDocument doc;
        QJsonObject root;

        root.insert("oldpassword", QString::fromStdString(strOldPwd));
        root.insert("password", QString::fromStdString(strNewPwd));
        root.insert("uid", QString::fromStdString(strDevId));

        doc.setObject(root);
        QByteArray data = doc.toJson(QJsonDocument::Compact);
        //request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());
        
        request.setUrl(QUrl(strUrl));
        auto pReply = m_pManager->post(request, data);
        QEventLoop loop;
        connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        auto ret = pReply->error();
        if (ret == QNetworkReply::NoError) {
        }
        else {
        }
        QByteArray bytes = pReply->readAll();
        LogInfo("updatePassWord request end %s", bytes.toStdString().c_str());
// 
//         connect(pReply, &QNetworkReply::finished, this, [this]() {
//             if (auto pSender = qobject_cast<QNetworkReply*>(sender()))
//             {
//                 QByteArray bytes = pSender->readAll();
//                 if (pSender->error() == QNetworkReply::NoError)
//                 {
//                 }
//                 else
//                 {
//                 }
//             }
//         });
    }
}

bool HttpHelper::IsActiveDevReply(QNetworkReply* replay)
{
    if (replay && replay->url().toString().indexOf("activity") != -1)
    {
        return true;
    }
    return false;
}

bool HttpHelper::IsChangePwdReply(QNetworkReply* replay)
{
    if (replay && replay->url().toString().indexOf("updatePassWord") != -1)
    {
        return true;
    }
    return false;
}

void HttpHelper::OnHttpReplyFinished(QNetworkReply* replay)
{
    emit  HttpReplyFinished(replay);
    replay->deleteLater();
}

QReplyTimeout::QReplyTimeout(QNetworkReply *reply, const int timeout)
    : QObject(reply) 
{
    Q_ASSERT(reply);
    if (reply && reply->isRunning())
    {
        QTimer::singleShot(timeout, this, SLOT(onTimeout()));
    }
}

void QReplyTimeout::onTimeout()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(parent());
    if (reply->isRunning())
    {
        reply->abort();
        reply->deleteLater();
        emit timeout();
    }
}