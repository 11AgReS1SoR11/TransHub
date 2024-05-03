#include <QCoreApplication>
#include "ServiceManager.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    ServiceManager manager;
    return app.exec();
}
