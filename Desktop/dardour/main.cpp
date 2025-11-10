#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "\n========================================";
    qDebug() << "DÉMARRAGE DE L'APPLICATION";
    qDebug() << "========================================\n";

    // Connexion à la base de données avec Singleton
    Connection& c = Connection::createInstance();

    if(c.createConnect())
    {
        qDebug() << "\n--- Test de la table ALERTES ---";

        // Test: vérifier que la table existe
        QSqlQuery testQuery;
        if(testQuery.exec("SELECT COUNT(*) FROM ALERTES"))
        {
            if(testQuery.next())
            {
                int count = testQuery.value(0).toInt();
                qDebug() << "✅ Table ALERTES accessible!";
                qDebug() << "   Nombre de lignes:" << count;
            }
        }
        else
        {
            qDebug() << "❌ Erreur lecture table:" << testQuery.lastError().text();
        }

        qDebug() << "--- Fin du test ---\n";

        // Lancer l'interface
        MainWindow w;
        w.show();

        qDebug() << "\n========================================";
        qDebug() << "APPLICATION LANCÉE - Interface affichée";
        qDebug() << "========================================\n";

        return a.exec();
    }
    else
    {
        qDebug() << "\n========================================";
        qDebug() << "ÉCHEC DE LA CONNEXION À LA BASE";
        qDebug() << "========================================\n";

        QMessageBox::critical(nullptr, "Erreur de connexion",
                              "❌ Impossible de se connecter à la base de données!\n\n"
                              "Vérifiez:\n"
                              "• Que Oracle est démarré\n"
                              "• Que la source ODBC 'Source_Projet2A' existe\n"
                              "• Les identifiants (dorra / oracle123)\n\n"
                              "Consultez la console pour plus de détails.");
        return -1;
    }
}
