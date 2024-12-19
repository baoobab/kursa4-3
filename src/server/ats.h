#ifndef ATS_H
#define ATS_H

#include "abonent.h"
#include <QMap>
#include <QDebug>
#include <QObject>
#include <QList>
#include "../common/communicator.h"


// Структура ответа от АТС
struct ATSMessage {
    bool isSuccess;
    QString message;

    ATSMessage() : isSuccess(true), message("OK") {}
    ATSMessage(const QString& msg) : isSuccess(false), message(msg) {}

    operator bool() const { return isSuccess; }
};

class ATS : public QObject {
    Q_OBJECT

public:
    static const quint16 address = 10000; // адрес АТС

    QList<Abonent*> getAllAbonents();
    int getCurrentConnections();
    int getMaxConnections();

    ATSMessage addAbonent(const QString& name, const QString& phone);
    ATSMessage removeAbonent(const QString& phone);
    Abonent* getAbonent(const QString& phone);
    Abonent::ConnectionStatus getAbonentStatus(const QString& phone);
    QString getAbonentStatusString (const QString& phone);

    ATSMessage initiateCall(const QString& callerPhone, const QString& targetPhone);
    ATSMessage endCall(const QString& phone);
    void sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message);

    void setMaxCallsCount(unsigned newCount);

signals:
    void messageReceived(QString from, QString to, QString message);
public slots:
    void receive(QByteArray msg);

private:
    struct CallRecord {
        Abonent* caller;
        TCommunicator* commCallerToATS;
        TCommunicator* commATSToCaller;
        Abonent* target;
        TCommunicator* commTargetToATS;
        TCommunicator* commATSToTarget;

        CallRecord(Abonent* caller, TCommunicator* commCallerToATS, TCommunicator* commATSToCaller,
                   Abonent* target, TCommunicator* commTargetToATS, TCommunicator* commATSToTarget)
            : caller(caller), commCallerToATS(std::move(commCallerToATS)), commATSToCaller(std::move(commATSToCaller)),
            target(target), commTargetToATS(std::move(commTargetToATS)), commATSToTarget(std::move(commATSToTarget)) {}
    };


    QMap<QString, Abonent*> abonents;
    QList<CallRecord> callRecords;
    unsigned maxCallsCount = 0;

    int findCallRecord(const QString& phone);
};

#endif // ATS_H
