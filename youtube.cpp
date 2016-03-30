#include "youtube.h"
#include "video.h"

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
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("https://r2---sn-qxa7en7z.googlevideo.com/videoplayback?pl=16&sparams=dur,expire,id,initcwndbps,ip,ipbits,itag,lmt,mime,mm,mn,ms,mv,nh,pl,requiressl,source,upn&ipbits=0&requiressl=yes&sver=3&itag=5&upn=JfRgy1m-KJ4&signature=0F46A3AAFBFE57B9AD6F03C779A47F5C6F8FDCD7.4627742AEA71A3DBAB18C2B0A63C7BF3CEB56E2D&id=o-AEY8dIyJ6PrI-qbXCe3lYLF6J-mwb0zWfpHRAsYazwK8&dur=3657.587&mime=video%2Fx-flv&lmt=1445338930072300&ip=120.56.106.127&key=cms1&fexp=9414671%2C9416126%2C9417827%2C9418642%2C9420452%2C9422596%2C9423661%2C9423662%2C9425791%2C9426197%2C9427126%2C9427600%2C9427902%2C9429147%2C9429564%2C9430818%2C9430829%2C9432198%2C9432390&expire=1459296394&source=youtube&redirect_counter=1&req_id=7ef766d0fd79a3ee&cms_redirect=yes&mm=30&mn=sn-qxa7en7z&ms=nxu&mt=1459274846&mv=m&nh=IgphcjAzLmRlbDAxKgkxMjcuMC4wLjE") ) );
    QNetworkReply *reply = mgr.head(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray receivedData = reply->readAll();
        qDebug() << reply->rawHeaderPairs(); return;
        delete reply;

        QRegularExpression re("ytplayer\\.config = (.*);ytplayer\\.load");
        QRegularExpressionMatch match = re.match(QString::fromUtf8(receivedData));
        QString json;
        if (match.hasMatch()) {
            json = match.captured(1);
            json = json.replace("\\u0026", "&");
            json = json.replace("\\u003c", "<");
            json = json.replace("\\u003e", ">");
            QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
            QJsonObject jsonObj = jsonDoc.object();
            Video video = Video(jsonObj);
            video.printInfo();
        } else {
            qDebug() << "No match occured";
        }
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
}

