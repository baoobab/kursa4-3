#include "include\abonentWidget.h"
#include <QVBoxLayout>

AbonentWidget::AbonentWidget(const QString& name, const QString& phone, const QString& status, QWidget* parent)
        : QWidget(parent), name(name), phone(phone), status(status) {

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

//     connect(deleteButton, &QPushButton::clicked, this, &AbonentWidget::deleteAbonent); // Uncomment if needed

   topLayout->addWidget(nameLabel);
   topLayout->addWidget(phoneLabel);
   topLayout->addWidget(statusLabel);
   topLayout->addWidget(callButton);
   topLayout->addWidget(deleteButton);

   layout->addLayout(topLayout);
}

void AbonentWidget::makeCall() {
   // Simulate calling another abonent
   QString targetAbonent = "8 970 323 10 20";
   ChatWindow* chatWindow = new ChatWindow(name, targetAbonent);
   chatWindow->setWindowTitle(name + " => " + targetAbonent);
   chatWindow->show();
}

AbonentWidget::~AbonentWidget()
{
    delete nameLabel;
    delete phoneLabel;
    delete statusLabel;
    delete callButton;
    delete deleteButton;
}
