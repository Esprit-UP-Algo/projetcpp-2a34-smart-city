#ifndef MXFACEAPI_H
#define MXFACEAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QImage>
#include <QBuffer>
#include <QString>
#include <QEventLoop>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QRandomGenerator>
#include <QMap>

class MXFaceAPI : public QObject
{
    Q_OBJECT

public:
    explicit MXFaceAPI(QObject *parent = nullptr);

    // Configuration et méthodes principales
    void setSubscriptionKey(const QString &subscriptionKey);

    // Fonctions de base de l'API MXFace
    QJsonObject detectFaces(const QImage &image);
    QJsonObject verifyFaces(const QImage &image1, const QImage &image2);
    QJsonObject searchFace(const QImage &image, const QString &faceListId = "");

    // Fonctions principales pour l'application
    bool enrollEmployee(const QImage &faceImage, const QString &employeeId);
    bool verifyEmployee(const QImage &faceImage, const QString &employeeId);
    QString recognizeEmployee(const QImage &faceImage);

    // Fonctions utilitaires statiques
    static QImage loadImageFromFile();
    static QByteArray imageToBase64(const QImage &image);

signals:
    // Signaux pour la communication avec l'interface
    void apiResponseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);
    void enrollmentCompleted(bool success, const QString &message);
    void verificationCompleted(bool success, double confidence);
    void recognitionCompleted(const QString &userId, double confidence);

private:
    // Membres privés
    QNetworkAccessManager *networkManager;
    QString subscriptionKey;
    QString baseUrl;

    // Méthodes privées pour les appels API
    QJsonObject makeApiCall(const QString &endpoint, const QJsonObject &data = QJsonObject());
    QNetworkRequest createRequest(const QString &endpoint, const QString &customBaseUrl = QString());

    // Méthodes pour la gestion des face lists
    bool createFaceList(const QString &faceListId, const QString &name);
    bool addFaceToFaceList(const QString &faceListId, const QImage &image, const QString &userData);
    QJsonObject searchInFaceList(const QImage &image, const QString &faceListId);
};

#endif // MXFACEAPI_H
