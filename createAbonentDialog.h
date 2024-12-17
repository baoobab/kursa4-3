#ifndef CREATEABONENTDIALOG_H
#define CREATEABONENTDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

class CreateAbonentDialog : public QDialog {
    Q_OBJECT

public:
    CreateAbonentDialog(QWidget* parent = nullptr);
    ~CreateAbonentDialog();
private slots:
    void createAbonent();

signals:
    void abonentCreated(const QString& name, const QString& status);

private:
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* statusInput;
};


#endif // CREATEABONENTDIALOG_H
