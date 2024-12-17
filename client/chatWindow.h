#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChatWindow : public QWidget {
    Q_OBJECT

public:
    ChatWindow(const QString& abonent1, const QString& abonent2, QWidget* parent = nullptr);
    ~ChatWindow();
private slots:
    void sendMessage();

private:
    QString abonent1;
    QString abonent2;
    QTextEdit* chatArea;
    QLineEdit* messageInput;
    QPushButton* sendButton;
};

#endif // CHATWINDOW_H
