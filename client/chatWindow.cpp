#include "chatWindow.h"

ChatWindow::ChatWindow(const QString& abonent1, const QString& abonent2, QWidget* parent)
    : QWidget(parent), abonent1(abonent1), abonent2(abonent2) {

    resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    chatArea = new QTextEdit(this);
    chatArea->setReadOnly(true);

    layout->addWidget(chatArea);

    QHBoxLayout* inputLayout = new QHBoxLayout();

    messageInput = new QLineEdit(this);

    inputLayout->addWidget(messageInput);

    sendButton = new QPushButton("Отправить", this);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);

    inputLayout->addWidget(sendButton);

    layout->addLayout(inputLayout);

    TCommParams params = { QHostAddress::LocalHost, 10001,
                          QHostAddress::LocalHost, 9000 };
    comm = new TCommunicator(params, this);

    connect(comm,SIGNAL(received(QByteArray)),this,
            SLOT(fromCommunicator(QByteArray)));

    connect(this,SIGNAL(request(QString)),
            this,SLOT(toCommunicator(QString)));
}

void ChatWindow::fromCommunicator(QByteArray msg)
{
    // interface->answer(QString(msg));
    qDebug() << "fromCom CHATWINDOW" << QString(msg);
}

void ChatWindow::toCommunicator(QString msg)
{
    comm->send(QByteArray().append(msg.toUtf8()));
}


void ChatWindow::sendMessage() {
    QString message = messageInput->text();

    if (!message.isEmpty()) {
        QString msg = "Коля (Вы) [19-10-24 20:30:09]: " + message;
        chatArea->append(msg);
        messageInput->clear();

        // Simulated response from another user
        QString msg2 = "Саня [19-10-24 20:32:12]: я на паре";
        chatArea->append(msg2);
    }
}

ChatWindow::~ChatWindow()
{

    delete chatArea;
    delete messageInput;
    delete sendButton;
}
