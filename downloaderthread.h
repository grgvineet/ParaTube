#ifndef DOWNLOADERTHREAD_H
#define DOWNLOADERTHREAD_H

#include <QThread>
#include <QString>
#include <QFile>
#include <QNetworkAccessManager>

class DownloaderThread : public QObject
{
    Q_OBJECT

    int prg;

public:
    QString url, filename;
    int begin, end;

    QFile *file;
    QNetworkAccessManager* mgr;

    DownloaderThread(const QString url, const QString file, int contentStart, int contentEnd, QNetworkAccessManager* qnam, QObject* parent = 0);
    void run();

signals:
    void progress(qint64);
    void downloadCompleted();

public slots:
    void dataReceived();
    void finished();
    void downloadProgress(qint64, qint64);
};

#endif // DOWNLOADERTHREAD_H
