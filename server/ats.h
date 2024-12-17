#ifndef ATS_H
#define ATS_H

#include "abonent.h"
#include <QMap>
#include <QDebug>

class ATS {
public:
    void addAbonent(Abonent* abonent) {
        abonents.insert(abonent->getPhone(), abonent);
        qDebug() << "Added abonent:" << abonent->getName() << "with phone:" << abonent->getPhone();
    }

    void removeAbonent(const QString& phone) {
        if (abonents.remove(phone) > 0) {
            qDebug() << "Removed abonent with phone:" << phone;
        } else {
            qDebug() << "Abonent not found with phone:" << phone;
        }
    }

    Abonent* getAbonent(const QString& phone) {
        return abonents.contains(phone) ? abonents[phone] : nullptr;
    }

    void changeAbonentStatus(const QString& phone, Abonent::ConnectionStatus newStatus) {
        if (abonents.contains(phone)) {
            abonents[phone]->setStatus(newStatus);
            qDebug() << "Changed status of abonent with phone:" << phone;
        } else {
            qDebug() << "Abonent not found with phone:" << phone;
        }
    }

private:
    QMap<QString, Abonent*> abonents; // Key: Phone number, Value: Abonent object
};

#endif // ATS_H
