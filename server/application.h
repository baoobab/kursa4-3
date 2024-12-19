#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QCoreApplication>
#include "ats.h"
#include "communicator.h"

class TApplication : public QCoreApplication
{
    Q_OBJECT

    TCommunicator *comm;
    TCommunicator *comm2;


public:
    ATS    *ats;
    TApplication(int, char**);

public slots:
    void fromCommunicator(QByteArray);
    void toCommunicator(QString);

};

#endif // APPLICATION_H
