#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static NetworkManager& getInstance();

    NetworkManager(NetworkManager const&) = delete;
    void operator=(NetworkManager const&) = delete;

    QNetworkReply* getWebPage(QString& url);
    QNetworkReply* head(QString& url);

private:
    QNetworkAccessManager* qNetworkAccessManager;
    NetworkManager();


signals:

public slots:
};

#endif // NETWORKMANAGER_H
