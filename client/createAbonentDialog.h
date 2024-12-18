#ifndef CREATEABONENTDIALOG_H
#define CREATEABONENTDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

#include "baseInterface.h"

class CreateAbonentDialog : public QDialog, BaseInterface {
    Q_OBJECT

public:
    CreateAbonentDialog(QWidget* parent = nullptr);
    ~CreateAbonentDialog();
private slots:
    void createAbonent();

signals:
    void abonentCreated(const QString& name, const QString& phone, const QString& status = "Свободен");

private:
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* statusInput;
};


#endif // CREATEABONENTDIALOG_H
