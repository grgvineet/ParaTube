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

private slots:
    void on_addVideo_triggered();
    void on_addPlaylist_triggered();
};

#endif // MAINWINDOW_H
