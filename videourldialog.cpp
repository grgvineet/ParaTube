#include "videourldialog.h"
#include "ui_videourldialog.h"
#include "youtube.h"
#include "videoinfo.h"

VideoUrlDialog::VideoUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoUrlDialog)
{
    ui->setupUi(this);

    ui->leUrl->setText("https://www.youtube.com/watch?v=a1Y73sPHKxw");
}

VideoUrlDialog::~VideoUrlDialog()
{
    delete ui;
}

void VideoUrlDialog::accept()
{
    Youtube youtube;
    QString string(ui->leUrl->text());
    connect(&youtube, SIGNAL(videoDataDownloaded(Video)), this, SLOT(videoDataDownloaded(Video)) );
    youtube.downloadVideo(string);
}

void VideoUrlDialog::videoDataDownloaded(Video video)
{
    VideoInfo dialog(video, this);
    dialog.setModal(true);
    this->close();
    dialog.exec();
}
