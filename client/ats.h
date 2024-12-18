#ifndef ATS_H
#define ATS_H

#include "abonent.h"
#include <QMap>
#include <QDebug>
#include <QObject>
#include <QList>
// #include <QCoreApplication>
#include "../common/communicator.h"


class ATS : public QObject {
public:
    static const quint16 address = 10000;

    void addAbonent(const QString& name, const QString& phone, const quint16& address) {
        Abonent* abonent = new Abonent(name, phone, address);
        abonents.insert(abonent->getPhone(), abonent);
        qDebug() << "Added abonent:" << abonent->getPhone() << " with address:" << abonent->getAddress();
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

    // TODO: не забыть удаление коммуникаторов и строки из callRecords после завершения звонка
    void initiateCall(const QString& callerPhone, const QString& targetPhone) {
        if (maxCallsCount == callRecords.length()) {
            qDebug() << "Не осталось свободных соединений";
            return;
        }
        if (callerPhone == targetPhone) {
            qDebug() << "Нельзя позвонить самому себе";
            return;
        }

        Abonent* caller = getAbonent(callerPhone);
        Abonent* target = getAbonent(targetPhone);

        if (!caller || !target) {
            qDebug() << "Кого-то из собеседников нет в базе АТС";
            return;
        }

        if (caller->getStatus() == Abonent::ConnectionStatus::Ready &&
            target->getStatus() == Abonent::ConnectionStatus::Ready) {
            // caller->makeCall(target);
            // target->receiveCall(caller);

            qDebug() << "Call startded!";
            // TCommParams paramsCallerToTarget = { QHostAddress::LocalHost, target->getAddress(),
            //                                  QHostAddress::LocalHost, caller->getAddress() };
            // TCommParams paramsTargetToCaller = { QHostAddress::LocalHost, caller->getAddress(),
            //                                     QHostAddress::LocalHost, target->getAddress() };

            TCommParams paramsCallerToATS = { QHostAddress::LocalHost, ATS::address,
                                QHostAddress::LocalHost, caller->getAddress() };
            TCommParams paramsATSToCaller = { QHostAddress::LocalHost, caller->getAddress(),
                                        QHostAddress::LocalHost, ATS::address };

            TCommParams paramsTargetToATS = { QHostAddress::LocalHost, ATS::address,
                                        QHostAddress::LocalHost, target->getAddress() };
            TCommParams paramsATSToTarget = { QHostAddress::LocalHost, target->getAddress(),
                                        QHostAddress::LocalHost, ATS::address };

            TCommunicator* commCaller = new TCommunicator(paramsCallerToATS, this);
            TCommunicator* commTarget = new TCommunicator(paramsTargetToATS, this);

            TCommunicator* commATSCaller = new TCommunicator(paramsATSToCaller, this);
            TCommunicator* commATSTarget = new TCommunicator(paramsATSToTarget, this);

            connect(commATSCaller, SIGNAL(received(QByteArray)), this, SLOT(receive(QByteArray)));
            connect(commATSTarget, SIGNAL(received(QByteArray)), this, SLOT(receive(QByteArray)));

            // Store the call record
            callRecords.append(CallRecord(caller, commCaller, commATSCaller, target, commTarget, commATSTarget));
        } else {
            qDebug() << "One of the abonents is not ready for a call.";
        }
    }

    void sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message) {
        if (findCallRecord(fromPhone) == -1 || findCallRecord(toPhone) == -1) {
            qDebug() << "звонок не найден";
            return;
        }

        CallRecord currentCall = callRecords[findCallRecord(fromPhone)];
        // теперь нужно понять кто отправляет смс - тот кто начал звонок, или его собеседник
        Abonent* fromAbonent = nullptr;
        TCommunicator* fromComm = nullptr;
        Abonent* toAbonent = nullptr;

        if (currentCall.caller->getPhone() == fromPhone) {
            fromAbonent = currentCall.caller;
            fromComm = currentCall.commCallerToATS;
        } else if (currentCall.target->getPhone() == fromPhone) {
            fromAbonent = currentCall.target;
            fromComm = currentCall.commTargetToATS;
        }

        if (currentCall.caller->getPhone() == toPhone) {
            toAbonent = currentCall.caller;
        } else if (currentCall.target->getPhone() == toPhone) {
            toAbonent = currentCall.target;
        }

        if (!fromAbonent || !toAbonent || fromAbonent == toAbonent) {
            qDebug() << "Не все участники звонка были найдены, проверьте правильность данных и наличие звонка";
            return;
        }

        QString messageMetadata = fromAbonent->getPhone() + ";" + toAbonent->getPhone() + ';';
        QString readyToSend = messageMetadata + message;
        fromComm->send(QByteArray().append(readyToSend.toUtf8()));

    }

