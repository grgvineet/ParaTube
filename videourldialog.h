#ifndef VIDEOURLDIALOG_H
#define VIDEOURLDIALOG_H

#include "video.h"

#include <QDialog>

namespace Ui {
class VideoUrlDialog;
}

class VideoUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoUrlDialog(QWidget *parent = 0);
    ~VideoUrlDialog();

private:
    Ui::VideoUrlDialog *ui;

public slots:
    void accept();
    void videoDataDownloaded(Video video);
};

#endif // VIDEOURLDIALOG_H
