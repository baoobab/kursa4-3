#ifndef ABONENT_H
#define ABONENT_H

#include <QDebug>
#include "QRandomGenerator"

class Abonent {
public:
    enum class  ConnectionStatus {
        Ready,
        InCall,
        OnHold,
        Free
    };

    Abonent(const QString& name, const QString& phone, quint16 address = 0) {
        this->name = name;
        this->phone = phone;
        this->address = address;
        if (address == 0) {
            this->address = 10000 + QRandomGenerator::global()->bounded(20000 - 10000 + 1); // пока так
        }
        this->status = ConnectionStatus::OnHold;
    }

    ConnectionStatus getStatus() const { return status; }
    quint16 getAddress() const { return address; }
    QString getPhone() const { return phone; }
    QString getName() const { return name; }

    void setStatus(ConnectionStatus newStatus) {
        status = newStatus;
        qDebug() << "Abonent" << getPhone() << "status changed to" << static_cast<int>(newStatus);
    }

    // void makeCall(Abonent* target);
    // void receiveCall(Abonent* caller);
    // void endCall();

    // void sendMessage(const QString& message) {

    // }


private:
    ConnectionStatus status;
    quint16 address;
    QString phone;
    QString name;
};

#endif // ABONENT_H
