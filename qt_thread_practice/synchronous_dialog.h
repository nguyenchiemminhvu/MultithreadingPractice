#pragma once

#ifndef SYNCHRONOUS_DIALOG_H
#define SYNCHRONOUS_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QQueue>
#include <QTime>
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

///////////////////////////////////////////////////////////////////////////////////////
/// \brief The SynchronousWorker class
///
class SynchronousWorker : public QObject
{
    Q_OBJECT

public:
    SynchronousWorker(int id, double * income, int * turn, int round, QMutex * locker);
    ~SynchronousWorker();

    int RandomValue(int from, int to);

signals:
    void Finished();
    void Finished(int);

public slots:
    void Process();

private:
    int m_id;
    double * p_income;
    int * p_turn;
    int m_round;

    QMutex * p_locker;
};

///////////////////////////////////////////////////////////////////////////////////////
/// \brief The Synchronous_Dialog class
///
class Synchronous_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Synchronous_Dialog(QWidget *parent = nullptr);
    ~Synchronous_Dialog();

signals:

public slots:
    void OnTimerTicked();
    void OnButtonStartClicked();
    void OnThreadFinished(int id);
    void ResetThreads();

private:
    void Initialize();
    void InitializeTimer();
    void InitializeThreads();
    void InitializeButtons();
    void Uninitialize();

private:
    double m_total_income;
    int m_turn;
    QTimer * m_timer;
    QMutex * m_mutex;
    QVector<bool> m_threads_stats;
    QVector<QThread*> m_threads;

private:
    Ui::Synchronous_Dialog *ui;
};

#endif // SYNCHRONOUS_DIALOG_H
