#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include "ats.h"
#include "QObject"
#include "QRandomGenerator"
#include "../common/communicator.h"


class Person : public QObject {
public:
    Person(const QString& name, const QString& phone, ATS* ats) {
        this->name = name;
        this->phone = phone;
        this->address = ATS::address + QRandomGenerator::global()->bounded(20000 - ATS::address + 1); // пока так
        this->ats = ats;

        // TCommParams paramsPersonToATS = { QHostAddress::LocalHost, ATS::address,
                                         // QHostAddress::LocalHost, address };
        // TCommunicator* commPerson = new TCommunicator(paramsPersonToATS, this);

        connect(ats, SIGNAL(messageReceived(QString, QString, QString)), this, SLOT(onMessageReceived(QString, QString, QString)));

    }

    // ~Person() {
    //     delete communicator;
    // }

    QString getName() const { return name; }
    QString getPhone() const { return phone; }
    quint16 getAddress() const { return address; }

    void enrollToATS() {
        (*ats).addAbonent(phone, address);
    }

    void makeCall(const Person* target) {
        (*ats).initiateCall(phone, target->getPhone());
    }

    void receiveCall(Abonent* caller);
    void endCall();

    void sendMessage(const Person* target, const QString message) {
        (*ats).sendMessage(phone, target->getPhone(), message);
    }

public slots:
    void onMessageReceived(QString from, QString to, QString message) {
        if (to != phone) {
            return;
        }
        qDebug() << "Person " << name << " received message from:" << from << "to:" << to << "Text:" << message;
    }

// public slots:

//     void receive(QByteArray msg) {
//         QString message = QString::fromUtf8(msg);
//         if (message.length() > 0) {
//             qDebug() << getName() << " received message:" << message;
//         }
//     }
private:
    Q_OBJECT

    QString name;
    QString phone;
    quint16 address;
    // TCommunicator* communicator;
    ATS* ats;
};

#endif // PERSON_H
