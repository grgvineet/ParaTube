#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "video.h"
#include "videoinfo.h"
#include "youtube.h"

#include <QtCore>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->bParseUrl, SIGNAL(clicked(bool)), this, SLOT(onParseUrlClicked(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onParseUrlClicked(bool checked)
{
    Youtube youtube;
    QString string(ui->leUrl->text());
    connect(&youtube, SIGNAL(videoDataDownloaded(Video)), this, SLOT(videoDataDownloaded(Video)) );
    youtube.downloadVideo(string);
}

void MainWindow::videoDataDownloaded(Video video)
{
    QDialog* dialog = new VideoInfo(video, this);
    dialog->show();
}
