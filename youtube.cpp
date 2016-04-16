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
#include <QUrlQuery>

Youtube::Youtube()
{

}

void Youtube::downloadVideo(QUrl &url)
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

void Youtube::downloadPlaylist(QUrl &url)
{
    // TODO : validate url
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

    QUrlQuery query(url.query());
    QString listId = query.queryItemValue("list");
    qDebug() << "List : " << listId;
    // TODO : if listId empty, invalid url

    QRegularExpression re;
    if(url.fileName() == "watch") {
        re.setPattern("<li class=\"yt-uix-scroller-scroll-unit.*>");
    } else {
        re.setPattern("<tr class=\"pl-video yt-uix-tile.*>");
    }
    QRegularExpressionMatchIterator iterator = re.globalMatch(QString::fromUtf8(receivedData).replace("&amp;", "&"));
//    qDebug() << iterator.regularExpression().pattern();
    QStringList videoIdList;
    while(iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
//        qDebug() << match.captured(0);
        QRegularExpression videoIdRE("data-video-id=\"(.*?)\"");
        QRegularExpressionMatch videoIdMatch = videoIdRE.match(match.captured(0));
        videoIdList.append(videoIdMatch.captured(1));
        qDebug() << "Video id : " << videoIdMatch.captured(1);
    }

    qDebug() << "Total videos : " << videoIdList.size();

    QVector<Video> videos;

    Q_FOREACH(QString id, videoIdList ) {
        qDebug() << "Fetching info for video " << id ;

        QUrl videoUrl("https://www.youtube.com/watch?v="+id);
        reply = NetworkManager::getInstance().getWebPage(videoUrl);

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            QString replyErrorString = reply->errorString();
            Q_EMIT errorDownloadingVideoPage(replyErrorString);
            delete reply;
            return;
        }

        receivedData = reply->readAll();
//        qDebug() << reply->rawHeaderPairs();

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
        videos.append(video);
        qDebug() << "Video with video id :" << id << " fetched";
    }
    Q_EMIT playlistDataDownloaded(videos);

    delete reply;
    return;
}

