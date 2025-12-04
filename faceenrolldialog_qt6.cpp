#include "faceenrolldialog.h"
#include "mxfaceapi.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QVideoWidget>
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QProgressDialog>

FaceEnrollDialog::FaceEnrollDialog(const QString &employeeId, QWidget *parent)
    : QDialog(parent),
      m_employeeId(employeeId),
      m_camera(new QCamera(this)),
      m_imageCapture(new QImageCapture(this)),
      m_viewfinder(new QVideoWidget(this)),
      m_capturedLabel(new QLabel(this)),
      m_captureButton(new QPushButton("Capturer visage", this)),
      m_cancelButton(new QPushButton("Annuler", this)),
      m_enrollButton(new QPushButton("Enregistrer", this)),
      m_faceAPI(new MXFaceAPI(this))
{
    setWindowTitle(QString("Enregistrer le visage - CIN: %1").arg(employeeId));
    resize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_viewfinder->setMinimumSize(640, 360);

    m_capturedLabel->setMinimumSize(240, 180);
    m_capturedLabel->setAlignment(Qt::AlignCenter);
    m_capturedLabel->setStyleSheet("border: 1px solid #cccccc; background-color: #f8f8f8;");
    m_capturedLabel->setText("Aucune image capturée");

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(m_viewfinder, 3);
    topLayout->addWidget(m_capturedLabel, 2);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_captureButton);
    buttonLayout->addWidget(m_enrollButton);
    buttonLayout->addWidget(m_cancelButton);
    
    m_enrollButton->setEnabled(false);
    m_enrollButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);

    m_captureSession.setCamera(m_camera);
    m_captureSession.setImageCapture(m_imageCapture);
    m_captureSession.setVideoOutput(m_viewfinder);

    connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)),
            this, SLOT(processCapturedImage(int,QImage)));

    connect(m_captureButton, SIGNAL(clicked()), this, SLOT(onCaptureClicked()));
    connect(m_enrollButton, SIGNAL(clicked()), this, SLOT(onEnrollClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    // Connect MXFaceAPI signals
    connect(m_faceAPI, &MXFaceAPI::enrollmentCompleted, 
            this, &FaceEnrollDialog::onEnrollmentCompleted);

    // Try to start camera
    if (m_camera->isAvailable()) {
    m_camera->start();
    } else {
        QMessageBox::warning(this, "Caméra non disponible",
                            "Aucune caméra détectée. Vous pouvez toujours charger une image depuis un fichier.");
    }
}

FaceEnrollDialog::~FaceEnrollDialog()
{
    if (m_camera && m_camera->isActive()) {
        m_camera->stop();
    }
}

void FaceEnrollDialog::onCaptureClicked()
{
    if (m_imageCapture && m_imageCapture->isReadyForCapture()) {
        m_imageCapture->capture();
    }
}

void FaceEnrollDialog::onCancelClicked()
{
    reject();
}

void FaceEnrollDialog::processCapturedImage(int, const QImage &image)
{
    m_currentImage = image;
    
    if (m_currentImage.isNull()) {
        QMessageBox::warning(this, "Erreur", "Impossible de capturer l'image du visage.");
        m_enrollButton->setEnabled(false);
        return;
    }

    // Afficher l'image capturée
    m_capturedLabel->setPixmap(QPixmap::fromImage(image).scaled(
                                m_capturedLabel->size(),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation));

    // Activer le bouton d'enregistrement
    m_enrollButton->setEnabled(true);
}

void FaceEnrollDialog::onEnrollClicked()
{
    if (m_currentImage.isNull()) {
        QMessageBox::warning(this, "Erreur", "Aucune image capturée. Veuillez d'abord capturer une image.");
        return;
    }

    // Désactiver les boutons pendant le traitement
    m_enrollButton->setEnabled(false);
    m_captureButton->setEnabled(false);
    
    // Sauvegarder localement
    saveImageLocally(m_currentImage);
    
    // Essayer d'enregistrer avec MXFace API (si configuré)
    enrollWithMXFace(m_currentImage);
}

void FaceEnrollDialog::saveImageLocally(const QImage &image)
{
    // Dossier local pour stocker les visages: <repertoire appli>/faces
    QDir appDir(QCoreApplication::applicationDirPath());
    if (!appDir.exists("faces")) {
        if (!appDir.mkpath("faces")) {
            QMessageBox::warning(this, "Erreur", "Impossible de creer le dossier 'faces'.");
            return;
        }
    }

    QDir facesDir(appDir.filePath("faces"));

    // Nom de fichier: CIN_timestamp.png (ex: 12345678_20231122_153045.png)
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString fileName = QString("%1_%2.png").arg(m_employeeId, timestamp);
    QString fullPath = facesDir.filePath(fileName);

    if (!image.save(fullPath)) {
        QMessageBox::warning(this, "Erreur", "Echec de l'enregistrement de l'image du visage.");
        return;
    }

    // ✅ SAUVEGARDER LE CHEMIN DANS LA BASE DE DONNÉES
    QSqlQuery query;
    QString sql = "UPDATE EMPLOYE SET FACE_IMAGE_PATH = :facePath WHERE CIN = :cin";
    query.prepare(sql);
    query.bindValue(":facePath", fullPath);
    query.bindValue(":cin", m_employeeId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur Base de Données", 
                           "Impossible de sauvegarder le chemin du visage: " + query.lastError().text());
        return;
    }
}

void FaceEnrollDialog::enrollWithMXFace(const QImage &image)
{
    // Essayer d'enregistrer avec MXFace API si disponible
    // Note: L'API key devrait être configurée ailleurs dans l'application
    // Pour l'instant, on sauvegarde juste localement
    // TODO: Ajouter configuration de l'API key MXFace
    
    // Simuler l'enrôlement (ou utiliser l'API réelle si configurée)
    m_faceAPI->enrollEmployee(image, m_employeeId);
}

void FaceEnrollDialog::onEnrollmentCompleted(bool success, const QString &message)
{
    // Réactiver les boutons
    m_enrollButton->setEnabled(true);
    m_captureButton->setEnabled(true);
    
    if (success) {
        QMessageBox::information(this, "Enregistrement réussi", message);
    accept();
    } else {
        // Même en cas d'échec de l'API, l'image est sauvegardée localement
        QMessageBox::warning(this, "Enregistrement partiel", 
                            QString("L'image a été sauvegardée localement.\n\n"
                                   "Erreur API: %1").arg(message));
        accept(); // Accepter quand même car l'image locale est sauvegardée
    }
}
