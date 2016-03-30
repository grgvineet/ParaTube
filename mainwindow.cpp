#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "youtube.h"

#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Youtube youtube;
    QString string("");
    youtube.downloadVideo(string);

}

MainWindow::~MainWindow()
{
    delete ui;
}
