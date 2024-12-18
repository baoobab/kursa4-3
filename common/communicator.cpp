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


// с отдельными сокетами (тож не работает)
// #include "communicator.h"
// #include <QDebug>


// TCommunicator::TCommunicator(TCommParams& pars, QObject* parent) : QObject(parent), ready_(false) {
//     params_ = pars;
//     socket_ = new QUdpSocket(this);
//     ready_ = socket_->bind(params_.rHost, params_.rPort,
//                            QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
//     if (!ready_) {
//         qCritical() << "Failed to bind to port" << params_.rPort;
//         delete socket_;
//         socket_ = nullptr;
//         return;
//     }
//     // connect(socket_, &QUdpSocket::readyRead, this, &TCommunicator::processPendingDatagrams);
//     connect(socket_,SIGNAL(readyRead()),this,SLOT(receive()));
// }

// TCommunicator::~TCommunicator() {
//     delete socket_;
// }

// bool TCommunicator::isReady() {
//     return ready_;
// }

// void TCommunicator::send(QByteArray msg) {
//     if (ready_) {
//         socket_->writeDatagram(msg, params_.sHost, params_.sPort);
//         qDebug() << "Sent" << msg << "to:" << params_.sHost.toString() << ":" << (int)params_.sPort;
//     }
// }

// void TCommunicator::receive() {
//     while (socket_->hasPendingDatagrams()) {
//         QByteArray datagram;
//         datagram.resize(socket_->pendingDatagramSize());
//         QHostAddress sender;
//         quint16 senderPort;
//         qint64 bytesRead = socket_->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
//         if (bytesRead > 0) {
//             emit received(datagram);
//         }
//     }
// }
// с отдельными сокетами (тож не работает)
