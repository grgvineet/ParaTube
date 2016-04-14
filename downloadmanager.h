#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "meta.h"

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QString>
#include <qnetworkaccessmanager.h>

/**
 * @brief It manages a given download. Split the download into parts and download them parallely. One single download is appeared on calling side
 *
 */

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    DownloadManager(Meta meta, QString path, QObject* parent = 0);

    void download();

private:
    int threadsCompleted = 0;
    const int PARTS = 4;
    qint64 size;
    qint64 progressTillNow;
    Meta meta;
    QString path;
    QFile* file;
    QNetworkAccessManager *qnam;
    QMutex mutex;

signals:
    void downloadCompleted();
    void progress(qint64, qint64);

private slots:
    void threadProgress(qint64);
    void threadFinished();
};

#endif // DOWNLOADMANAGER_H
