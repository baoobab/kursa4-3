#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../common/communicator.h"

class ChatWindow : public QWidget {
    Q_OBJECT
    TCommunicator* comm;

public:
    ChatWindow(const QString& abonent1, const QString& abonent2, QWidget* parent = nullptr);
    ~ChatWindow();
public slots:
    void fromCommunicator(QByteArray);
    void toCommunicator(QString);
private slots:
    void sendMessage();
signals:
    void request(QString);
    void received(QByteArray msg);
private:
    QString abonent1;
    QString abonent2;
    QTextEdit* chatArea;
    QLineEdit* messageInput;
    QPushButton* sendButton;
};

#endif // CHATWINDOW_H
