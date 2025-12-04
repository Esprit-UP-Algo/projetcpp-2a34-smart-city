#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include "connection.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "serialreader.h"
#include "employee.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug() << "=== APPLICATION DÉMARRÉE ===";

    // Style de l'application
    app.setStyle("Fusion");

    // Palette sombre
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    // Établir la connexion à la base de données
    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Impossible de se connecter à la base de données.\n"
                              "L'application va se fermer.");
        return -1;
    }

    // Initialiser la colonne RFID si nécessaire
    employee::ajouterColonneRFID();

    // Initialize SerialReader for RFID
    SerialReader *reader = new SerialReader();
    reader->openPort("COM5");

    
    // Variables pour les fenêtres
    LoginWindow *loginWindow = new LoginWindow();
    MainWindow *mainWindow = nullptr;
    bool rfidAuthenticated = false;
    
    // Connecter le signal RFID pour ouvrir automatiquement la fenêtre principale
    QObject::connect(reader, &SerialReader::uidReceived, [&](QString uid){
        qDebug() << "📡 UID RFID reçu :" << uid;

        // Vérifier si l'UID existe dans la base de données avec requête directe
        QSqlQuery query;
        query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE RFID_UID = :rfid_uid");
        query.bindValue(":rfid_uid", uid);
        
        qDebug() << "🔍 Requête RFID (main.cpp) - UID recherché:" << uid;
        
        if (query.exec() && query.next()) {
            QString cin = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QString nomEmploye = nom + " " + prenom;
            
            qDebug() << "✅ Accès autorisé pour :" << nomEmploye << "(CIN:" << cin << ")";
            
            // Marquer l'authentification RFID comme réussie
            rfidAuthenticated = true;
            
            // Afficher le message de succès sur la fenêtre de connexion
            QMessageBox::information(loginWindow, "✅ Accès Autorisé", 
                                    QString("👋 Bienvenue %1!\n\n🔓 Accès autorisé par carte RFID\n🆔 CIN: %2\n🎫 UID: %3")
                                    .arg(nomEmploye).arg(cin).arg(uid));
            
            // Fermer la fenêtre de connexion
            if (loginWindow && loginWindow->isVisible()) {
                loginWindow->close();
                loginWindow = nullptr;
            }
            
            // Créer et afficher la fenêtre principale
            if (!mainWindow) {
                mainWindow = new MainWindow();
                mainWindow->setWindowTitle("Gestion Intégrée - Employés & Équipements");
                mainWindow->resize(1400, 700);
                mainWindow->show();
            }
            
        } else {
            qDebug() << "❌ Accès refusé - UID inconnu :" << uid;
            if (!query.exec()) {
                qDebug() << "❌ Erreur requête RFID:" << query.lastError().text();
            }
            QMessageBox::warning(nullptr, "❌ Accès Refusé", 
                               "🚫 Carte RFID non reconnue!\n\n"
                               "Veuillez contacter l'administrateur\n"
                               "ou utiliser une carte autorisée.");
        }
    });

    // Afficher la fenêtre de connexion au démarrage
    loginWindow->show();
    
    // Si la connexion manuelle réussit, afficher la fenêtre principale
    QObject::connect(loginWindow, &QDialog::accepted, [&](){
        if (!mainWindow) {
            mainWindow = new MainWindow();
            mainWindow->setWindowTitle("Gestion Intégrée - Employés & Équipements");
            mainWindow->resize(1400, 700);
            mainWindow->show();
        }
    });

    return app.exec();
}

