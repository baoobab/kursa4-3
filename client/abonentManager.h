#ifndef ABONENTMANAGER_H
#define ABONENTMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include "abonentWidget.h"
#include "createAbonentDialog.h"
#include "../common/communicator.h"


class AbonentManager : public QWidget {
    Q_OBJECT

    TCommunicator* comm;
public:
    AbonentManager(QWidget* parent = nullptr);
    ~AbonentManager();
signals:
    void request(QString);
    void received(QByteArray msg);
public slots:
    void fromCommunicator(QByteArray);
    void toCommunicator(QString);
    void handleDeleteRequest();
    void addAbonent(const QString& name, const QString& phone, const QString& status = "Свободен");
private slots:
    void showCreateDialog();
    void removeAbonent(AbonentWidget* abonent);
private:
    void updateHeader();

private:
    void drawAbonentsTable();
    QLabel *headerLabel;
    QLabel *headerLabelCon;
    QPushButton *createButton;
    QVBoxLayout *layout;
    QVBoxLayout *layoutCon;
    std::vector<AbonentWidget *> abonentsWidget;
};


#endif // ABONENTMANAGER_H
