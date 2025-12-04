#include "serialreader.h"
#include <QDebug>

SerialReader::SerialReader(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead,
            this, &SerialReader::onReadyRead);
}

void SerialReader::openPort(QString portName)
{
    qDebug() << " Tentative d'ouverture du port série:" << portName;
    
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);

    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << " Impossible d'ouvrir le port" << portName;
        qDebug() << " Erreur:" << serial->errorString();
        qDebug() << " Vérifiez:";
        qDebug() << "   - Le port COM est correct";
        qDebug() << "   - L'Arduino est connecté";
        qDebug() << "   - Le port n'est pas utilisé par autre chose";
    } else {
        qDebug() << " Port ouvert avec succès :" << portName;
        qDebug() << " Baud rate: 9600";
        qDebug() << " En attente de données RFID...";
    }
}

void SerialReader::onReadyRead()
{
    static QByteArray buffer; // Buffer pour accumuler les données
    
    QByteArray data = serial->readAll();
    buffer.append(data);
    
    // Si on trouve une ligne complète (\n ou \r\n)
    if (buffer.contains('\n') || buffer.contains('\r')) {
        QString line = QString::fromUtf8(buffer).trimmed();
        
        qDebug() << "📨 Données reçues (brutes):" << data.toHex();
        qDebug() << "📨 Ligne complète reçue:" << line;
        qDebug() << "📏 Taille des données:" << data.length() << "octets";

        // Accepter les deux formats: "UID:" et "UID du tag"
        if (line.startsWith("UID:") || line.contains("UID du tag")) {
            QString uid = "";
            
            if (line.startsWith("UID:")) {
                uid = line.split(":").last().trimmed();
            } else if (line.contains("UID du tag")) {
                uid = line.split(":").last().trimmed();
            }
            
            // Nettoyer l'UID (enlever les espaces)
            uid = uid.replace(" ", "").toUpper();
            
            qDebug() << "🎯 UID extrait et nettoyé:" << uid;
            qDebug() << "📤 Émission du signal uidReceived...";
            emit uidReceived(uid);
        } else if (!line.isEmpty()) {
            qDebug() << "⚠️ Ligne reçue mais ne contient pas d'UID:" << line;
            qDebug() << "🔍 Recherche de 'UID' dans la ligne:" << line.contains("UID");
        }
        
        buffer.clear(); // Vider le buffer après traitement
    } else if (data.length() > 0) {
        qDebug() << "ℹ️ Données partielles reçues, en attente de la ligne complète...";
    }
}
