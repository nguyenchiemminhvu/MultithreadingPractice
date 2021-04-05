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

}

void Synchronous_Dialog::Uninitialize()
{
    if (m_timer)
    {
        delete m_timer;
    }
}
