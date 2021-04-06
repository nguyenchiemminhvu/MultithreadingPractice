#include "synchronous_dialog.h"
#include "ui_synchronous_dialog.h"

Synchronous_Dialog::Synchronous_Dialog(QWidget *parent) :
    QDialog(parent),
    m_total_income(0),
    m_turn(0),
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

void Synchronous_Dialog::OnButtonStartClicked()
{
    m_total_income = 0;
    ui->button_start->setEnabled(false);

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        m_threads[i]->start();
    }
}

void Synchronous_Dialog::ResetThreads()
{
    m_threads.clear();
    InitializeThreads();

    ui->button_start->setEnabled(true);
}

void Synchronous_Dialog::Initialize()
{
    InitializeTimer();
    InitializeThreads();
    InitializeButtons();
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
        QThread * pThread = new QThread();
        m_threads.push_back(pThread);
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        SynchronousWorker * worker = new SynchronousWorker(i, &m_total_income, &m_turn, 10);
        worker->moveToThread(m_threads[i]);
        connect(m_threads[i], SIGNAL(started()), worker, SLOT(Process()));
        connect(worker, SIGNAL(Finished()), this, SLOT(ResetThreads()));
        connect(worker, SIGNAL(Finished()), m_threads[i], SLOT(quit()));
        connect(worker, SIGNAL(Finished()), worker, SLOT(deleteLater()));
        connect(m_threads[i], SIGNAL(finished()), m_threads[i], SLOT(deleteLater()));
    }
}

void Synchronous_Dialog::InitializeButtons()
{
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(OnButtonStartClicked()));
}

void Synchronous_Dialog::Uninitialize()
{
    if (m_timer)
    {
        delete m_timer;
    }
}

SynchronousWorker::SynchronousWorker(int id, double * income, int * turn, int round)
    : m_id(id),
      m_round(round)
{
    p_income = income;
    p_turn = turn;
}

SynchronousWorker::~SynchronousWorker()
{

}

int SynchronousWorker::RandomValue(int from, int to)
{
    return qrand() % ((to + 1) - from) + from;
}

void SynchronousWorker::Process()
{
    if (p_income)
    {
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        for (int i = 0; i < m_round; i++)
        {
            int sign = RandomValue(0, 1);
            int cash = RandomValue(1, 10);
            *p_income += sign ? -cash : cash;

            QThread::currentThread()->msleep(500);
        }
    }

    emit Finished();
}
