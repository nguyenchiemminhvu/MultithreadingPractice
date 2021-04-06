#include "synchronous_dialog.h"
#include "ui_synchronous_dialog.h"

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::Synchronous_Dialog
/// \param parent
///
Synchronous_Dialog::Synchronous_Dialog(QWidget *parent) :
    QDialog(parent),
    m_total_income(0),
    m_turn(0),
    m_mutex(NULL),
    ui(new Ui::Synchronous_Dialog)
{
    ui->setupUi(this);

    Initialize();
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::~Synchronous_Dialog
///
Synchronous_Dialog::~Synchronous_Dialog()
{
    Uninitialize();

    delete ui;
    this->deleteLater();
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::OnTimerTicked
///
void Synchronous_Dialog::OnTimerTicked()
{
    ui->tick_count->display(m_total_income);
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::OnButtonStartClicked
///
void Synchronous_Dialog::OnButtonStartClicked()
{
    m_total_income = 0;
    ui->button_start->setEnabled(false);

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        m_threads[i]->start();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::OnThreadFinished
/// \param id
///
void Synchronous_Dialog::OnThreadFinished(int id)
{
    m_threads_stats[id] = true;

    bool all_done = true;
    for (bool b : m_threads_stats)
    {
        all_done &= b;
    }

    if (all_done)
    {
        ResetThreads();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::ResetThreads
///
void Synchronous_Dialog::ResetThreads()
{
    m_threads.clear();
    m_threads_stats.clear();
    InitializeThreads();

    ui->button_start->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::Initialize
///
void Synchronous_Dialog::Initialize()
{
    InitializeTimer();
    InitializeThreads();
    InitializeButtons();
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::InitializeTimer
///
void Synchronous_Dialog::InitializeTimer()
{
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTicked()));
    connect(m_timer, SIGNAL(timeout()), m_timer, SLOT(start()));
    m_timer->start(100);
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::InitializeThreads
///
void Synchronous_Dialog::InitializeThreads()
{
    if (m_mutex)
    {
        delete m_mutex;
        m_mutex = NULL;
    }
    m_mutex = new QMutex();

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        QThread * pThread = new QThread();
        m_threads.push_back(pThread);
        m_threads_stats.push_back(false);
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        SynchronousWorker * worker = new SynchronousWorker(i, &m_total_income, &m_turn, 10, m_mutex);
        worker->moveToThread(m_threads[i]);
        connect(m_threads[i], SIGNAL(started()), worker, SLOT(Process()));
        connect(worker, SIGNAL(Finished(int)), this, SLOT(OnThreadFinished(int)));
        connect(worker, SIGNAL(Finished()), m_threads[i], SLOT(quit()));
        connect(worker, SIGNAL(Finished()), worker, SLOT(deleteLater()));
        connect(m_threads[i], SIGNAL(finished()), m_threads[i], SLOT(deleteLater()));
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::InitializeButtons
///
void Synchronous_Dialog::InitializeButtons()
{
    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(OnButtonStartClicked()));
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief Synchronous_Dialog::Uninitialize
///
void Synchronous_Dialog::Uninitialize()
{
    if (m_timer)
    {
        delete m_timer;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief SynchronousWorker::SynchronousWorker
/// \param id
/// \param income
/// \param turn
/// \param round
///
SynchronousWorker::SynchronousWorker(int id, double * income, int * turn, int round, QMutex * locker)
    : m_id(id),
      m_round(round)
{
    p_income = income;
    p_turn = turn;
    p_locker = locker;
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief SynchronousWorker::~SynchronousWorker
///
SynchronousWorker::~SynchronousWorker()
{

}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief SynchronousWorker::RandomValue
/// \param from
/// \param to
/// \return
///
int SynchronousWorker::RandomValue(int from, int to)
{
    return qrand() % ((to + 1) - from) + from;
}

///////////////////////////////////////////////////////////////////////////////////////
/// \brief SynchronousWorker::Process
///
void SynchronousWorker::Process()
{
    if (p_income)
    {
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        for (int i = 0; i < m_round * 10000; i++)
        {
            p_locker->lock();
            (*p_income)++;
            p_locker->unlock();

            /*
            int sign = RandomValue(0, 1);
            int cash = RandomValue(1, 10);
            p_locker->lock();
            (*p_income) += sign ? -cash : cash;
            p_locker->unlock();
            */
        }
    }

    emit Finished();
    emit Finished(m_id);
}
