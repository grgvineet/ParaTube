#ifndef YOUTUBE_H
#define YOUTUBE_H

#include "video.h"

#include <QObject>
#include <QString>
#include <QVector>

class Youtube : public QObject
{
    Q_OBJECT
public:
    Youtube();
    virtual ~Youtube() {}

    void downloadVideo(QUrl& url);
    void downloadPlaylist(QUrl& url);

signals:
    void videoDataDownloaded(Video video);
    void playlistDataDownloaded(QVector<Video> videos);
    void errorDownloadingVideoPage(QString& error);
    void videoDataNotFound();

public slots:
};

#endif // YOUTUBE_H
