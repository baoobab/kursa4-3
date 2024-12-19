#include "abonentManager.h"
#include <QApplication>
#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QtDebug>

AbonentManager::AbonentManager(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    // Left Block - List of Connections
    QScrollArea* scrollAreaCon = new QScrollArea(this);
    scrollAreaCon->setWidgetResizable(true);
    QWidget* containerCon = new QWidget();
    scrollAreaCon->setWidget(containerCon);
    layoutCon = new QVBoxLayout(containerCon);
    // Add buttons for increasing and decreasing connections
    increaseConnectionsButton = new QPushButton("Макс. Кол-во соединений +1", this);
    decreaseConnectionsButton = new QPushButton("Макс. Кол-во соединений -1", this);
    connect(increaseConnectionsButton, &QPushButton::clicked, this, &AbonentManager::increaseMaxConnections);
    connect(decreaseConnectionsButton, &QPushButton::clicked, this, &AbonentManager::decreaseMaxConnections);
    layoutCon->addWidget(increaseConnectionsButton);
    layoutCon->addWidget(decreaseConnectionsButton);

    // Create the "Создать соединение" button
    createConnectionButton = new QPushButton("Создать соединение", this);
    connect(createConnectionButton, &QPushButton::clicked, this, &AbonentManager::showCreateConnectionDialog);
    layoutCon->addWidget(createConnectionButton); // Add the button to the left block

    createButton = new QPushButton("Создать абонента", this);
    layoutCon->addWidget(createButton);
    connect(createButton, &QPushButton::clicked, this, &AbonentManager::showCreateDialog);
    horizontalLayout->addWidget(scrollAreaCon);

    // Center Block - ATS Information with button
    QFrame* atsInfoFrame = new QFrame(this);
    atsInfoFrame->setFrameShape(QFrame::StyledPanel);
    atsInfoFrame->setLineWidth(1);
    atsInfoFrame->setContentsMargins(5, 5, 5, 5);
    QVBoxLayout* atsInfoLayout = new QVBoxLayout(atsInfoFrame);
    atsInfoLayout->addWidget(new QLabel("Информация об АТС", atsInfoFrame));

    // Add new label for maximum connections
    maxConnectionsLabel = new QLabel("Максимальное количество соединений: 0", atsInfoFrame);
    atsInfoLayout->addWidget(maxConnectionsLabel);

    headerLabelCon = new QLabel("Список соединений (0)", atsInfoFrame);
    atsInfoLayout->addWidget(headerLabelCon);

    atsInfoLayout->addWidget(new QLabel("Версия АТС: 1.0", atsInfoFrame));
    atsInfoLayout->addWidget(new QLabel("Статус АТС: Работает", atsInfoFrame));

    horizontalLayout->addWidget(atsInfoFrame);

    // Right Block - List of Abonents
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget* container = new QWidget();
    scrollArea->setWidget(container);
    layout = new QVBoxLayout(container);
    headerLabel = new QLabel("Список абонентов (0)", container);
    layout->addWidget(headerLabel);
    horizontalLayout->addWidget(scrollArea);

    mainLayout->addLayout(horizontalLayout);

    // Draw table of abonents
    drawAbonentsTable();

    connect(&ats, &ATS::messageReceived, this, [this](QString from, QString to, QString message)
    {

        // Find the corresponding chat window and append the message

        // You may need to maintain a list of chat windows to find the correct one

        for (auto chatWindow : chatWindows)
        {
            // Assuming you maintain a list of chat windows

            if ((chatWindow->fromPhone == from && chatWindow->toPhone == to) ||

                (chatWindow->fromPhone == to && chatWindow->toPhone == from))
            {

                chatWindow->appendMessage(from + ": " + message);

                break;
            }
        }
    });
}

// Implement the removeSelectedAbonent slot
void AbonentManager::removeSelectedAbonent()
{

}


// Implement the showCreateConnectionDialog slot
void AbonentManager::showCreateConnectionDialog()
{
    QDialog* createConnectionDialog = new QDialog(this);
    createConnectionDialog->setWindowTitle("Создать соединение");
    QVBoxLayout* dialogLayout = new QVBoxLayout(createConnectionDialog);


    // Create combo boxes for selecting abonents
    QComboBox* abonent1ComboBox = new QComboBox(createConnectionDialog);
    QComboBox* abonent2ComboBox = new QComboBox(createConnectionDialog);


    // Populate the combo boxes with abonents
    QList<Abonent*> abonentsList = ats.getAllAbonents(); // Use the new method
    for (Abonent* abonent : abonentsList)
    {
        abonent1ComboBox->addItem(abonent->getName(), abonent->getPhone());
        abonent2ComboBox->addItem(abonent->getName(), abonent->getPhone());
    }


    dialogLayout->addWidget(new QLabel("Выберите первого абонента:", this));
    dialogLayout->addWidget(abonent1ComboBox);
    dialogLayout->addWidget(new QLabel("Выберите второго абонента:", this));
    dialogLayout->addWidget(abonent2ComboBox);


    // Create buttons for OK and Cancel
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, createConnectionDialog);

    connect(buttonBox, &QDialogButtonBox::accepted, createConnectionDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, createConnectionDialog, &QDialog::reject);
    dialogLayout->addWidget(buttonBox);

    if (currentConnections < maxConnections && createConnectionDialog->exec() == QDialog::Accepted)
    {
        currentConnections++;

        QString abonent1Phone = abonent1ComboBox->currentData().toString();

        QString abonent2Phone = abonent2ComboBox->currentData().toString();

        // Logic to create the connection
        initiateCall(abonent1Phone, abonent2Phone);

        ChatWindow* chatWindowA = new ChatWindow(abonent1Phone, abonent2Phone, this);
        ChatWindow* chatWindowB = new ChatWindow(abonent2Phone, abonent1Phone, this);

        // Connect signals to send messages through ATS
        connect(chatWindowA, &ChatWindow::messageSent, this, &AbonentManager::sendMessage);
        connect(chatWindowB, &ChatWindow::messageSent, this, &AbonentManager::sendMessage);

        // Add chat windows to the list
        chatWindows.append(chatWindowA);
        chatWindows.append(chatWindowB);

        // Show both chat windows
        chatWindowA->show();
        chatWindowB->show();
    }
    // Clean up
    delete createConnectionDialog;
}

