#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ÉTABLIR LA CONNEXION D'ABORD (comme page 49)
    Connection* c = Connection::instance();
    bool test = c->createConnect();

    if (test) {
        // PUIS INSTANCIER MAINWINDOW (comme page 49)
        MainWindow SSC;
        SSC.show();
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connection successful.\nClick Cancel to exit."), QMessageBox::Cancel);
        return a.exec();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
}
