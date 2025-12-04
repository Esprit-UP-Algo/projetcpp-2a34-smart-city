#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== Test RFID UID Database Query ===";
    
    // Database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Oracle in OraClient21Home1};DBA=W;Server=localhost/XE;Uid=SYSTEM;Pwd=123456789;");
    
    if (!db.open()) {
        qDebug() << "❌ Erreur connexion base de données:" << db.lastError().text();
        return 1;
    }
    
    qDebug() << "✅ Base de données connectée";
    
    // Test 1: Check if RFID_UID column exists
    QSqlQuery columnQuery;
    columnQuery.prepare("SELECT COLUMN_NAME FROM ALL_TAB_COLUMNS WHERE TABLE_NAME = 'EMPLOYE' AND COLUMN_NAME = 'RFID_UID'");
    
    if (columnQuery.exec() && columnQuery.next()) {
        qDebug() << "✅ Colonne RFID_UID existe";
    } else {
        qDebug() << "❌ Colonne RFID_UID n'existe pas";
        return 1;
    }
    
    // Test 2: Get all employees with RFID_UID
    QSqlQuery allEmployeesQuery;
    allEmployeesQuery.prepare("SELECT CIN, NOM, PRENOM, RFID_UID FROM EMPLOYE ORDER BY CIN");
    
    if (allEmployeesQuery.exec()) {
        qDebug() << "\n📊 Tous les employés:";
        while (allEmployeesQuery.next()) {
            int cin = allEmployeesQuery.value(0).toInt();
            QString nom = allEmployeesQuery.value(1).toString();
            QString prenom = allEmployeesQuery.value(2).toString();
            QString rfid_uid = allEmployeesQuery.value(3).toString();
            
            QString rfidDisplay = rfid_uid.isEmpty() ? "NULL" : rfid_uid;
            qDebug() << QString("  CIN: %1 | Nom: %2 %3 | UID: %4")
                         .arg(cin).arg(nom).arg(prenom).arg(rfidDisplay);
        }
    }
    
    // Test 3: Check specific UIDs
    QStringList testUids = {"79ECF504", "3F93CFC4"};
    
    for (const QString &uid : testUids) {
        QSqlQuery uidQuery;
        uidQuery.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE RFID_UID = :uid");
        uidQuery.bindValue(":uid", uid);
        
        qDebug() << QString("\n🔍 Test UID: %1").arg(uid);
        
        if (uidQuery.exec()) {
            qDebug() << "  Requête exécutée avec succès";
            qDebug() << "  Nombre de résultats:" << uidQuery.size();
            
            if (uidQuery.next()) {
                int cin = uidQuery.value(0).toInt();
                QString nom = uidQuery.value(1).toString();
                QString prenom = uidQuery.value(2).toString();
                qDebug() << QString("  ✅ Trouvé: %1 %2 (CIN: %3)").arg(nom).arg(prenom).arg(cin);
            } else {
                qDebug() << "  ❌ Non trouvé";
            }
        } else {
            qDebug() << "  ❌ Erreur requête:" << uidQuery.lastError().text();
        }
    }
    
    // Test 4: Check employees with CIN 12358946 and 99999999
    QList<int> testCins = {12358946, 99999999};
    
    for (int cin : testCins) {
        QSqlQuery cinQuery;
        cinQuery.prepare("SELECT NOM, PRENOM, RFID_UID FROM EMPLOYE WHERE CIN = :cin");
        cinQuery.bindValue(":cin", cin);
        
        qDebug() << QString("\n👤 Test CIN: %1").arg(cin);
        
        if (cinQuery.exec() && cinQuery.next()) {
            QString nom = cinQuery.value(0).toString();
            QString prenom = cinQuery.value(1).toString();
            QString rfid_uid = cinQuery.value(2).toString();
            
            QString rfidDisplay = rfid_uid.isEmpty() ? "NULL" : rfid_uid;
            qDebug() << QString("  %1 %2 | UID: %3").arg(nom).arg(prenom).arg(rfidDisplay);
        } else {
            qDebug() << "  ❌ Employé non trouvé";
        }
    }
    
    db.close();
    qDebug() << "\n🔌 Connexion fermée";
    
    return 0;
}
