#include "youtube.h"
#include "video.h"
#include "networkmanager.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

Youtube::Youtube()
{

}

void Youtube::downloadVideo(QString &url)
{
    QNetworkReply* reply = NetworkManager::getInstance().getWebPage(url);

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        QString replyErrorString = reply->errorString();
        Q_EMIT errorDownloadingVideoPage(replyErrorString);
        delete reply;
        return;
    }

    QByteArray receivedData = reply->readAll();
    qDebug() << reply->rawHeaderPairs();

    QRegularExpression re("ytplayer\\.config = (.*);ytplayer\\.load");
    QRegularExpressionMatch match = re.match(QString::fromUtf8(receivedData));

    if (!match.hasMatch()) {
        Q_EMIT videoDataNotFound();
        delete reply;
        return;
    }

    QString json = match.captured(1);
    json = json.replace("\\u0026", "&");
    json = json.replace("\\u003c", "<");
    json = json.replace("\\u003e", ">");
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    Video video = Video(jsonObj);
    video.printInfo();
    Q_EMIT videoDataDownloaded(video);
    delete reply;
    return;
}

