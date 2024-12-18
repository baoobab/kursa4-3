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
#include "baseInterface.h"

class AbonentManager : public QWidget, BaseInterface {
    Q_OBJECT

public:
    AbonentManager(QWidget* parent = nullptr);
    ~AbonentManager();
public slots:
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
