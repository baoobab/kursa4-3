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
    increaseConnectionsButton = new QPushButton("Макс. Кол-во соединений +1 ✌️", this);
    decreaseConnectionsButton = new QPushButton("Макс. Кол-во соединений -1 ☝️", this);
    connect(increaseConnectionsButton, &QPushButton::clicked, this, &AbonentManager::increaseMaxConnections);
    connect(decreaseConnectionsButton, &QPushButton::clicked, this, &AbonentManager::decreaseMaxConnections);
    layoutCon->addWidget(increaseConnectionsButton);
    layoutCon->addWidget(decreaseConnectionsButton);

    // Create the "Создать соединение" button
    createConnectionButton = new QPushButton("Создать соединение️ ☡", this);
    connect(createConnectionButton, &QPushButton::clicked, this, &AbonentManager::showCreateConnectionDialog);
    layoutCon->addWidget(createConnectionButton); // Add the button to the left block

    createButton = new QPushButton("Создать абонента ✍️", this);
    layoutCon->addWidget(createButton);
    connect(createButton, &QPushButton::clicked, this, &AbonentManager::showCreateDialog);

    pickUpButton = new QPushButton("Снять трубку ☎️ ", this);
    layoutCon->addWidget(pickUpButton); // Assuming layoutCon is your layout for the left block
    connect(pickUpButton, &QPushButton::clicked, this, &AbonentManager::showPickUpReceiverDialog);

    QPushButton* hangUpButton = new QPushButton("Положить трубку ☏", this);
    layoutCon->addWidget(hangUpButton); // Assuming layoutCon is your layout for the left block
    connect(hangUpButton, &QPushButton::clicked, this, &AbonentManager::showHangUpReceiverDialog);

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
        for (auto chatWindow : chatWindows)
        {
            if ((chatWindow->fromPhone == to && chatWindow->toPhone == from))
            {
                chatWindow->appendMessage(from + ": " + message);
                break;
            }
        }
    });
}

Person* AbonentManager::findPerson(const QString& phone) {
    for (auto person : persons) {
        if (person->getPhone() == phone) return person;
    }
    return nullptr;
}
bool AbonentManager::appendPerson(const QString& name, const QString& phone) {
    if (findPerson(phone)) return false;
    persons.append(new Person(name, phone, &ats));
    return true;
}

QString AbonentManager::getStylesViaStatus(Abonent::ConnectionStatus status) {
    switch (status) {
    case Abonent::ConnectionStatus::Free: {
        return "color: red;";
    }
    case Abonent::ConnectionStatus::Ready: {
        return "color: green;";
    }
    case Abonent::ConnectionStatus::InCall: {
        return "color: blue;";
    }
    }
}


void AbonentManager::handleCallEnded(const QString& fromPhone, const QString& toPhone)
{
    Person* endingAbonent = findPerson(fromPhone);
    if (endingAbonent) {
        endingAbonent->endCall();
    }

    QList<int> toRemove;
    for (int i = 0; i < chatWindows.length(); i++)
    {
        auto chatWindow = chatWindows[i];
        if ((chatWindow->fromPhone == fromPhone && chatWindow->toPhone == toPhone) ||
            (chatWindow->fromPhone == toPhone && chatWindow->toPhone == fromPhone))
        {
            toRemove.append(i);

            disconnect(chatWindow, &ChatWindow::messageSent, this, &AbonentManager::sendMessage);
            disconnect(chatWindow, &ChatWindow::callEnded, this, &AbonentManager::handleCallEnded);
            chatWindow->accept();
            delete chatWindow;
        }
    }

    for (auto idx : toRemove) {
        chatWindows.remove(idx, 2);
        break;
    }

    for (QWidget* abonentWidget : abonentsWidget) {

        QHBoxLayout* abonentLayout = qobject_cast<QHBoxLayout*>(abonentWidget->layout());

        if (abonentLayout) {

            QLabel* phoneLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(1)->widget());

            QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget());


            if (phoneLabel) {

                if (phoneLabel->text() == fromPhone) {

                    if (statusLabel) {
                        QString currStatus = ats.getAbonentStatusString(fromPhone);
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(fromPhone)));

                    }

                } else if (phoneLabel->text() == toPhone) {

                    if (statusLabel) {
                        QString currStatus = ats.getAbonentStatusString(toPhone);
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(toPhone)));

                    }

                }

            }

        }

    }

}


