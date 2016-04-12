#ifndef YOUTUBE_H
#define YOUTUBE_H

#include "video.h"

#include <QObject>
#include <QString>

class Youtube : public QObject
{
    Q_OBJECT
public:
    Youtube();
    virtual ~Youtube() {}

    void downloadVideo(QString& url);
    void downloadPlaylist(QString& url);

signals:
    void videoDataDownloaded(Video video);
    void errorDownloadingVideoPage(QString& error);
    void videoDataNotFound();

public slots:
};

#endif // YOUTUBE_H
