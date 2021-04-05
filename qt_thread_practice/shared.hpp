#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

#include <QDebug>
#include <QString>
#include <QVector>
#include <QQueue>

#include <QThread>
#include <QFuture>
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

        static QString query_name_from_DB(QString username, QString password)
        {
            Q_UNUSED(username);
            Q_UNUSED(password);

            QThread::msleep(1000);

            return "404 Not Found";
        }
    };

    void CreateSampleThreads()
    {
        qDebug() << "#" << (int)QThread::currentThreadId();

        QtConcurrent::run(&SampleWorker::do_small_task).waitForFinished();

        QFuture<QString> f_name_db = QtConcurrent::run(&SampleWorker::query_name_from_DB, "vu", "123");
        QString str_name_db = f_name_db.result();
        qDebug() << "Query name from DB: " << str_name_db;

        SampleWorker worker;
        worker.start();
        worker.wait();
    }
}


#endif // SHARED_HPP
