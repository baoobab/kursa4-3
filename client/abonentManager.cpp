#include "abonentManager.h"
#include <QApplication>
#include <QDebug>
#include <QFrame>

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
    headerLabelCon = new QLabel("Список соединений (0)", containerCon);
    layoutCon->addWidget(headerLabelCon);
    horizontalLayout->addWidget(scrollAreaCon);

    // Center Block - ATS Information with button
    QFrame* atsInfoFrame = new QFrame(this);
    atsInfoFrame->setFrameShape(QFrame::StyledPanel);
    atsInfoFrame->setLineWidth(1);
    atsInfoFrame->setContentsMargins(5, 5, 5, 5);
    QVBoxLayout* atsInfoLayout = new QVBoxLayout(atsInfoFrame);
    atsInfoLayout->addWidget(new QLabel("Информация об АТС", atsInfoFrame));
    atsInfoLayout->addWidget(new QLabel("Версия АТС: 1.0", atsInfoFrame));
    atsInfoLayout->addWidget(new QLabel("Статус АТС: Работает", atsInfoFrame));
    createButton = new QPushButton("Создать абонента", this);
    atsInfoLayout->addWidget(createButton);
    connect(createButton, &QPushButton::clicked, this, &AbonentManager::showCreateDialog);
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

    QPushButton* deleteButton = new QPushButton("Удалить", abonentWidget);
    QPushButton* callButton = new QPushButton("Позвонить", abonentWidget);
    QPushButton* messageButton = new QPushButton("Отправить сообщение", abonentWidget);

    connect(deleteButton, &QPushButton::clicked, this, [this, abonentWidget]()
    {
        removeAbonent(abonentWidget);
    });

    connect(callButton, &QPushButton::clicked, this, [this, phone]()
    {
        initiateCall("YourPhoneNumber", phone); // Replace "YourPhoneNumber" with the actual caller's phone number
    });

   connect(messageButton, &QPushButton::clicked, this, [this, phone]()
   {
       sendMessage("YourPhoneNumber", phone, "Hello!"); // Replace "YourPhoneNumber" and message as needed
   });

   abonentLayout->addWidget(nameLabel);
   abonentLayout->addWidget(phoneLabel);
   abonentLayout->addWidget(callButton);
   abonentLayout->addWidget(messageButton);
   abonentLayout->addWidget(deleteButton);

   layout->addWidget(abonentWidget);

   abonentsWidget.push_back(abonentWidget);

   ats.addAbonent(name, phone, 2228); // Add abonent to ATS // TODO: 2228 - ZAGLUSHKA - SET UP WITH NETW
   updateHeader();
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

void AbonentManager::sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message)
{
   ats.sendMessage(fromPhone, toPhone, message);
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
