#include "chatWindow.h"

ChatWindow::ChatWindow(const QString& fromPhone, const QString& toPhone, QWidget* parent)

    : QDialog(parent), fromPhone(fromPhone), toPhone(toPhone) {

    setWindowTitle("Chat between " + fromPhone + " and " + toPhone);
    resize(400, 300);
    QVBoxLayout* layout = new QVBoxLayout(this);

    chatArea = new QTextEdit(this);
    chatArea->setReadOnly(true);

    layout->addWidget(chatArea);

    QHBoxLayout* inputLayout = new QHBoxLayout();
    messageInput = new QLineEdit(this);
    inputLayout->addWidget(messageInput);

    sendButton = new QPushButton("Send", this);
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    inputLayout->addWidget(sendButton);

    layout->addLayout(inputLayout);
}


void ChatWindow::sendMessage()
{
    QString message = messageInput->text();
    if (!message.isEmpty())
    {
        appendMessage("You: " + message);
        messageInput->clear();

        // Emit a signal to send the message to ATS
        emit messageSent(fromPhone, toPhone, message);
    }

}


void ChatWindow::appendMessage(const QString& message)
{
    chatArea->append(message);
}



//ChatWindow::~ChatWindow()
//{

//    delete chatArea;
//    delete messageInput;
//    delete sendButton;
//}
