#include "include\abonentManager.h"

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

// Sample data for abonents
QString statusArr[4] = {"Готов", "Разговор", "Вызов", "Свободен"};
QString phoneArr[6] = {"Вася", "Коля", "Виталий", "Саня", "Аноним", "ski tiger"};
QString numsArr[8] = {"8 980 123 53 21", "8 972 142 32 21",
                      "8 980 424 11 22", "8 924 332 13 78",
                      "8 927 421 32 55", "8 970 768 05 30",
                      "8 980 123 53 21", "8 980 123 53 21"};

for (int i = 0; i < 20; ++i) {
       QString name= numsArr[i % 8];
       QString phone= phoneArr[i % 6];
       QString status= statusArr[i % 4];

       AbonentWidget *abonentWidget= new AbonentWidget(name ,phone ,status );

       layout -> addWidget(abonentWidget );
       abonents.push_back(abonentWidget );
}

updateHeader();

scrollArea -> setMaximumWidth(400 );
}

void AbonentManager::showCreateDialog() {
       CreateAbonentDialog dialog(this);
       dialog.exec();
}

void AbonentManager::addAbonent(const QString& name ,const QString& phone ,const QString& status ) {
       AbonentWidget *abonentWidget= new AbonentWidget(name ,phone ,status );
       layout -> addWidget(abonentWidget );
       abonents.push_back(abonentWidget );
       updateHeader();
}

void AbonentManager::removeAbonent(AbonentWidget* abonent) {
       layout -> removeWidget(abonent);
       abonent -> deleteLater();
       abonents.erase(std::remove(abonents.begin(), abonents.end(), abonent), abonents.end());
       updateHeader();
}

void AbonentManager::updateHeader() {
       headerLabel -> setText("Список абонентов (" + QString::number(abonents.size()) + ")");
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
