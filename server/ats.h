#ifndef ATS_H
#define ATS_H

#include "abonent.h"
#include <QMap>
#include <QDebug>

class ATS {
public:
    static void addAbonent(Abonent* abonent) {
        abonents.insert(abonent->getPhone(), abonent);
        qDebug() << "Added abonent:" << abonent->getName() << "with phone:" << abonent->getPhone();
    }

    static void removeAbonent(const QString& phone) {
        if (abonents.remove(phone) > 0) {
            qDebug() << "Removed abonent with phone:" << phone;
        } else {
            qDebug() << "Abonent not found with phone:" << phone;
        }
    }

    static Abonent* getAbonent(const QString& phone) {
        return abonents.contains(phone) ? abonents[phone] : nullptr;
    }

    static void initiateCall(const QString& callerPhone, const QString& targetPhone) {
        Abonent* caller = getAbonent(callerPhone);
        Abonent* target = getAbonent(targetPhone);

        if (!caller || !target) {
            qDebug() << "Caller or target not found.";
            return;
        }

        if (caller->getStatus() == Abonent::ConnectionStatus::Ready &&
            target->getStatus() == Abonent::ConnectionStatus::Ready) {
            caller->makeCall(target);
            target->receiveCall(caller);
        } else {
            qDebug() << "One of the abonents is not ready for a call.";
        }
    }

private:
    static QMap<QString, Abonent*> abonents; // Key: Phone number, Value: Abonent object
};

// Definition of the static member
QMap<QString, Abonent*> ATS::abonents;

#endif // ATS_H
