#include "video.h"

#include <QtCore>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

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
        meta.setItag(map["itag"]);
        meta.setUrl(map["url"]);
        QString extension, resolution;
        switch (map["itag"].toInt()) {
            case 5:
                extension = "flv";
                resolution = "";
                break;
            default:
                extension = "mp4";
                resolution = "";
                break;
        }
        meta.setExtension(extension);
        meta.setResolution(resolution);
        this->availaibleFormats.append(meta);
        qDebug() << "\n\n";
    }

    // TODO : Handle adaptive formats
}

void Video::printInfo()
{
    qDebug() << "Title : " << this->title;
    qDebug() << "Author : " << this->author;
    qDebug() << "Length : " << this->length;
    qDebug() << "Video Id : " << this->videoId;
    Q_FOREACH(Meta meta, this->availaibleFormats) {
        qDebug() << meta.getItag();
    }
}

