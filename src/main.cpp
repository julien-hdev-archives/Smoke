#include "Core/App.hpp"

int
main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    App application(argc, argv);

    return application.exec();
}