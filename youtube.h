#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QtCore>
#include <QString>

class Youtube
{
public:
    Youtube();

    void downloadVideo(QString& url);
    void downloadPlaylist(QString& url);

signals:

public slots:
};

#endif // YOUTUBE_H