void AbonentManager::sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message) {

    ats.sendMessage(fromPhone, toPhone, message);

}

void AbonentManager::showCreateDialog()
{
    createAbonentDialog = new QDialog(this);
    QFormLayout* layout = new QFormLayout(createAbonentDialog);
    createAbonentDialog->setWindowTitle("Добавление абонента");

    nameInput = new QLineEdit(createAbonentDialog);
    phoneInput = new QLineEdit(createAbonentDialog);

    layout->addRow("Имя:", nameInput);
    layout->addRow("Телефон:", phoneInput);

    QPushButton* createButton = new QPushButton("Создать", createAbonentDialog);

    connect(createButton, &QPushButton::clicked, this, [this]()
    {
        QString name = nameInput->text();
        QString phone = phoneInput->text();
        if (!name.isEmpty() && !phone.isEmpty())
        {
            addAbonent(name, phone);
            createAbonentDialog->accept();
        }
    });

    layout->addWidget(createButton);
    createAbonentDialog->exec();
}

void AbonentManager::addAbonent(const QString& name, const QString& phone)
{
    QWidget* abonentWidget = new QWidget(this);
    QHBoxLayout* abonentLayout = new QHBoxLayout(abonentWidget);
    QLabel* nameLabel = new QLabel(name, abonentWidget);
    QLabel* phoneLabel = new QLabel(phone, abonentWidget);

    // Add a status label
    QLabel* statusLabel = new QLabel("Готов", abonentWidget); // You can change the status as needed
//    QPushButton* deleteButton = new QPushButton("Удалить", abonentWidget);

//    connect(deleteButton, &QPushButton::clicked, this, [this, abonentWidget]()
//    {
//        removeAbonent(abonentWidget);
//    });

    // Only add the delete button and status label
    abonentLayout->addWidget(nameLabel);
    abonentLayout->addWidget(phoneLabel);
    abonentLayout->addWidget(statusLabel); // Add the status label
//    abonentLayout->addWidget(deleteButton); // Add the delete button

    layout->addWidget(abonentWidget);


    abonentsWidget.push_back(abonentWidget);

    ats.addAbonent(name, phone, 2228); // Add abonent to ATS // TODO: 2228 - ZAGLUSHKA - SET UP WITH NETW
    updateHeader();

//    connect(callButton, &QPushButton::clicked, this, [this, phone]()
//    {
//        initiateCall("YourPhoneNumber", phone); // Replace "YourPhoneNumber" with the actual caller's phone number
//    });

//   connect(messageButton, &QPushButton::clicked, this, [this, phone]()
//   {
//       sendMessage("YourPhoneNumber", phone, "Hello!"); // Replace "YourPhoneNumber" and message as needed
//   });


}

void AbonentManager::removeAbonent(QWidget* abonent)
{
   layout->removeWidget(abonent);
   abonent->deleteLater();
   abonentsWidget.erase(std::remove(abonentsWidget.begin(), abonentsWidget.end(), abonent), abonentsWidget.end());
   updateHeader();
}

void AbonentManager::updateHeader()
{
   headerLabel->setText("Список абонентов (" + QString::number(abonentsWidget.size()) + ")");
}

void AbonentManager::drawAbonentsTable()
{
   // Sample data for abonents
   addAbonent("Вася", "8 980 123 53 21");
   addAbonent("Коля", "8 972 142 32 21");
   addAbonent("Виталий", "8 980 424 11 22");
   addAbonent("Саня", "8 924 332 13 78");
   addAbonent("Аноним", "8 927 421 32 55");
   addAbonent("ski tiger", "8 970 768 05 30");
}

void AbonentManager::initiateCall(const QString& callerPhone, const QString& targetPhone)
{
   ats.initiateCall(callerPhone, targetPhone);
}


void AbonentManager::increaseMaxConnections()
{
    maxConnections++;
    maxConnectionsLabel->setText(QString("Максимальное количество соединений: %1").arg(maxConnections));
}


void AbonentManager::decreaseMaxConnections()
{
    if (maxConnections > 0)
    {
        maxConnections--;
        maxConnectionsLabel->setText(QString("Максимальное количество соединений: %1").arg(maxConnections));
    }
}


AbonentManager::~AbonentManager()
{
   qDeleteAll(abonentsWidget);
   delete headerLabel;
   delete headerLabelCon;
   delete createButton;
   delete layout;
   delete layoutCon;
   delete createAbonentDialog;
}
