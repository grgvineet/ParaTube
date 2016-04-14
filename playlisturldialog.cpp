#include "playlisturldialog.h"
#include "ui_playlisturldialog.h"

PlaylistUrlDialog::PlaylistUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistUrlDialog)
{
    ui->setupUi(this);
}

PlaylistUrlDialog::~PlaylistUrlDialog()
{
    delete ui;
}
