#include "videoinfo.h"
#include "ui_videoinfo.h"
#include "downloaderthread.h"
#include "downloadmanager.h"
#include "networkmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QSettings>
#include <QMessageBox>

#include <algorithm>

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
    QList<Meta> availaibleFormats = video.getAvailaibleFormats();
    table->setRowCount(availaibleFormats.size());
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QString("ITAG;EXTENSION;RESOLUTION;SIZE").split(";"));

    Q_FOREACH (Meta meta, availaibleFormats) {
        qDebug() << meta.getSize();
    }
    qDebug() << "*********************************";
    std::sort(availaibleFormats.begin(),availaibleFormats.end(), [](const Meta &a, const Meta &b) {
        qDebug() << a.getSize() << " " << b.getSize();
        int resA = a.getResolution().split('X')[0].toInt() * a.getResolution().split('X')[1].toInt();
        int resB = b.getResolution().split('X')[0].toInt() * b.getResolution().split('X')[1].toInt();
        if (resA > resB) {
            return true;
        } else if (resA == resB) {
            if (a.getExtension() == b.getExtension()) {
                return a.getSize() > b.getSize();
            } else {
                if (a.getExtension() == "mp4") {
                    // Prefer mp4 over other, mp4-ist :P
                    return true;
                }
                return false;
            }
        } else {
            return false;
        }
    });

    int i=0;
    Q_FOREACH (Meta meta, availaibleFormats) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(meta.getItag())));
        table->setItem(i, 1, new QTableWidgetItem(meta.getExtension()));
        table->setItem(i, 2, new QTableWidgetItem(meta.getResolution()));
        table->setItem(i, 3, new QTableWidgetItem(meta.getHumanReadableSize()));
        i++;
    }
    table->selectRow(0);

    QDir downloadDir;
    QSettings settings("Parallel Tube");
    qDebug() << settings.fileName();
    QString dirPath = settings.value("downloadDir").toString();
    if (dirPath.isEmpty()) {
        downloadDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    } else {
        downloadDir = QDir(dirPath);
    }
    ui->leDestination->setText(downloadDir.absoluteFilePath(video.getTitle() + "." + ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text()));

    connect(ui->bDownload, SIGNAL(clicked(bool)), this, SLOT(onDownloadClicked(bool)));
    connect(ui->bBrowse, SIGNAL(clicked(bool)), this, SLOT(onBrowseClicked(bool)));
    connect(ui->tableWidget, SIGNAL(cellActivated(int,int)), this, SLOT(tableRowChanged(int, int)) );

    progress = 0;

}

VideoInfo::~VideoInfo()
{
    delete ui;
}

void VideoInfo::tableRowChanged(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    QString oldFileName = ui->leDestination->text().split('/').back(); // TODO : Fix for windows
    QString newFileName = video.getTitle() + "." + ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
    QString filePath = ui->leDestination->text().replace(oldFileName, newFileName);
    ui->leDestination->setText(filePath);
}

void VideoInfo::onBrowseClicked(bool clicked)
{
    Q_UNUSED(clicked);
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly);
    int result = fileDialog.exec();
    if (result) {
        QDir downloadDir = QDir(fileDialog.selectedFiles()[0]);
        ui->leDestination->setText(downloadDir.absoluteFilePath(video.getTitle() + "." + ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text()));


        // TODO : Create a settings manager
        QSettings settings("Parallel Tube");
        settings.setValue("downloadDir", downloadDir.absolutePath());
    }
}

void VideoInfo::onDownloadClicked(bool checked)
{

    QFile file(ui->leDestination->text());
    bool isFileWritePossible = file.open(QIODevice::ReadWrite);
    if (!isFileWritePossible) {
        QMessageBox msgBox;
        msgBox.setText("Invalid file path");
        msgBox.exec();
        return;
    }

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
        QUrl url(meta.getUrl());
        QNetworkReply* reply = NetworkManager::getInstance().head(url);

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


    //DownloadManager *dm = new DownloadManager(meta, "googleio." + tableWidget->item(selectedRow, 1)->text(), this);
    DownloadManager *dm = new DownloadManager(meta, ui->bBrowse->text(), this);
    connect(dm, SIGNAL(progress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)) );
    connect(dm, SIGNAL(downloadCompleted()), this, SLOT(finished()) );
    dm->download();

}

void VideoInfo::finished()
{
    this->close();

    DownloadManager* dm = qobject_cast<DownloadManager*>(sender());
    delete dm;
}

void VideoInfo::downloadProgress(qint64 bytesReceived, qint64 totalBytes)
{
//    qDebug() << bytesReceived << " of " << totalBytes;
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesReceived);
}
