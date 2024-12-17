#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person {
public:
    Person(const QString& name, const QString& phone)
        : name(name), phone(phone) {}

    QString getName() const { return name; }
    QString getPhone() const { return phone; }

private:
    QString name;
    QString phone;
};

#endif // PERSON_H
