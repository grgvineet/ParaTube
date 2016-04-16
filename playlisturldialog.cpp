#include "playlisturldialog.h"
#include "ui_playlisturldialog.h"
#include "videoinfo.h"
#include "youtube.h"

#include <QUrl>

PlaylistUrlDialog::PlaylistUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistUrlDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();
    ui->leUrl->setText("https://www.youtube.com/watch?v=-lXH1-FCcCY&index=1&list=PLjwBf9QEIO95j5kXIm9XAS_NAiu9_NDmv");
}

PlaylistUrlDialog::~PlaylistUrlDialog()
{
    delete ui;
}

void PlaylistUrlDialog::accept()
{
    ui->progressBar->setMaximum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    Youtube youtube;
    QUrl url = QUrl::fromUserInput(ui->leUrl->text());
    connect(&youtube, SIGNAL(playlistDataDownloaded(QVector<Video>)), this, SLOT(playlistDataDownloaded(QVector<Video>)) );
    youtube.downloadPlaylist(url);
}

void PlaylistUrlDialog::playlistDataDownloaded(QVector<Video> videos)
{
    this->close();

    Q_FOREACH(Video video, videos) {
        VideoInfo dialog(video, this);
        dialog.setModal(true);
        dialog.exec();
    }
}
