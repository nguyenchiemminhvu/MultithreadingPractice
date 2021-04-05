#include "synchronous_dialog.h"
#include "ui_synchronous_dialog.h"

Synchronous_Dialog::Synchronous_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Synchronous_Dialog)
{
    ui->setupUi(this);
}

Synchronous_Dialog::~Synchronous_Dialog()
{
    delete ui;
    this->deleteLater();
}
