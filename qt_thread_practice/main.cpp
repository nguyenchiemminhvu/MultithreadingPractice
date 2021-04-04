#include <QCoreApplication>

#include "shared.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello qt multithreading";

    return a.exec();
}
