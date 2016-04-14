#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playlisturldialog.h"
#include "video.h"
#include "videoinfo.h"
#include "videourldialog.h"
#include "youtube.h"

#include <QtCore>
#include <QString>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addVideo_triggered()
{
    VideoUrlDialog dialog;
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_addPlaylist_triggered()
{
    PlaylistUrlDialog dialog;
    dialog.setModal(true);
    dialog.exec();
}
