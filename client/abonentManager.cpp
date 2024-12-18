#include "abonentManager.h"
#include "QMap"
#include "QtDebug"
#include "../server/abonent.h"
//#include "../server/ats.h"

AbonentManager::AbonentManager(QWidget* parent) : QWidget(parent) {
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



   // draw table of abonents
   drawAbonentsTable();

   scrollArea->setMaximumWidth(400);
}

void AbonentManager::showCreateDialog() {
//       CreateAbonentDialog dialog(this);
//       dialog.exec();

    CreateAbonentDialog* dialog = new CreateAbonentDialog(this);
    connect(dialog, &CreateAbonentDialog::abonentCreated, this, &AbonentManager::addAbonent);
    dialog->exec();
}

void AbonentManager::addAbonent(const QString& name, const QString& phone, const QString& status) {
    AbonentWidget* abonentWidget = new AbonentWidget(name, phone, status);
    layout->addWidget(abonentWidget);
    abonentsWidget.push_back(abonentWidget);
    updateHeader(); // Update header after adding
}

void AbonentManager::removeAbonent(AbonentWidget* abonent) {
       layout -> removeWidget(abonent);
       abonent -> deleteLater();
       abonentsWidget.erase(std::remove(abonentsWidget.begin(), abonentsWidget.end(), abonent), abonentsWidget.end());
       updateHeader();
}

void AbonentManager::updateHeader() {
       headerLabel -> setText("Список абонентов (" + QString::number(abonentsWidget.size()) + ")");
}

void AbonentManager::drawAbonentsTable()
{
    // ### temporary stuff for test waiting for hella backend
    // example of abonents mappyyy
    QMap<QString, Abonent*> abonents;


    // Sample data for abonents
//    QString statusArr[4] = {"Готов", "Разговор", "Вызов", "Свободен"};
//    QString phoneArr[6] = {"Вася", "Коля", "Виталий", "Саня", "Аноним", "ski tiger"};
//    QString numsArr[6] = {"8 980 123 53 21", "8 972 142 32 21",
//                          "8 980 424 11 22", "8 924 332 13 78",
//                          "8 927 421 32 55", "8 970 768 05 30"};
    QString phonesExample[6] = {"8 980 123 53 21", "8 972 142 32 21",
                              "8 980 424 11 22", "8 924 332 13 78",
                              "8 927 421 32 55", "8 970 768 05 30"};
    quint16 portsExample[6] = {228, 1488, 239, 768, 282, 164};

    for (int i = 0; i < 6; ++i)
    {
        Abonent* abonent = new Abonent(phonesExample[i], portsExample[i]);
        abonents.insert(phonesExample[i], abonent);
    }
    // temporary stuff for test waiting for hella backend ###



    // iterate through map
    for (auto it = abonents.cbegin(); it != abonents.cend(); ++it)
    {
       quint16 adress = abonents[it.key()]->getAddress();
       QString phone = abonents[it.key()]->getPhone();
       Abonent::ConnectionStatus status = abonents[it.key()]->getStatus();

       QString statusStr;
       switch (status) {
           case Abonent::ConnectionStatus::Ready:
               statusStr = "Готов";
               break;
           case Abonent::ConnectionStatus::InCall:
               statusStr = "Разговор";
               break;
           case Abonent::ConnectionStatus::OnHold:
               statusStr = "Вызов";
               break;
           case Abonent::ConnectionStatus::Free:
               statusStr = "Свободен";
               break;
           default:
               statusStr = "Неизвестно"; // Handle unexpected statuses
               break;
       }
       AbonentWidget *abonentWidget= new AbonentWidget("name", phone/*, statusStr*/);
       layout->addWidget(abonentWidget );

       connect(abonentWidget, &AbonentWidget::deleteRequested, this, &AbonentManager::handleDeleteRequest);

       abonentsWidget.push_back(abonentWidget );
    }

    updateHeader();
}

void AbonentManager::handleDeleteRequest()
{

    AbonentWidget* abonent = qobject_cast<AbonentWidget*>(sender());
       if (abonent)
       {
           removeAbonent(abonent); // Call remove method
//           ATS ats; // need SINGLETONE
//           ats.removeAbonent(abonent->getPhone());
       }
       else
       {
           qDebug() << "Failed to delete abonent: sender is not an AbonentWidget.";
       }
}

AbonentManager::~AbonentManager()
{
    delete headerLabel;
    delete headerLabelCon;
    delete createButton;
    delete layout;
    delete layoutCon;
//    delet abonents;
}
