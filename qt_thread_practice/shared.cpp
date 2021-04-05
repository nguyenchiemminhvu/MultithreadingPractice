#include "shared.hpp"

namespace SampleThreads
{

    SampleWorker::SampleWorker()
    {

    }

    SampleWorker::~SampleWorker()
    {

    }

    void SampleWorker::run()
    {
        for (int i = 0; i < 10; i++)
        {
            qDebug() << "#" << (int)this->currentThreadId() << ": " << i;
        }
    }

    void SampleWorker::do_small_task()
    {
        for (int i = 0; i < 10; i++)
        {
            qDebug() << "#" << (int)QThread::currentThreadId() << ": " << i;
        }
    }

    QString SampleWorker::query_name_from_DB(const QByteArray &username, const QByteArray &password)
    {
        Q_UNUSED(username);
        Q_UNUSED(password);

        return "404 Not Found";
    }

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

        auto f_lambda = [=]() -> bool { return s_name_db.contains("404"); };
        QFuture<bool> f_stupid_work = QtConcurrent::run(f_lambda);
        bool f_check = f_stupid_work.result();
        if (f_check)
        {
            qDebug() << "It's really stupid thread";
        }

        worker.wait();
        f_small_work.waitForFinished();
    }
}


namespace WorkingSynchronously
{

    MyCounter::MyCounter()
        : m_count(0)
    {

    }

    MyCounter::~MyCounter()
    {

    }

    int MyCounter::GetCount()
    {
        return m_count;
    }

    void MyCounter::Increase()
    {
        for (int i = 0; i < 10000; i++)
        {
            m_locker.lock();
            m_count++;
            m_locker.unlock();
        }
    }

    void MyCounter::Decrease()
    {
        for (int i = 0; i < 10000; i++)
        {
            m_locker.lock();
            m_count--;
            m_locker.unlock();
        }
    }

    SampleWorker::SampleWorker(MyCounter *c, SampleWorker::Type t)
        : m_type(t)
    {
        m_counter = c;
    }

    SampleWorker::~SampleWorker()
    {

    }

    void SampleWorker::run()
    {
        switch (m_type)
        {
        case Type::INCREMENT:
            m_counter->Increase();
            break;

        case Type::DECREMENT:
            m_counter->Decrease();
            break;
        }
    }

    void RunExample()
    {
        MyCounter c;

        SampleWorker inc_worker(&c, SampleWorker::Type::INCREMENT);
        SampleWorker dec_worker(&c, SampleWorker::Type::DECREMENT);

        inc_worker.start();
        dec_worker.start();

        inc_worker.wait();
        dec_worker.wait();

        qDebug() << "Final result of counter: " << c.GetCount();
    }
}
