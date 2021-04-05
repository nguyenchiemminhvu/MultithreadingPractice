#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

#include <QDebug>
#include <QString>
#include <QVector>
#include <QQueue>

#include <QThread>
#include <QtConcurrent/qtconcurrentrun.h>

namespace SampleThreads
{
    class SampleWorker : public QThread
    {
    public:
        SampleWorker()
        {

        }

        ~SampleWorker()
        {

        }

        virtual void run() override
        {
            for (int i = 0; i < 10; i++)
            {
                qDebug() << "#" << (int)this->currentThreadId() << ": " << i;
                QThread::msleep(1000);
            }
        }

        static void do_small_task()
        {
            for (int i = 0; i < 10; i++)
            {
                qDebug() << "#" << (int)QThread::currentThreadId() << ": " << i;
            }
        }
    };

    void CreateSampleThreads()
    {
        qDebug() << "#" << (int)QThread::currentThreadId();

        QtConcurrent::run(&SampleWorker::do_small_task).waitForFinished();

        SampleWorker worker;
        worker.start();
        worker.wait();
    }
}


#endif // SHARED_HPP
