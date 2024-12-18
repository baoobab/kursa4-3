#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

struct TCommParams
{
    QHostAddress rHost;
    quint16      rPort;
    QHostAddress sHost;
    quint16      sPort;
};

class TCommunicator : public QUdpSocket
{
    Q_OBJECT

    bool         ready;
    TCommParams  params;

public:
    TCommunicator(TCommParams&,QObject *parent = nullptr);

    bool isReady();

signals:
    void received(QByteArray);

public slots:
    void send(QByteArray);

private slots:
    void receive();

};


// с отдельными сокетами (тож не работает)
// #ifndef COMMUNICATOR_H
// #define COMMUNICATOR_H

// #include <QObject>
// #include <QUdpSocket>
// #include <QHostAddress>

// struct TCommParams {
//     QHostAddress rHost;
//     quint16      rPort;
//     QHostAddress sHost;
//     quint16      sPort;
// };

// class TCommunicator : public QObject {
//     Q_OBJECT

// public:
//     TCommunicator(TCommParams&, QObject* parent = nullptr);
//     ~TCommunicator();
//     bool isReady();

// signals:
//     void received(QByteArray);

// public slots:
//     void send(QByteArray);
// private slots:
//     void receive();
// private:
//     QUdpSocket* socket_;
//     TCommParams params_;
//     bool ready_;
// };
// с отдельными сокетами (тож не работает)


#endif // COMMUNICATOR_H

