#include <QCoreApplication>

#include "shared.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //SampleThreads::CreateSampleThreads();
    WorkingSynchronously::RunExample();

    return a.exec();
}
