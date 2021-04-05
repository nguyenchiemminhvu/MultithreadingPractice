#pragma once

#ifndef SYNCHRONOUS_DIALOG_H
#define SYNCHRONOUS_DIALOG_H

#include <QDialog>

#include <QDebug>
#include <QString>
#include <QVector>
#include <QQueue>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

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

signals:

public slots:
    void OnTimerTicked();

private:
    void Initialize();
    void InitializeTimer();
    void InitializeThreads();
    void Uninitialize();

private:
    QTimer * m_timer;
    double m_total_income;

private:
    Ui::Synchronous_Dialog *ui;
};

#endif // SYNCHRONOUS_DIALOG_H
