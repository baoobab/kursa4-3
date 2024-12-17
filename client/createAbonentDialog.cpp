#include "createAbonentDialog.h"
#include "../server/ats.h"
#include "../server/abonent.h"


CreateAbonentDialog::CreateAbonentDialog(QWidget* parent) : QDialog(parent) {

   QFormLayout* layout = new QFormLayout(this);
   setWindowTitle("Добавление абонента");

   nameInput = new QLineEdit(this);
   phoneInput = new QLineEdit(this); // Not used yet but can be implemented later
   statusInput = new QLineEdit(this);

   layout->addRow("Имя:", nameInput);
   layout->addRow("Телефон:", phoneInput);
   layout->addRow("Статус:", statusInput);

   QPushButton* createButton = new QPushButton("Создать", this);

   connect(createButton, &QPushButton::clicked, this, &CreateAbonentDialog::createAbonent);

   layout->addWidget(createButton);
}

void CreateAbonentDialog::createAbonent() {
   QString name = nameInput->text();
   // TODO: handle phone input if needed.
   QString status = statusInput->text();

   if (!name.isEmpty() && !status.isEmpty()) {
//       emit abonentCreated(name, status); // Uncomment if needed.
       accept();
   }
   Abonent *abonent = new Abonent("Eblan", "+78800553535");
   ATS ats;
   ats.addAbonent(abonent);


   Abonent *abonent1 = new Abonent("Eblan2", "+7880055353222");
   ats.addAbonent(abonent1);

}

CreateAbonentDialog::~CreateAbonentDialog()
{

    delete nameInput;
    delete phoneInput;
    delete statusInput;
}
