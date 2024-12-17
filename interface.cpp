#include "interface.h"

TInterface::TInterface(int& argc, char** argv) : app(argc, argv) {
    abonentManager = new AbonentManager();
    abonentManager->setWindowTitle("Мини-АТС");
}

void TInterface::run() {
    abonentManager->show();
    app.exec();
}

TInterface::~TInterface()
{
    delete abonentManager;
}
