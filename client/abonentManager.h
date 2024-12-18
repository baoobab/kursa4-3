#ifndef ABONENTMANAGER_H
#define ABONENTMANAGER_H


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>
#include <QMap>
#include <QVector>
#include "ats.h" // Include ATS header
#include "abonent.h" // Include Abonent header


class AbonentManager : public QWidget
{
    Q_OBJECT

public:
    AbonentManager(QWidget* parent = nullptr);
    ~AbonentManager();

private slots:
    void showCreateDialog();
    void addAbonent(const QString& name, const QString& phone);
    void removeAbonent(QWidget* abonent);
    void updateHeader();
    void drawAbonentsTable();
    void initiateCall(const QString& callerPhone, const QString& targetPhone);
    void sendMessage(const QString& fromPhone, const QString& toPhone, const QString& message);

private:
    QLabel* headerLabel;
    QLabel* headerLabelCon;
    QPushButton* createButton;
    QVBoxLayout* layout;
    QVBoxLayout* layoutCon;
    QVector<QWidget*> abonentsWidget;


    // Create Abonent Dialog components
    QDialog* createAbonentDialog;
    QLineEdit* nameInput;
    QLineEdit* phoneInput;


    // ATS instance
    ATS ats; // Instance of ATS to manage abonents

};

#endif // ABONENTMANAGER_H