void AbonentManager::showHangUpReceiverDialog()
{
    // Create a dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Выберите абонента");

    // Create a layout for the dialog
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("Выберите абонента:", this);
    layout->addWidget(label);

    // Create a combo box to select an abonent
    QComboBox* abonentComboBox = new QComboBox(this);
    // QList<Abonent*> abonentsList = ats.getAllAbonents();

    for (Person* person : persons)
    {
        abonentComboBox->addItem(person->getName(), person->getPhone());
    }


    layout->addWidget(abonentComboBox);


    // Create an OK button
    QPushButton* okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, [&dialog, abonentComboBox]()
    {
        dialog.accept(); // Close the dialog when OK is clicked
    });


    layout->addWidget(okButton);


    // Show the dialog and wait for user input
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selectedPhone = abonentComboBox->currentData().toString();
        Person* selectedPerson = findPerson(selectedPhone);

        if (selectedPerson)
        {
            selectedPerson->endCall();
            selectedPerson->hangUpPhone();

            // Update the UI to reflect the change in status
            for (QWidget* abonentWidget : abonentsWidget)
            {
                QHBoxLayout* abonentLayout = qobject_cast<QHBoxLayout*>(abonentWidget->layout());
                if (abonentLayout)
                {
                    QLabel* phoneLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(1)->widget()); // Assuming phone label is at index 1
                    if (phoneLabel && phoneLabel->text() == selectedPhone)
                    {
                        QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget()); // Assuming status label is at index 2
                        if (statusLabel)
                        {
                            QString currStatus = ats.getAbonentStatusString(selectedPhone);
                            statusLabel->setText(currStatus); // Update the status label to "Отошел"
                            statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(selectedPhone))); // Set color to red or any color you prefer
                        }
                        break;
                    }
                }
            }
        }
    }
    updateCurrentConnections();
}

void AbonentManager::showPickUpReceiverDialog()
{
    if (ats.getCurrentConnections() == ats.getMaxConnections()) {
        QMessageBox::warning(this, "Ошибка", "Не осталось свободных соединений");
        return;
    }
    // Create a dialog

    QDialog dialog(this);

    dialog.setWindowTitle("Выберите абонента");

    // Create a layout for the dialog
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("Выберите абонента:", this);
    layout->addWidget(label);


    // Create a combo box to select an abonent
    QComboBox* abonentComboBox = new QComboBox(this);

    for (Person* person : persons)
    {
        abonentComboBox->addItem(person->getName(), person->getPhone());
    }

    layout->addWidget(abonentComboBox);

    // Create an OK button
    QPushButton* okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, [&dialog, abonentComboBox]() {
        dialog.accept(); // Close the dialog when OK is clicked
    });

    layout->addWidget(okButton);

    // Show the dialog and wait for user input
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selectedPhone = abonentComboBox->currentData().toString();

        Person* selectedPerson = findPerson(selectedPhone);

        if (selectedPerson)
        {
            selectedPerson->pickUpPhone();

            // Update the UI to reflect the change in status

            for (QWidget* abonentWidget : abonentsWidget)

            {

                QHBoxLayout* abonentLayout = qobject_cast<QHBoxLayout*>(abonentWidget->layout());

                if (abonentLayout)

                {

                    QLabel* phoneLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(1)->widget()); // Assuming phone label is at index 1

                    if (phoneLabel && phoneLabel->text() == selectedPhone)

                    {

                        QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget()); // Assuming status label is at index 2

                        if (statusLabel)

                        {
                            QString currStatus = ats.getAbonentStatusString(selectedPhone);
                            statusLabel->setText(currStatus); // Update the status label to "Отошел"
                            statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(selectedPhone))); // Set color to red or any color you prefer

                        }

                        break;

                    }

                }

            }

        }
    }
}

// Implement the removeSelectedAbonent slot
void AbonentManager::removeSelectedAbonent()
{

}


