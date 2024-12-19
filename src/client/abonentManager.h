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
#include <QComboBox> // Include QComboBox for selecting abonents

#include "server/ats.h" // Include ATS header
#include "server/abonent.h" // Include Abonent header
#include "server/person.h"

#include "chatWindow.h"

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
    void increaseMaxConnections();
    void decreaseMaxConnections();
    void updateCurrentConnections();
    void showCreateConnectionDialog(); // New slot for creating a connection
    void removeSelectedAbonent(); // New slot for removing selected abonent
    void showPickUpReceiverDialog();
    void showHangUpReceiverDialog();
    void handleCallEnded(const QString& fromPhone, const QString& toPhone);
private:
    QLabel* headerLabel;
    QLabel* headerLabelCon;
    QLabel* maxConnectionsLabel; // New label for max connections
    QPushButton* createButton;
    QPushButton* increaseConnectionsButton;
    QPushButton* decreaseConnectionsButton;
    QPushButton* createConnectionButton; // New button for creating a connection
    QVBoxLayout* layout;
    QVBoxLayout* layoutCon;
    QVector<QWidget*> abonentsWidget;


    // Create Abonent Dialog components
    QDialog* createAbonentDialog;
    QLineEdit* nameInput;
    QLineEdit* phoneInput;

    // Create Connection Dialog components
    QDialog* createConnectionDialog; // New dialog for creating a connection
    QComboBox* abonent1ComboBox; // ComboBox for selecting the first abonent
    QComboBox* abonent2ComboBox; // ComboBox for selecting the second abonent


    // ComboBox for selecting an abonent to delete
    QComboBox* abonentToDeleteComboBox; // New combo box for selecting an abonent to delete

    QPushButton* pickUpButton; // Button to pick up the receiver

    // ATS instance
    ATS ats; // Instance of ATS to manage abonents
    QList<Person*> persons; // For storing users

    QList<ChatWindow*> chatWindows;

    QString getStylesViaStatus(Abonent::ConnectionStatus status);
    Person* findPerson(const QString& phone);
    bool appendPerson(const QString& name, const QString& phone);
};

#endif // ABONENTMANAGER_H
