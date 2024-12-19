#ifndef ABONENT_CPP
#define ABONENT_CPP

#include <QDebug>
#include "abonent.h"
#include "map"
#include "QRandomGenerator"
#include "../common/communicator.h"

Abonent::Abonent(const QString& name, const QString& phone) {
    this->phone = phone;
    this->name = name;
    this->address = 10001 + QRandomGenerator::global()->bounded(20000 - 10001 + 1); // пока так
    this->status = ConnectionStatus::Ready; // Т.к. когда создается абонент - происходит событие снятия трубки
}

Abonent::ConnectionStatus Abonent::getStatus() const { return status; }
quint16 Abonent::getAddress() const { return address; }
QString Abonent::getPhone() const { return phone; }
QString Abonent::getName() const { return name; }

bool Abonent::makeCall() {
    return changeStatus(ConnectionStatus::InCall);
}
bool Abonent::receiveCall() {
    return changeStatus(ConnectionStatus::InCall);
}
bool Abonent::endCall() {
    return changeStatus(ConnectionStatus::Ready);
}

bool Abonent::changeStatus(ConnectionStatus newStatus) {
    // Таблица переходов (машина состояний)
    static const std::map<ConnectionStatus, std::map<ConnectionStatus, bool>> transitions = {
        {ConnectionStatus::Ready, {{ConnectionStatus::InCall, true}}}, // , {ConnectionStatus::Free, false}
        {ConnectionStatus::InCall, {{ConnectionStatus::Ready, true}}}, // , {ConnectionStatus::Free, false}
        // {ConnectionStatus::Free, {{ConnectionStatus::Ready, true}, {ConnectionStatus::InCall, false}}}
    };

    auto it = transitions.find(status);
    if (it != transitions.end()) {
        auto it2 = it->second.find(newStatus);
        if (it2 != it->second.end() && it2->second) {
            status = newStatus;
            qDebug() << "Abonent" << phone << "status changed to" << (int)newStatus;
            return true;
        }
    }
    qDebug() << "Invalid status transition for abonent" << phone << "from" << (int)status << "to" << (int)newStatus;
    return false; // Недопустимый переход
}


#endif // ABONENT_CPP
