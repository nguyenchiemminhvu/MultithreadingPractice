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

class SynchronousWorker : public QObject
{
    Q_OBJECT

public:
    SynchronousWorker(int id, double * income, int * turn, int round);
    ~SynchronousWorker();

    int RandomValue(int from, int to);

signals:
    void Finished();

public slots:
    void Process();

private:
    int m_id;
    double * p_income;
    int * p_turn;
    int m_round;

    QWaitCondition m_condition;
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
    void OnButtonStartClicked();

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
    QVector<QThread*> m_threads;

private:
    Ui::Synchronous_Dialog *ui;
};

#endif // SYNCHRONOUS_DIALOG_H
