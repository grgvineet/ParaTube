#include "videourldialog.h"
#include "ui_videourldialog.h"
#include "youtube.h"
#include "videoinfo.h"

#include <QUrl>

VideoUrlDialog::VideoUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoUrlDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();
    ui->leUrl->setText("https://www.youtube.com/watch?v=a1Y73sPHKxw");
}

VideoUrlDialog::~VideoUrlDialog()
{
    delete ui;
}

void VideoUrlDialog::accept()
{
    ui->progressBar->setMaximum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    Youtube youtube;
    QUrl url = QUrl::fromUserInput(ui->leUrl->text());
    connect(&youtube, SIGNAL(videoDataDownloaded(Video)), this, SLOT(videoDataDownloaded(Video)) );
    youtube.downloadVideo(url);
}

void VideoUrlDialog::videoDataDownloaded(Video video)
{
    ui->progressBar->hide();

    VideoInfo dialog(video, this);
    dialog.setModal(true);
    this->close();
    dialog.exec();
}
