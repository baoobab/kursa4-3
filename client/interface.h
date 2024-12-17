#ifndef INTERFACE_H
#define INTERFACE_H

#include <QApplication>
#include <QWidget>
#include "abonentManager.h"

class TInterface {
public:
    TInterface(int& argc, char** argv);
    ~TInterface();
    void run();

private:
    QApplication app;
    AbonentManager* abonentManager;
};

#endif // INTERFACE_H
