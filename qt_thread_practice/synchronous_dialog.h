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
#include <QWaitCondition>
#include <QtConcurrent/qtconcurrentrun.h>

#include "shared.hpp"

namespace Ui
{
    class Synchronous_Dialog;
}

class SynchronousWorker : public QObject
{
public:
    SynchronousWorker(double * income, int round);
    ~SynchronousWorker();

signals:
    void Finished();

public slots:
    void Process();

private:
    double * p_income;
    int m_round;
};

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
    double m_total_income;

    QTimer * m_timer;
    QVector<QThread*> m_threads;
    QVector<QWaitCondition*> m_conditions;

private:
    Ui::Synchronous_Dialog *ui;
};

#endif // SYNCHRONOUS_DIALOG_H
