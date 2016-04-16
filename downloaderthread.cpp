#include "downloaderthread.h"

#include <QEventLoop>
#include <QNetworkReply>


DownloaderThread::DownloaderThread(const QString u, const QString f, int s, int e,QNetworkAccessManager *qnam, QObject *parent) :
    url(u),
    filename(f),
    begin(s),
    end(e),
    mgr(qnam),
    QObject(parent)
{

    prg = 0;
}

void DownloaderThread::run()
{
    qDebug() << "Thread started";

    file = new QFile(filename);

    file->open(QIODevice::WriteOnly);
    file->seek(begin);

    QNetworkRequest request(url);
    request.setRawHeader("Range", QString("bytes").append("=").append(QString::number(begin)).append("-").append(QString::number(end)).toLatin1());
    qDebug() << request.rawHeaderList();
    QNetworkReply* reply = mgr->get(request);

    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    connect(reply, SIGNAL(readyRead()), this, SLOT(dataReceived()) );
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64, qint64)) );

}

void DownloaderThread::dataReceived()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    qDebug() << "Data read :" << reply->bytesAvailable();
    file->write(reply->readAll());
}

void DownloaderThread::finished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    file->close();
    Q_EMIT downloadCompleted();
}

void DownloaderThread::downloadProgress(qint64 p, qint64 total)
{
    int temp = p - prg;
    prg = p;
    Q_EMIT progress(temp);
}
