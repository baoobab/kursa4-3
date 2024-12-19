#include "application.h"

TApplication::TApplication(int argc, char *argv[])
    : QCoreApplication(argc,argv)
{
    TCommParams pars = { QHostAddress("127.0.0.1"), 9000,
                        QHostAddress("127.0.0.1"), 10001};
    comm = new TCommunicator(pars, this);

    TCommParams pars2 = { QHostAddress("127.0.0.1"), 9001,
                        QHostAddress("127.0.0.1"), 10001};
    comm2 = new TCommunicator(pars2, this);

    ats = new ATS();

    connect(comm,SIGNAL(received(QByteArray)),this,
            SLOT(fromCommunicator(QByteArray)));

    connect(ats,SIGNAL(request(QString)),
            this,SLOT(toCommunicator(QString)));
    qDebug() << "server started";
}

void TApplication::fromCommunicator(QByteArray msg)
{
    ats->answer(QString(msg));
}

void TApplication::toCommunicator(QString msg)
{
    // disconnect(comm,SIGNAL(received(QByteArray)),this,
    //            SLOT(fromCommunicator(QByteArray)));
    // todo podumat
    comm->send(QByteArray().append(msg.toUtf8()));
}
