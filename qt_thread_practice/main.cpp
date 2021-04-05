#include <QApplication>

#include "shared.hpp"
#include "synchronous_dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //SampleThreads::CreateSampleThreads();
    //WorkingSynchronously::RunExample();

    Synchronous_Dialog * dialog;
    dialog = new Synchronous_Dialog();
    dialog->show();

    return app.exec();
}
