#include "videoinfo.h"
#include "ui_videoinfo.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

VideoInfo::VideoInfo(Video video,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoInfo)
{
    ui->setupUi(this);

    this->video = video;
    mgr = new QNetworkAccessManager;

    ui->lTitle->setText(QString("Title : ") + video.getTitle());
    ui->lAuthor->setText(QString("Author : ") + video.getAuthor());
    ui->lLength->setText(QString("Length : ") + video.getLength());
    ui->progressBar->hide();

    QTableWidget *table = ui->tableWidget;
    QMap<int, Meta> map = video.getAvailaibleFormats();
    table->setRowCount(map.size());
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QString("ITAG;EXTENSION;RESOLUTION;SIZE").split(";"));
    int i=0;
    Q_FOREACH (Meta meta, map.values()) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(meta.getItag())));
        table->setItem(i, 1, new QTableWidgetItem(meta.getExtension()));
        table->setItem(i, 2, new QTableWidgetItem(meta.getResolution()));
        table->setItem(i, 3, new QTableWidgetItem(meta.getSize()));
        i++;
    }
    table->selectRow(0);

    connect(ui->bDownload, SIGNAL(clicked(bool)), this, SLOT(onDownloadClicked(bool)));

}

VideoInfo::~VideoInfo()
{
    delete ui;
}

void VideoInfo::onDownloadClicked(bool checked)
{
    ui->progressBar->show();
    ui->bDismiss->hide();
    ui->bDownload->hide();

    QTableWidget* tableWidget = ui->tableWidget;
    int selectedRow = tableWidget->currentRow();
    QTableWidgetItem *itag = tableWidget->item(selectedRow,0);
    QString itagText = itag->text();

    // the HTTP request
    Meta meta = video.getAvailaibleFormats()[itagText.toInt()];

    while (true) {
        QEventLoop eventLoop;
        QNetworkRequest req(meta.getUrl());
        QObject::connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QNetworkReply *reply = mgr->head(req);
        eventLoop.exec();

        QObject::disconnect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            delete reply;
            return;
        }

        switch(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
            case 302:
                meta.setUrl(reply->header(QNetworkRequest::LocationHeader).toString());
                delete reply;
                continue;
                break;
        }

        delete reply;
        break;
    }

    QNetworkReply* reply = mgr->get(QNetworkRequest(meta.getUrl()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(dataReceived()) );
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64, qint64)) );

    file = new QFile(video.getTitle() + "." + tableWidget->item(selectedRow, 1)->text(), this);
    file->open(QIODevice::WriteOnly);
    qDebug() << file->fileName();

}

void VideoInfo::dataReceived()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    file->write(reply->readAll());
}

void VideoInfo::finished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    delete reply;
    file->close();
    delete file;
}

void VideoInfo::downloadProgress(qint64 bytesReceived, qint64 totalBytes)
{
//    qDebug() << bytesReceived << " of " << totalBytes;
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesReceived);
}
