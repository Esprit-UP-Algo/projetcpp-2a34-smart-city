#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection& c = Connection::createInstance();
    if (!c.createConnect()) {
        qDebug() << "Impossible de se connecter";
        return -1;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
