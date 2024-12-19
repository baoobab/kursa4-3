#include "application.h"

TApplication::TApplication(int argc, char *argv[])
    : QCoreApplication(argc, argv) {
    qDebug("server started");
    ats = new ATS();
}


