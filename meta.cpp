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

int Meta::getSize() const
{
    return size;
}

void Meta::setSize(const int &value)
{
    size = value;
}

QString Meta::getHumanReadableSize() const
{
    return humanReadableSize;
}

void Meta::setHumanReadableSize(const QString &value)
{
    humanReadableSize = value;
}

Meta::Meta()
{

}

void Meta::findVideoSize()
{

    QNetworkReply* reply = NetworkManager::getInstance().head(url);

    if (reply->error() == QNetworkReply::NoError) {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        switch (statusCode) {
            case 200:
                this->size = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
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
    double x = this->getSize();
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
    this->setHumanReadableSize(QString("%1").arg(x, 0, 'f', 2) + " " + unit);
}



