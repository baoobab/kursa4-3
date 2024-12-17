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

class AbonentManager : public QWidget {
    Q_OBJECT

public:
    AbonentManager(QWidget* parent = nullptr);
    ~AbonentManager();
private slots:
    void showCreateDialog();
    void addAbonent(const QString& name, const QString& phone, const QString& status);
    void removeAbonent(AbonentWidget* abonent);

private:
    void updateHeader();

private:
    QLabel *headerLabel;
    QLabel *headerLabelCon;
    QPushButton *createButton;
    QVBoxLayout *layout;
    QVBoxLayout *layoutCon;
    std::vector<AbonentWidget *> abonents;
};


#endif // ABONENTMANAGER_H