// Implement the showCreateConnectionDialog slot
void AbonentManager::showCreateConnectionDialog()
{
    if (ats.getMaxConnections() == ats.getCurrentConnections()) {
        QMessageBox::warning(this, "Ошибка", "Не осталось свободных соединений");
        return;
    }

    QDialog* createConnectionDialog = new QDialog(this);
    createConnectionDialog->setWindowTitle("Создать соединение");
    QVBoxLayout* dialogLayout = new QVBoxLayout(createConnectionDialog);


    // Create combo boxes for selecting abonents
    QComboBox* abonent1ComboBox = new QComboBox(createConnectionDialog);
    QComboBox* abonent2ComboBox = new QComboBox(createConnectionDialog);


    // Populate the combo boxes with abonents
    for (Person* person : persons)
    {
        abonent1ComboBox->addItem(person->getName(), person->getPhone());
        abonent2ComboBox->addItem(person->getName(), person->getPhone());
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

    if (createConnectionDialog->exec() == QDialog::Accepted)
    {

        QString abonent1Phone = abonent1ComboBox->currentData().toString();
        QString abonent2Phone = abonent2ComboBox->currentData().toString();

        if (abonent1Phone == abonent2Phone) {
            QMessageBox::warning(this, "Ошибка", "Нельзя позвонить самому себе");
            return;
        }
        if (ats.getAbonentStatus(abonent1Phone) != Abonent::ConnectionStatus::Ready || ats.getAbonentStatus(abonent2Phone) != Abonent::ConnectionStatus::Ready)
        {
            QMessageBox::warning(this, "Ошибка", "Оба абонента должны быть в статусе 'Готов' для создания соединения.");
            return;
        }


        // Logic to create the connection
        Person* caller = findPerson(abonent1Phone);
        Person* target = findPerson(abonent2Phone);
        caller->makeCall(target);
        updateCurrentConnections();

        for (QWidget* abonentWidget : abonentsWidget)
        {

            QHBoxLayout* abonentLayout = qobject_cast<QHBoxLayout*>(abonentWidget->layout());
            if (abonentLayout)
            {
                QLabel* phoneLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(1)->widget());
                if (phoneLabel && phoneLabel->text() == caller->getPhone())
                {
                    QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget());
                    if (statusLabel)
                    {
                        QString currStatus = ats.getAbonentStatusString(caller->getPhone());
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(caller->getPhone())));
                    }
                } else if (phoneLabel && phoneLabel->text() == target->getPhone())
                {
                    QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget());
                    if (statusLabel)
                    {
                        QString currStatus = ats.getAbonentStatusString(target->getPhone());
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(target->getPhone())));
                    }
                }
            }
        }

        ChatWindow* chatWindowA = new ChatWindow(abonent1Phone, abonent2Phone, this);
        ChatWindow* chatWindowB = new ChatWindow(abonent2Phone, abonent1Phone, this);

        // Connect signals to send messages through ATS
        connect(chatWindowA, &ChatWindow::messageSent, this, &AbonentManager::sendMessage);
        connect(chatWindowB, &ChatWindow::messageSent, this, &AbonentManager::sendMessage);

        // Connect signal to end the call
        connect(chatWindowA, &ChatWindow::callEnded, this, &AbonentManager::handleCallEnded);
        connect(chatWindowB, &ChatWindow::callEnded, this, &AbonentManager::handleCallEnded);


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
    // ats.sendMessage(fromPhone, toPhone, message);
    Person* from = findPerson(fromPhone);
    Person* to = findPerson(toPhone);
    if (from && to) {
        from->sendMessage(to, message);
    }
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
            if (appendPerson(name, phone)) addAbonent(name, phone);
            else {
                QMessageBox::warning(this, "Ошибка", "Абонент с таким номером уже существует");
                return;
            }
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

    QLabel* statusLabel = new QLabel(ats.getAbonentStatusString(phone), abonentWidget);
    statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(phone)));

    // Only add the delete button and status label
    abonentLayout->addWidget(nameLabel);
    abonentLayout->addWidget(phoneLabel);
    abonentLayout->addWidget(statusLabel); // Add the status label

    layout->addWidget(abonentWidget);

    abonentsWidget.push_back(abonentWidget);

    updateHeader();
}

void AbonentManager::removeAbonent(QWidget* abonent)
{
   layout->removeWidget(abonent);
   abonent->deleteLater();
   abonentsWidget.erase(std::remove(abonentsWidget.begin(), abonentsWidget.end(), abonent), abonentsWidget.end());
   updateHeader();
   drawAbonentsTable();
}

void AbonentManager::updateHeader()
{
   headerLabel->setText("Список абонентов (" + QString::number(abonentsWidget.size()) + ")");
}

void AbonentManager::drawAbonentsTable()
{
    for (auto p : persons) {
        addAbonent(p->getName(), p->getPhone());
    }
}

void AbonentManager::initiateCall(const QString& callerPhone, const QString& targetPhone)
{
    Person* caller = findPerson(callerPhone);
    Person* target = findPerson(targetPhone);

    caller->makeCall(target);

    // Update the UI for both abonents

    for (QWidget* abonentWidget : abonentsWidget) {

        QHBoxLayout* abonentLayout = qobject_cast<QHBoxLayout*>(abonentWidget->layout());

        if (abonentLayout) {

            QLabel* phoneLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(1)->widget()); // Assuming phone label is at index 1

            QLabel* statusLabel = qobject_cast<QLabel*>(abonentLayout->itemAt(2)->widget()); // Assuming status label is at index 2


            if (phoneLabel) {

                if (phoneLabel->text() == callerPhone) {
                    if (statusLabel) {
                        QString currStatus = ats.getAbonentStatusString(callerPhone);
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(callerPhone)));
                    }
                } else if (phoneLabel->text() == targetPhone) {
                    if (statusLabel) {
                        QString currStatus = ats.getAbonentStatusString(targetPhone);
                        statusLabel->setText(currStatus);
                        statusLabel->setStyleSheet(getStylesViaStatus(ats.getAbonentStatus(targetPhone)));
                    }
                }
            }
        }
    }
}

void AbonentManager::updateCurrentConnections() {
    headerLabelCon->setText(QString("Список соединений (%1)").arg(ats.getCurrentConnections()));
}
void AbonentManager::increaseMaxConnections()
{
    ats.setMaxCallsCount(ats.getMaxConnections() + 1);
    maxConnectionsLabel->setText(QString("Максимальное количество соединений: %1").arg(ats.getMaxConnections()));
}


void AbonentManager::decreaseMaxConnections()
{
    if (ats.getCurrentConnections() == ats.getMaxConnections()) {
        QMessageBox::warning(this, "Ошибка", "Остались незавершённые звонки - закончите их, чтобы уменить макс. число соединений");
        return;
    }

    if (ats.getMaxConnections() <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Меньше нуля быть не может");
        return;
    }

    ats.setMaxCallsCount(ats.getMaxConnections() - 1);
    maxConnectionsLabel->setText(QString("Максимальное количество соединений: %1").arg(ats.getMaxConnections()));
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
