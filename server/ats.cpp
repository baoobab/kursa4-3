#ifndef ATS_CPP
#define ATS_CPP

#include "abonent.h"
#include "ats.h"
#include <QMap>
#include <QDebug>
#include <QObject>
#include <QList>
#include "../common/communicator.h"

ATSMessage ATS::addAbonent(const QString& name, const QString& phone) {
    auto abonentsCountBefore = abonents.size();

    Abonent* abonent = new Abonent(name, phone);
    abonents.insert(abonent->getPhone(), abonent);

    if (abonents.size() == abonentsCountBefore) {
        return ATSMessage("Abonent already exists"); // Возврат ошибки
    }
    if (maxCallsCount == callRecords.length()) {
        qDebug() << "Free connections not found";
        return ATSMessage("Free connections not found");
    }

    qDebug() << "Added abonent:" << abonent->getPhone() << " with address:" << abonent->getAddress();

    return ATSMessage(); // Сообщение без параметров - означает успешное выполнение

}

ATSMessage ATS::removeAbonent(const QString& phone) {
    if (abonents.remove(phone) > 0) {
        qDebug() << "Removed abonent with phone:" << phone;
        return ATSMessage();
    } else {
        qDebug() << "Abonent not found with phone:" << phone;
        return ATSMessage("Abonent not found");
    }
}

Abonent* ATS::getAbonent(const QString& phone) {
    return abonents.contains(phone) ? abonents[phone] : nullptr;
}

ATSMessage ATS::initiateCall(const QString& callerPhone, const QString& targetPhone) {
    if (maxCallsCount == callRecords.length()) {
        qDebug() << "Free connections not found";
        return ATSMessage("Free connections not found");
    }
    if (callerPhone == targetPhone) {
        qDebug() << "Can not call to yourself";
        return ATSMessage("Can not call to yourself");
    }
    if (findCallRecord(callerPhone) != -1 || findCallRecord(targetPhone) != -1) {
        qDebug("One of the abonents already have an active call");
        return ATSMessage("One of the abonents already have an active call");
    }

    Abonent* caller = getAbonent(callerPhone);
    Abonent* target = getAbonent(targetPhone);

    if (!caller || !target) {
        qDebug() << "Caller or target not enrolled to ATS DB";
        return ATSMessage("Caller or target not enrolled to ATS DB");
    }

    if (caller->getStatus() == Abonent::ConnectionStatus::Ready &&
        target->getStatus() == Abonent::ConnectionStatus::Ready) {

        bool isCallerCalled = caller->makeCall();
        if (!isCallerCalled) {
            return ATSMessage("Caller can not make the call, check limits");
        }
        bool isTargetCalled = target->receiveCall();
        if (!isTargetCalled) {
            return ATSMessage("Target can not receive the call, check limits");
        }

        qDebug() << "Call startded!";
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

        // Добавляем запись об активном звонке
        callRecords.append(CallRecord(caller, commCaller, commATSCaller, target, commTarget, commATSTarget));

        return ATSMessage();
    } else {
        qDebug() << "One of the abonents is not ready for a call.";
        return ATSMessage("One of the abonents is not ready for a call");
    }
}

ATSMessage ATS::endCall(const QString& phone) {

    if (findCallRecord(phone) == -1) {
        qDebug() << "Call record not found";
        return ATSMessage("Call record not found");
    }

    int callRecordIndex = findCallRecord(phone);
    CallRecord& currentCall = callRecords[callRecordIndex];

    // Изменение состояния абонентов
    bool callerEnded = currentCall.caller->endCall();
    bool targetEnded = currentCall.target->endCall();

    if (!callerEnded || !targetEnded) {
        qDebug() << "Failed to end call for one or both abonents";
        return ATSMessage("Failed to end call");
    }
    abonents.remove(currentCall.caller->getPhone());
    abonents.remove(currentCall.target->getPhone());

    // Удаление коммуникаторов
    delete currentCall.commCallerToATS;
    delete currentCall.commTargetToATS;
    delete currentCall.commATSToCaller;
    delete currentCall.commATSToTarget;
    currentCall.commCallerToATS = nullptr;
    currentCall.commTargetToATS = nullptr;
    currentCall.commATSToCaller = nullptr;
    currentCall.commATSToTarget = nullptr;

    // Удаление записи о звонке
    callRecords.removeAt(callRecordIndex);

    qDebug() << "Call ended successfully for" << phone;
    return ATSMessage();
}

void ATS::sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message) {
    if (findCallRecord(fromPhone) == -1 || findCallRecord(toPhone) == -1) {
        qDebug() << "Call not found";
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
        qDebug() << "Caller or target not found, check call data";
        return;
    }

    QString messageMetadata = fromAbonent->getPhone() + ";" + toAbonent->getPhone() + ';';
    QString readyToSend = messageMetadata + message;
    fromComm->send(QByteArray().append(readyToSend.toUtf8()));

}

// TODO: задавать степенью двойки
void ATS::setMaxCallsCount(unsigned newCount) {
    maxCallsCount = newCount;
}

void ATS::receive(QByteArray msg) {
    QString message = QString::fromUtf8(msg);

    QStringList parts = message.split(';');

    if (parts.size() == 3) {
        QString fromPhone = parts[0];
        QString toPhone = parts[1];
        QString textMessage = parts[2];

        qDebug() << "Sender:" << fromPhone << " reciever:" << toPhone << " text:" << textMessage;

        if (findCallRecord(toPhone) == -1) {
            qDebug() << "Call not found";
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
            qDebug() << "Reciever not found";
            return;
        }
        // toComm->send(QByteArray().append(textMessage.toUtf8()));
        emit messageReceived(fromPhone, toPhone, textMessage);
        qDebug() << "Message sent to " << toAbonent->getAddress();
    } else {
        qDebug() << "Bad messaage format!";
    }

}

int ATS::findCallRecord(const QString& phone) {
    for (int i = 0; i < callRecords.length(); i++) {
        auto record = callRecords[i];
        if (record.caller->getPhone() == phone || record.target->getPhone() == phone) {
            return i;
        }
    }

    return -1;
}

#endif // ATS_CPP
