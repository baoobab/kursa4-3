#ifndef CHATWINDOW_H
#define CHATWINDOW_H


#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChatWindow : public QDialog
{
    Q_OBJECT

public:

    ChatWindow(const QString& fromPhone, const QString& toPhone, QWidget* parent = nullptr);
    void appendMessage(const QString& message);


signals:
    void messageSent(const QString& fromPhone, const QString& toPhone, const QString& message); // Signal to send messages


private slots:
    void sendMessage();

public:
    QString fromPhone;
    QString toPhone;

private:

    QTextEdit* chatArea;
    QLineEdit* messageInput;
    QPushButton* sendButton;

};

#endif // CHATWINDOW_H
