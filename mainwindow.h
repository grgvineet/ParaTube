#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "video.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void onParseUrlClicked(bool checked);
    void videoDataDownloaded(Video video);
};

#endif // MAINWINDOW_H
