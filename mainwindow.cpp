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

#include <QList>
#include <QSslSocket>
#include <QSslCipher>
#include <QDebug>
#include <QSslConfiguration>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "This is a string";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addVideo_triggered()
{
    qDebug() << QSslSocket::supportsSsl();
    QList<QSslCipher> supportedCiphers = QSslSocket::supportedCiphers();
    Q_FOREACH(QSslCipher cipher, supportedCiphers) {
        qDebug() << cipher.name();
    }
    QList<QSslCipher> defaultCiphers = QSslSocket::defaultCiphers();
    Q_FOREACH(QSslCipher cipher, defaultCiphers) {
        qDebug() << cipher.name();
    }

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
