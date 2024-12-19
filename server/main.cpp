#include "ats.h"
#include "person.h"
#include "application.h"


int main(int argc, char *argv[])
{

    TApplication a(argc, argv);

    a.ats->setMaxCallsCount(52); // TODO: задавать степенью двойки

    // Обозначили собеседников
    Person* p1 = new Person("Vasya", "8800", a.ats); // TODO: номер фиксированный, либо отдельный тип, либо short phone[const size]
    Person* p2 = new Person("Kolya", "7952", a.ats); // TODO: добавить автоустановку портов

    // Внесли их контакты в базу АТС
    p1->pickUpPhone();
    p2->pickUpPhone();

    // ..смска
    QString message = "allah pomogi!";

    // Чел p1 звонит p2, но p2 положил трубу
    p2->hangUpPhone();
    p1->makeCall(p2);

    // Чел p1 звонит p2, тепрь все норм
    p2->pickUpPhone();
    p1->makeCall(p2);


    // Чел p1 отправляет смску челу p2
    p1->sendMessage(p2, message);


    // Дальше можно раскомментарить, тоже код для примера

    // Очибка - нельзя отправить самому себе
    // p1->sendMessage(p1, message);

    // Чел p2 ложит трубку
    p2->endCall();
    qDebug() << (int)a.ats->getAbonentStatus(p1->getPhone()) << " " << (int)a.ats->getAbonentStatus(p2->getPhone());

    // Чел p2 отправляет 2 смски челу p1 но звонок уже завершен, в ЧС кинул всё капут
    p2->sendMessage(p1, "from p2 to p1 first try");

    // p2 так-то яростно повесил трубу, и пытается позвонить, не подняв её - ошибка будет
    p2->makeCall(p1);

    // Вот теперь все будет ок
    p2->pickUpPhone();
    p2->makeCall(p1);

    return a.exec();
}
