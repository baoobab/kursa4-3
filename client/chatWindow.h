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
    void callEnded(const QString& fromPhone, const QString& toPhone); // Signal to end the call

private slots:
    void sendMessage();
    void endCall(); // Slot to handle ending the call
public:
    QString fromPhone;
    QString toPhone;

private:

    QTextEdit* chatArea;
    QLineEdit* messageInput;
    QPushButton* sendButton;
    QPushButton* hangUpButton; // Button to end the call
};

#endif // CHATWINDOW_H
