#ifndef ABONENT_H
#define ABONENT_H

#include <QDebug>

class Abonent {
public:
    enum class  ConnectionStatus {
        Ready,
        InCall,
        OnHold,
        Free
    };

    Abonent(const QString& phone, const quint16 address) {
        this->phone = phone;
        this->address = address;
        this->status = ConnectionStatus::Ready;
    }

    ConnectionStatus getStatus() const { return status; }
    quint16 getAddress() const { return address; }
    QString getPhone() const { return phone; }
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
};

#endif // ABONENT_H
