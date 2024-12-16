#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QFormLayout>
#include <QTextEdit>

class ChatWindow : public QWidget {
public:
    ChatWindow(const QString& abonent1, const QString& abonent2, QWidget* parent = nullptr)
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
    }

    void sendMessage() {
        QString message = messageInput->text();
        if (!message.isEmpty()) {
            QString msg;
            msg += "Коля (Вы)";
            msg += " [19-10-24 20:30:09]: ";
            msg += message;
            chatArea->append(msg);
            messageInput->clear();
            // Отправка сообщения другому абоненту (здесь симулируем)
            QString msg2;
            msg2 += "Саня";
            msg2 += " [19-10-24 20:32:12]: ";
            msg2 += " я на паре";
            chatArea->append(msg2);
        }
    }

private:
    QString abonent1;
    QString abonent2;
    QTextEdit* chatArea;
    QLineEdit* messageInput;
    QPushButton* sendButton;
};

class AbonentWidget : public QWidget {
public:
    AbonentWidget(const QString& name, const QString& phone, const QString& status, QWidget* parent = nullptr)
        : QWidget(parent), name(name), status(status) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* topLayout = new QHBoxLayout();
        nameLabel = new QLabel(name, this);
        phoneLabel = new QLabel(phone, this);
        statusLabel = new QLabel(status, this);

        if (status == "Готов") statusLabel->setStyleSheet("color: red;");
        if (status == "Разговор") statusLabel->setStyleSheet("color: green;");
        if (status == "Вызов") statusLabel->setStyleSheet("color: orange;");
        if (status == "Свободен") statusLabel->setStyleSheet("color: white;");

        callButton = new QPushButton("Позвонить", this);
        connect(callButton, &QPushButton::clicked, this, &AbonentWidget::makeCall);
        deleteButton = new QPushButton("Удалить", this);
        // connect(deleteButton, &QPushButton::clicked, this, &AbonentWidget::deleteAbonent);
        topLayout->addWidget(nameLabel);
        topLayout->addWidget(phoneLabel);
        topLayout->addWidget(statusLabel);
        topLayout->addWidget(callButton);
        topLayout->addWidget(deleteButton);
        layout->addLayout(topLayout);
    }

    void makeCall() {
        // Симулируем вызов другого абонента
        QString targetAbonent = "8 970 323 10 20";
        // Создание нового окна чата
        QString s = name;
        s += " => 8 970 323 10 20";
        ChatWindow* chatWindow = new ChatWindow(name, targetAbonent);
        chatWindow->setWindowTitle(s);
        chatWindow->show();
    }

    // void deleteAbonent() {
    //     // Emit сигнал о удалении абонента
    //     emit abonentDeleted(this);
    // }

    // signals:
    //     void abonentDeleted(AbonentWidget* abonent);

private:
    QString name;
    QString phone;
    QString status;
    QLabel* nameLabel;
    QLabel* phoneLabel;
    QLabel* statusLabel;
    QPushButton* callButton;
    QPushButton* deleteButton;
};

class CreateAbonentDialog : public QDialog {
public:
    CreateAbonentDialog(QWidget* parent = nullptr) : QDialog(parent) {
        QFormLayout* layout = new QFormLayout(this);
        setWindowTitle("Добавление абонента");
        nameInput = new QLineEdit(this);
        phoneInput = new QLineEdit(this);
        statusInput = new QLineEdit(this);
        layout->addRow("Имя:", nameInput);
        layout->addRow("Телефон:", phoneInput);
        layout->addRow("Статус:", statusInput);

        QPushButton* createButton = new QPushButton("Создать", this);
        connect(createButton, &QPushButton::clicked, this, &CreateAbonentDialog::createAbonent);
        layout->addWidget(createButton);
    }

    void createAbonent() {
        QString name = nameInput->text();
        // TODO: phone
        QString status = statusInput->text();
        if (!name.isEmpty() &&!status.isEmpty()) {
            // emit abonentCreated(name, status);
            accept();
        }
    }

    // signals:
    //     void abonentCreated(const QString& name, const QString& status);

private:
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* statusInput;
};

