#ifndef PERSON_CPP
#define PERSON_CPP

#include <QString>
#include "ats.h"
#include "person.h"
#include "QObject"

Person::Person(const QString& name, const QString& phone, ATS* ats) {
    this->name = name;
    this->phone = phone;
    this->ats = ats;
}

Person::~Person() {
    disconnect(ats, SIGNAL(messageReceived(QString, QString, QString)), this, SLOT(received(QString, QString, QString)));
}

QString Person::getName() const { return name; }
QString Person::getPhone() const { return phone; }

void Person::pickUpPhone() {
    bool isPickedUp = (*ats).addAbonent(name, phone);

    if (isPickedUp) {
        // Слушаем АТС, после успешного подключения
        connect(ats, SIGNAL(messageReceived(QString, QString, QString)), this, SLOT(received(QString, QString, QString)));
        // connect(ats, SIGNAL(callEnded(QString, QString)), this, SLOT(received(QString, QString, QString)));
    }
}

void Person::hangUpPhone() {
    bool isHangedUp = (*ats).removeAbonent(phone);

    if (isHangedUp) {
        // Отключаемс от АТС, после того как положили телефон
        disconnect(ats, SIGNAL(messageReceived(QString, QString, QString)), this, SLOT(received(QString, QString, QString)));
    }
}

void Person::makeCall(const Person* target) {
    (*ats).initiateCall(phone, target->getPhone());
}

void Person::endCall() {
    (*ats).endCall(phone);
}

void Person::sendMessage(const Person* target, const QString message) {
    (*ats).sendMessage(phone, target->getPhone(), message);
}

void Person::received(QString from, QString to, QString message) {
    if (to != phone) return;
    qDebug() << "Person " << name << " received message from:" << from << "to:" << to << "Text:" << message;
}

#endif // PERSON_CPP
