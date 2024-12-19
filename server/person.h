#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QObject>
#include "ats.h"


class Person : public QObject {
    Q_OBJECT
public:
    Person(const QString& name, const QString& phone, ATS* ats);
    ~Person(); // деструктор для корректного отключения сигналов

    QString getName() const;
    QString getPhone() const;

    void pickUpPhone();
    void hangUpPhone();
    void makeCall(const Person* target);
    void endCall();
    void sendMessage(const Person* target, const QString message);

public slots:
    void received(QString from, QString to, QString message);

private:
    QString name;
    QString phone;
    ATS* ats;
};

#endif // PERSON_H

