#ifndef ABONENTWIDGET_H
#define ABONENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "chatWindow.h"
#include "baseInterface.h"

class AbonentWidget : public QWidget, BaseInterface {
    Q_OBJECT

public:
    AbonentWidget(const QString& name, const QString& phone, const QString& status = "Свободен", QWidget* parent = nullptr);
    ~AbonentWidget();
    QString getPhone();
signals:
    void deleteRequested(); // Signal emitted when delete is requested
private slots:
    void makeCall();
    void deleteAbonent(); // Slot for handling deletion
private:
    QString name;
    QString phone;
    QString status;
    QLabel* nameLabel;
    QLabel* phoneLabel;
    QLabel* statusLabel;
    QPushButton* callButton;
    QPushButton* deleteButton;
};


#endif // ABONENTWIDGET_H
