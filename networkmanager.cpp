#include "networkmanager.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

NetworkManager &NetworkManager::getInstance()
{
    static NetworkManager instance;
    return instance;
}

QNetworkReply *NetworkManager::getWebPage(QUrl &url)
{
    // Since webpages are small, using QEventLoop
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(this->qNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(url);
    QNetworkReply *reply = qNetworkAccessManager->get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    // Reply should be deleted by function caller
    return reply;
}

QNetworkReply *NetworkManager::head(QUrl &url)
{
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(this->qNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(url);
    QNetworkReply *reply = qNetworkAccessManager->head(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    // Reply should be deleted by function caller
    return reply;
}

NetworkManager::NetworkManager()
{
    this->qNetworkAccessManager = new QNetworkAccessManager;
}

