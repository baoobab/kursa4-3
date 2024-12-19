#include "application.h"
#include "person.h"

int main(int argc, char *argv[])
{

    TApplication a(argc, argv);

    // ATS ats;
    a.ats->setMaxCallsCount(52); // TODO: задавать степенью двойки

    // Обозначили собеседников
    Person* p1 = new Person("Vasya", "8800", a.ats); // TODO: номер фиксированный, либо отдельный тип, либо short phone[const size]
    Person* p2 = new Person("Kolya", "7952", a.ats); // TODO: добавить автоустановку портов

    // Внесли их контакты в базу АТС
    p1->enrollToATS();
    p2->enrollToATS();

    // ..смска
    QString message = "allah pomogi!";

    // Чел p1 звонит p2
    p1->makeCall(p2);

    // Чел p1 отправляет смску челу p2
    p1->sendMessage(p2, message);


    // Дальше можно раскомментарить, тоже код для примера

    // Очибка - нельзя отправить самому себе
    // p1->sendMessage(p1, message);

    // Чел p2 отправляет 2 смски челу p1
    // p2->sendMessage(p1, "from p2 to p1 first try");
    // p2->sendMessage(p1, "from p2 to p1 second try");

    return a.exec();
}
