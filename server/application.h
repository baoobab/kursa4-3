#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QCoreApplication>
#include "ats.h"

class TApplication : public QCoreApplication
{
    Q_OBJECT
public:
    TApplication(int, char**);
    ATS* ats;
};

#endif // APPLICATION_H
