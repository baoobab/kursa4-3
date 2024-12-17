#ifndef ABONENT_H
#define ABONENT_H

#include "person.h"

class Abonent : public Person {
public:
    enum class ConnectionStatus {
        Ready,
        InCall,
        OnHold,
        Free
    };

    Abonent(const QString& name, const QString& phone, ConnectionStatus status = ConnectionStatus::Free)
        : Person(name, phone), status(status) {}

    ConnectionStatus getStatus() const { return status; }
    void setStatus(ConnectionStatus newStatus) { status = newStatus; }

private:
    ConnectionStatus status;
};

#endif // ABONENT_H
