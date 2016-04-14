#ifndef PLAYLISTURLDIALOG_H
#define PLAYLISTURLDIALOG_H

#include <QDialog>

namespace Ui {
class PlaylistUrlDialog;
}

class PlaylistUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistUrlDialog(QWidget *parent = 0);
    ~PlaylistUrlDialog();

private:
    Ui::PlaylistUrlDialog *ui;
};

#endif // PLAYLISTURLDIALOG_H
