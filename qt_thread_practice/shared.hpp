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
            }
        }

        static void do_small_task()
        {
            for (int i = 0; i < 10; i++)
            {
                qDebug() << "#" << (int)QThread::currentThreadId() << ": " << i;
            }
        }

        static QString query_name_from_DB(const QByteArray& username, const QByteArray& password)
        {
            Q_UNUSED(username);
            Q_UNUSED(password);

            return "404 Not Found";
        }
    };

    void CreateSampleThreads()
    {
        qDebug() << "#" << (int)QThread::currentThreadId();

        SampleWorker worker;
        worker.start();

        QFuture<void> f_small_work = QtConcurrent::run(&SampleWorker::do_small_task);

        QByteArray username = QString("vu").toUtf8();
        QByteArray password = QString("123").toUtf8();
        QFuture<QString> f_name_db = QtConcurrent::run(&SampleWorker::query_name_from_DB, username, password);
        QString s_name_db = f_name_db.result();
        qDebug() << "Query name from DB: " << s_name_db;

        worker.wait();
        f_small_work.waitForFinished();
    }
}


#endif // SHARED_HPP
