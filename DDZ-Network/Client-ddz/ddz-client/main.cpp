#include <QtWidgets/QApplication>
#include <QResource>
#include "Cards.h"
// #include "Loading.h"
#include "Login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    // QResource::registerResource("./resource.rcc");
    // Loading w;
    Login w;
    w.show();
    return a.exec();
}
