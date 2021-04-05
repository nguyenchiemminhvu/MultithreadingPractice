#include "synchronous_dialog.h"
#include "ui_synchronous_dialog.h"

Synchronous_Dialog::Synchronous_Dialog(QWidget *parent) :
    QDialog(parent),
    m_total_income(0),
    ui(new Ui::Synchronous_Dialog)
{
    ui->setupUi(this);

    Initialize();
}

Synchronous_Dialog::~Synchronous_Dialog()
{
    Uninitialize();

    delete ui;
    this->deleteLater();
}

void Synchronous_Dialog::OnTimerTicked()
{
    ui->tick_count->display(m_total_income);
}

void Synchronous_Dialog::Initialize()
{
    InitializeTimer();
    InitializeThreads();
}

void Synchronous_Dialog::InitializeTimer()
{
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTicked()));
    connect(m_timer, SIGNAL(timeout()), m_timer, SLOT(start()));
    m_timer->start(100);
}

void Synchronous_Dialog::InitializeThreads()
{
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        m_threads[i] = new QThread();
        m_conditions[i] = new QWaitCondition();
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        SynchronousWorker * worker = new SynchronousWorker(&m_total_income, 10);
        worker->moveToThread(m_threads[i]);
        connect(m_threads[i], SIGNAL(started()), worker, SLOT(Process()));
        connect(m_threads[i], SIGNAL(finished()), m_threads[i], SLOT(deleteLater()));
    }
}

void Synchronous_Dialog::Uninitialize()
{
    if (m_timer)
    {
        delete m_timer;
    }
}

SynchronousWorker::SynchronousWorker(double *income, int round)
    : p_income(income), m_round(round)
{

}

SynchronousWorker::~SynchronousWorker()
{

}

void SynchronousWorker::Process()
{
    for (int i = 0; i < m_round; i++)
    {
        *p_income++;
    }
}
