#include "video.h"

#include <QtCore>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

QMap<int, Meta> Video::getAvailaibleFormats() const
{
    return availaibleFormats;
}

void Video::setAvailaibleFormats(const QMap<int, Meta> &value)
{
    availaibleFormats = value;
}

QString Video::getVideoId() const
{
    return videoId;
}

void Video::setVideoId(const QString &value)
{
    videoId = value;
}

QString Video::getTitle() const
{
    return title;
}

void Video::setTitle(const QString &value)
{
    title = value;
}

QString Video::getAuthor() const
{
    return author;
}

void Video::setAuthor(const QString &value)
{
    author = value;
}

QString Video::getThumbnail() const
{
    return thumbnail;
}

void Video::setThumbnail(const QString &value)
{
    thumbnail = value;
}

QString Video::getUrl() const
{
    return url;
}

void Video::setUrl(const QString &value)
{
    url = value;
}

QString Video::getLength() const
{
    return length;
}

void Video::setLength(const QString &value)
{
    length = value;
}

Video::Video()
{

}

Video::Video(QJsonObject &videoData)
{
    this->videoId = videoData["args"].toObject()["video_id"].toString();
    this->title = videoData["args"].toObject()["title"].toString();
    this->author = videoData["args"].toObject()["author"].toString();
    this->thumbnail = videoData["args"].toObject()["thumbnail_url"].toString();
    this->url = videoData["args"].toObject()["loadedUrl"].toString();
    this->length = videoData["args"].toObject()["length_seconds"].toString();

    QString urlEncodedFmtStreamMap = videoData["args"].toObject()["url_encoded_fmt_stream_map"].toString();
    QStringList availaibleStreams = urlEncodedFmtStreamMap.split(",");
    Q_FOREACH(QString stream, availaibleStreams) {
        QStringList parameters = stream.split("&");
        QMap<QString, QString> map;
        Q_FOREACH(QString parameter, parameters) {
            QStringList keyValue = parameter.split("=");
            map.insert(keyValue.at(0), QUrl::fromPercentEncoding(keyValue.at(1).toUtf8()));
            qDebug() << keyValue.at(0) << " - " << QUrl::fromPercentEncoding(keyValue.at(1).toUtf8());
        }
        Meta meta;
        meta.setItag(map["itag"].toInt());
        meta.setUrl(map["url"]);
        QString extension, resolution;
        switch (meta.getItag()) {
            case 5:
                extension = "flv";
                resolution = "400X240";
                break;
            case 17:
                extension = "3gp";
                resolution = "320X240";
                break;
            case 18:
                extension = "mp4";
                resolution = "640X360";
                break;
            case 22:
                extension = "mp4";
                resolution = "1280X720";
                break;
            case 36:
                extension = "3gp";
                resolution = "320X240";
                break;
            case 43:
                extension = "webm";
                resolution = "640X360";
                break;
            default:
                extension = "mp4";
                resolution = "";
                break;
        }
        meta.setExtension(extension);
        meta.setResolution(resolution);
        meta.findVideoSize();
        this->availaibleFormats[meta.getItag()] = meta;
        qDebug() << "\n\n";
    }

    // TODO : Handle adaptive formats

    // Converting length form seconds to hh:mm:ss
    // TODO : move to utils
    int ss = this->getLength().toInt();
    int mm = ss/60; ss = ss%60;
    int hh = mm/60; mm = mm%60;
    this->setLength(QString("%1").arg(hh, 2, 10, QChar('0')) + ":" + QString("%1").arg(mm, 2, 10, QChar('0')) + ":" + QString("%1").arg(ss, 2, 10, QChar('0')));
}

void Video::printInfo()
{
    qDebug() << "Title : " << this->title;
    qDebug() << "Author : " << this->author;
    qDebug() << "Length : " << this->length;
    qDebug() << "Video Id : " << this->videoId;
    Q_FOREACH(Meta meta, this->availaibleFormats) {
        qDebug() << meta.getItag() << " " << meta.getHumanReadableSize();
    }
}

