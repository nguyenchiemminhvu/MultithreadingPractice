#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

#include <QDebug>
#include <QString>
#include <QVector>
#include <QQueue>

#include <QThread>
#include <QMutex>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#define NUMBER_OF_THREADS (4)

namespace SampleThreads
{
    class SampleWorker : public QThread
    {
        Q_OBJECT

    public:
        SampleWorker();
        ~SampleWorker();

    signals:

    public slots:

    public:

        virtual void run() override;

        static void do_small_task();

        static QString query_name_from_DB(const QByteArray& username, const QByteArray& password);
    };

    void CreateSampleThreads();
}

namespace WorkingSynchronously
{
    class MyCounter
    {
    public:
        MyCounter();
        ~MyCounter();

        int GetCount();

        void Increase();

        void Decrease();

    private:
        QMutex m_locker;
        int m_count;
    };

    class SampleWorker : public QThread
    {
        Q_OBJECT
    public:
        enum Type
        {
            INCREMENT,
            DECREMENT
        };

    public:
        SampleWorker(MyCounter * c, Type t = INCREMENT);
        ~SampleWorker();

    signals:

    public slots:

    public:

        virtual void run() override;

    private:
        Type m_type;
        MyCounter * m_counter;
    };

    void RunExample();
}


#endif // SHARED_HPP
