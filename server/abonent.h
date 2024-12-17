#ifndef ABONENT_H
#define ABONENT_H

#include "person.h"
#include "../common/communicator.h"
#include <QDebug>

class Abonent : public Person {
public:
    enum class ConnectionStatus {
        Ready,
        InCall,
        OnHold,
        Free
    };

    Abonent(const QString& name, const QString& phone)
        : Person(name, phone), status(ConnectionStatus::Free) {
        TCommParams params;
        params.rHost = QHostAddress::LocalHost;
        params.rPort = abonentPort;
        params.sHost = QHostAddress::LocalHost;
        params.sPort = ++abonentPort;

        communicator(params);
        connect(communicator, &TCommunicator::received, this, &Abonent::onMessageReceived);
    }

    ~Abonent() {
        delete communicator;
    }

    ConnectionStatus getStatus() const { return status; }
    void setStatus(ConnectionStatus newStatus) {
        status = newStatus;
        qDebug() << "Abonent" << getName() << "status changed to" << static_cast<int>(newStatus);
    }

    void makeCall(Abonent* target);
    void receiveCall(Abonent* caller);
    void endCall();

    void sendMessage(const QString& message) {
        QByteArray msg = message.toUtf8();
        communicator->send(msg);
        qDebug() << getName() << "sent message:" << message;
    }

private slots:
    void onMessageReceived(QByteArray msg) {
        QString message = QString::fromUtf8(msg);
        qDebug() << getName() << "received message:" << message;
        // Handle incoming messages (e.g., display in chat)
    }

private:
    ConnectionStatus status;
    TCommunicator* communicator; // Communicator instance for sending/receiving messages

    static quint16 abonentPort = 10000;
};

#endif // ABONENT_H