class AbonentManager : public QWidget {
public:
    AbonentManager(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        // Horizontal layout for three sections
        QHBoxLayout* horizontalLayout = new QHBoxLayout();

        // Left Block - List of Connections
        QScrollArea* scrollAreaCon = new QScrollArea(this);
        scrollAreaCon->setWidgetResizable(true);

        QWidget* containerCon = new QWidget();
        scrollAreaCon->setWidget(containerCon);

        layoutCon = new QVBoxLayout(containerCon);
        headerLabelCon = new QLabel("Список соединений (0)", containerCon);
        layoutCon->addWidget(headerLabelCon);

        horizontalLayout->addWidget(scrollAreaCon); // Add connections block to horizontal layout

        // Center Block - ATS Information with button
        QFrame* atsInfoFrame = new QFrame(this);
        atsInfoFrame->setFrameShape(QFrame::StyledPanel); // Set frame style
        atsInfoFrame->setLineWidth(1); // Set line width
        atsInfoFrame->setContentsMargins(5, 5, 5, 5); // Set margins

        QVBoxLayout* atsInfoLayout = new QVBoxLayout(atsInfoFrame);

        atsInfoLayout->addWidget(new QLabel("Информация об АТС", atsInfoFrame));
        atsInfoLayout->addWidget(new QLabel("Версия АТС: 1.0", atsInfoFrame));
        atsInfoLayout->addWidget(new QLabel("Статус АТС: Работает", atsInfoFrame));

        createButton = new QPushButton("Создать абонента", this);
        atsInfoLayout->addWidget(createButton); // Move the button into the ATS info block

        connect(createButton, &QPushButton::clicked, this, &AbonentManager::showCreateDialog);

        horizontalLayout->addWidget(atsInfoFrame); // Add ATS info block to horizontal layout

        // Right Block - List of Abonents
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);

        QWidget* container = new QWidget();
        scrollArea->setWidget(container);

        layout = new QVBoxLayout(container);
        headerLabel = new QLabel("Список абонентов (0)", container);
        layout->addWidget(headerLabel);

        horizontalLayout->addWidget(scrollArea); // Add abonents block to horizontal layout

        mainLayout->addLayout(horizontalLayout); // Add the complete horizontal layout to main layout

        QString statusArr[4] = {"Готов", "Разговор", "Вызов", "Свободен"};
        QString phoneArr[6] = {"Вася", "Коля", "Виталий", "Саня", "Аноним", "ski tiger"};
        QString numsArr[8] = {"8 980 123 53 21", "8 972 142 32 21", "8 980 424 11 22", "8 924 332 13 78",
                              "8 927 421 32 55", "8 970 768 05 30", "8 980 123 53 21", "8 980 123 53 21"};

        // Adding abonents to the list
        for (int i = 0; i < 20; ++i) {
            QString name= numsArr[i % 8];
            QString phone= phoneArr[i % 6];
            QString status= statusArr[i % 4];

            AbonentWidget *abonentWidget= new AbonentWidget(name ,phone ,status );

            layout -> addWidget(abonentWidget );
            abonents.push_back(abonentWidget );
        }

        updateHeader();

        // Set maximum width for the abonents section
        scrollArea -> setMaximumWidth(400 );
    }

    void showCreateDialog() {
        CreateAbonentDialog dialog(this);
        dialog.exec();
    }

    void addAbonent(const QString& name ,const QString& phone ,const QString& status ) {
        AbonentWidget *abonentWidget= new AbonentWidget(name ,phone ,status );
        layout -> addWidget(abonentWidget );
        abonents.push_back(abonentWidget );
        updateHeader();
    }

    void removeAbonent(AbonentWidget* abonent) {
        layout -> removeWidget(abonent);
        abonent -> deleteLater();
        abonents.erase(std::remove(abonents.begin(), abonents.end(), abonent), abonents.end());
        updateHeader();
    }

    void updateHeader() {
        headerLabel -> setText("Список абонентов (" + QString::number(abonents.size()) + ")");
    }

private:
    QLabel *headerLabel;
    QLabel *headerLabelCon;
    QPushButton *createButton;
    QVBoxLayout *layout;
    QVBoxLayout *layoutCon;
    std::vector<AbonentWidget *> abonents;
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    AbonentManager window;
    window.setWindowTitle("Мини-АТС");
    window.show();

    return app.exec();
}
