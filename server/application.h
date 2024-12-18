#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QCoreApplication>

class Application : public QCoreApplication
{
    Q_OBJECT
public:
    Application(int, char**);
};

#endif // APPLICATION_H