    // ATS(int argc, char *argv[])
    //     : QCoreApplication(argc, argv) {
    //     qDebug() << "ats started";
    // }

     // TODO: задавать степенью двойки
    void setMaxCallsCount(unsigned newCount) {
        maxCallsCount = newCount;
    }
signals:
    void messageReceived(QString from, QString to, QString message); // новый сигнал
public slots:

    void receive(QByteArray msg) {
        QString message = QString::fromUtf8(msg);

        QStringList parts = message.split(';');

        if (parts.size() == 3) {
            QString fromPhone = parts[0];
            QString toPhone = parts[1];
            QString textMessage = parts[2];

            qDebug() << "Отправитель:" << fromPhone << " Получатель:" << toPhone << " Сообщение:" << textMessage;

            if (findCallRecord(toPhone) == -1) {
                qDebug() << "Разговор не найден";
                return;
            }

            CallRecord currentCall = callRecords[findCallRecord(toPhone)];
            Abonent* toAbonent = nullptr;
            TCommunicator* toComm = nullptr;

            if (currentCall.caller->getPhone() == toPhone) {
                toAbonent = currentCall.caller;
                toComm = currentCall.commATSToCaller;
            } else if (currentCall.target->getPhone() == toPhone) {
                toAbonent = currentCall.target;
                toComm = currentCall.commATSToTarget;
            }

            if (!toAbonent) {
                qDebug() << "Получатель не найден";
                return;
            }
            // toComm->send(QByteArray().append(textMessage.toUtf8()));
            emit messageReceived(fromPhone, toPhone, textMessage); // излучение сигнала вместо реальной отправки
            qDebug() << "Сообщение было передано на " << toAbonent->getAddress();
        } else {
            qDebug() << "Неверный формат сообщения!";
        }

    }

private:
    Q_OBJECT

    struct CallRecord {
        Abonent* caller;
        TCommunicator* commCallerToATS;
        TCommunicator* commATSToCaller;
        Abonent* target;
        TCommunicator* commTargetToATS;
        TCommunicator* commATSToTarget;


        CallRecord(Abonent* caller, TCommunicator* commCallerToATS, TCommunicator* commATSToCaller,
                   Abonent* target, TCommunicator* commTargetToATS, TCommunicator* commATSToTarget)
            : caller(caller), commCallerToATS(commCallerToATS), commATSToCaller(commATSToCaller),
            target(target), commTargetToATS(commTargetToATS), commATSToTarget(commATSToTarget) {}
    };

    QMap<QString, Abonent*> abonents; // Key: Phone number, Value: Abonent object
    QList<CallRecord> callRecords;     // List of active calls
    unsigned maxCallsCount; // Макс. число соединений (мб степенью двойки сделать)
    quint16 lastAbonentAddress = 10001; // Последний выданный порт для абонента

    int findCallRecord(const QString& phone) {
        for (int i = 0; i < callRecords.length(); i++) {
            auto record = callRecords[i];
            if (record.caller->getPhone() == phone || record.target->getPhone() == phone) {
                return i;
            }
        }

        return -1;
    }

};

// Definition of the static member
// QMap<QString, Abonent*> ATS::abonents;


// TODO: статик конструктор по синглтону

#endif // ATS_H
