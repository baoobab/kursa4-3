#include <QApplication>
#include "client/interface.h"

int main(int argc, char** argv) {
    TInterface interface(argc, argv);
    interface.run();

    return 0;
}
