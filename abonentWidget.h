#ifndef ABONENTWIDGET_H
#define ABONENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "chatWindow.h"

class AbonentWidget : public QWidget {
    Q_OBJECT

public:
    AbonentWidget(const QString& name, const QString& phone, const QString& status, QWidget* parent = nullptr);
    ~AbonentWidget();
private slots:
    void makeCall();

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
