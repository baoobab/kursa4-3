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

#include "ats.h" // Include ATS header
#include "abonent.h" // Include Abonent header

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
    void showCreateConnectionDialog(); // New slot for creating a connection
    void removeSelectedAbonent(); // New slot for removing selected abonent

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

    // ATS instance
    ATS ats; // Instance of ATS to manage abonents
    int maxConnections = 0; // TODO: redo with ats's internal amount of maxConnections
    int currentConnections = 0;
    QList<ChatWindow*> chatWindows;

};

#endif // ABONENTMANAGER_H
