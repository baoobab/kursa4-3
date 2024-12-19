#ifndef ABONENT_H
#define ABONENT_H

#include <QString>
#include <QDebug>
#include <QRandomGenerator>


class Abonent {
public:
    enum class ConnectionStatus {
        Ready,      // Свободен, снял трубку, может принимать и отправлять звонки
        InCall,     // Участвует в звонке
        Free,       // Положил трубку, не может принимать и отправлять звонки
    };

    Abonent(const QString& name, const QString& phone);

    ConnectionStatus getStatus() const;
    quint16 getAddress() const;
    QString getPhone() const;
    QString getName() const;

    bool makeCall();
    bool receiveCall();
    bool endCall();

private:
    bool changeStatus(ConnectionStatus newStatus);

    ConnectionStatus status;
    quint16 address;
    QString phone;
    QString name;
};

#endif // ABONENT_H
