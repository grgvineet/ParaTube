#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include "video.h"

#include <QDialog>
#include <QFile>
#include <QNetworkAccessManager>

namespace Ui {
class VideoInfo;
}

class VideoInfo : public QDialog
{
    Q_OBJECT

    qint64 progress;
public:
    explicit VideoInfo(Video video, QWidget *parent = 0);
    ~VideoInfo();

private:
    Ui::VideoInfo *ui;
    Video video;
    QNetworkAccessManager* mgr;

private slots:
    void tableRowChanged(int row, int column);
    void onBrowseClicked(bool clicked);
    void onDownloadClicked(bool checked);
    void finished();
    void downloadProgress(qint64, qint64);
};

#endif // VIDEOINFO_H
