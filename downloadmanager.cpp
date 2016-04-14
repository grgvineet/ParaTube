#include "downloaderthread.h"
#include "downloadmanager.h"

DownloadManager::DownloadManager(Meta meta, QString path, QObject *parent)
{
    this->meta = meta;
    this->path = path;

    this->size = meta.getSize();
    this->progressTillNow = 0;

    this->qnam = new QNetworkAccessManager(parent);

    this->file = new QFile(path);
    file->open(QIODevice::WriteOnly);
    file->resize(size);
    file->close();
}

void DownloadManager::download()
{
    for (int i=0; i<PARTS ; i++) {
        int part = meta.getSize()/PARTS;
        int start = i*part;
        int end = i!=PARTS-1 ? start+part : meta.getSize();
        end--;

        DownloaderThread *dt = new DownloaderThread(meta.getUrl(), path, start, end, qnam, this);
        connect(dt, SIGNAL(progress(qint64)), this, SLOT(threadProgress(qint64)));
        connect(dt, SIGNAL(downloadCompleted()), this, SLOT(threadFinished()) );
        dt->run();
        qDebug() << i << " " << part << " " << start << " " << end ;
    }
}

void DownloadManager::threadProgress(qint64 p)
{
    // Multiple threads will be emitting this
    mutex.lock();
    progressTillNow += p;
    mutex.unlock();

    Q_EMIT progress(progressTillNow, size);

}

void DownloadManager::threadFinished()
{
    mutex.lock();
    threadsCompleted++;
    mutex.unlock();

    DownloaderThread* t = qobject_cast<DownloaderThread*>(sender());
    delete t;

    if (threadsCompleted == PARTS) {
        Q_EMIT downloadCompleted();
    }
}

