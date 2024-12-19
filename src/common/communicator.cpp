#include "communicator.h"

TCommunicator::TCommunicator(TCommParams& pars, QObject *parent) : QUdpSocket(parent)
{
    params = pars;
    ready = bind(params.rHost, params.rPort,
                 QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
    if (ready) connect(this,SIGNAL(readyRead()),this,SLOT(receive()));
    else qCritical() << "Failed to bind to port";
}


bool TCommunicator::isReady()
{
    return ready;
}

void TCommunicator::send(QByteArray msg)
{
    if (ready)
    {
        writeDatagram(msg, params.sHost, params.sPort);
        qDebug()<<"sended"<< msg << " from: " << params.sHost.toString() << ":" << (int)params.sPort;
    }
}

void TCommunicator::receive()
{
    if (hasPendingDatagrams())
    {
        quint64 size = pendingDatagramSize();
        QByteArray msg(size,'\0');
        QHostAddress sender;
        quint16 senderPort;
        qint64 bytesRead = readDatagram(msg.data(), size, &sender, &senderPort);

        if (bytesRead > 0) {
            qDebug()<<"recieved "<< msg << " from: " << sender.toString() << ":" << (int)senderPort;
            emit received(msg);
        }
    }
}
