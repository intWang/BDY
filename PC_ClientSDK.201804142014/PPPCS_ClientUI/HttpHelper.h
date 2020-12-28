#pragma once

#include <QObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <QtNetwork/QSsl>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

#include <QTimer>
class QNetworkAccessManager;
class QNetworkReply;

class HttpHelper : public QObject
{
    Q_OBJECT

public:
    HttpHelper(QObject *parent = nullptr);
    ~HttpHelper();

    void RequestLongCode(std::string& strShortCode);
    void RequestActiveDev(std::string& strDevId);
    void RequestChangePwd(const std::string& strDevId, const std::string& strOldPwd, const std::string& strNewPwd);

    bool IsActiveDevReply(QNetworkReply* replay);
    bool IsChangePwdReply(QNetworkReply* replay);
signals:
    void HttpReplyFinished(QNetworkReply* replay);
    void RequestTimeout();

protected slots:
    void OnHttpReplyFinished(QNetworkReply* replay);

protected:
    QNetworkAccessManager *m_pManager = nullptr;
};


class QReplyTimeout : public QObject {

    Q_OBJECT

public:
    QReplyTimeout(QNetworkReply *reply, const int timeout);

signals:
    void timeout();

private slots:
    void onTimeout();
};

