#include "meta.h"
#include "networkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <qeventloop.h>

int Meta::getItag() const
{
    return itag;
}

void Meta::setItag(const int &value)
{
    itag = value;
}

QString Meta::getUrl() const
{
    return url;
}

void Meta::setUrl(const QString &value)
{
    url = value;
}

QString Meta::getExtension() const
{
    return extension;
}

void Meta::setExtension(const QString &value)
{
    extension = value;
}

QString Meta::getResolution() const
{
    return resolution;
}

void Meta::setResolution(const QString &value)
{
    resolution = value;
}

QString Meta::getSize() const
{
    return size;
}

void Meta::setSize(const QString &value)
{
    size = value;
}

Meta::Meta()
{

}

void Meta::findVideoSize()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.head(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        switch (statusCode) {
            case 200:
                this->size = reply->header(QNetworkRequest::ContentLengthHeader).toString();
                break;
            case 301:
            case 302:
                this->url = reply->header(QNetworkRequest::LocationHeader).toString();
                this->findVideoSize();
                break;
            default:
                break;
            // TODO : Handler other cases
        }
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
    }
    delete reply;

    // Convert to human readable
    double x = this->getSize().toDouble();
    QString unit = "bytes";
    if (x/1024 >= 1) {
        x = x/1024;
        unit = "KB";
    }
    if (x/1024 >= 1) {
        x = x/1024;
        unit = "MB";
    }
    if (x/1024 >= 1) {
        x = x/1024;
        unit = "GB";
    }
    this->setSize(QString("%1").arg(x, 0, 'f', 2) + " " + unit);
}



