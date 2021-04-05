#pragma once

#ifndef SYNCHRONOUS_DIALOG_H
#define SYNCHRONOUS_DIALOG_H

#include <QDialog>

namespace Ui
{
    class Synchronous_Dialog;
}

class Synchronous_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Synchronous_Dialog(QWidget *parent = nullptr);
    ~Synchronous_Dialog();

private:
    Ui::Synchronous_Dialog *ui;
};

#endif // SYNCHRONOUS_DIALOG_H
