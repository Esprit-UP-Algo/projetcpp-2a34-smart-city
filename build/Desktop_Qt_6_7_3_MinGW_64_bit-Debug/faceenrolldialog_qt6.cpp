#include "faceenrolldialog.h"

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

FaceEnrollDialog::FaceEnrollDialog(const QString &employeeId, QWidget *parent)
    : QDialog(parent),
      m_employeeId(employeeId),
      m_camera(new QCamera(this)),
      m_imageCapture(new QImageCapture(this)),
      m_viewfinder(new QVideoWidget(this)),
      m_capturedLabel(new QLabel(this)),
      m_captureButton(new QPushButton("Capturer visage", this)),
      m_cancelButton(new QPushButton("Annuler", this))
{
    setWindowTitle("Enregistrer le visage de l'employe");
    resize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_viewfinder->setMinimumSize(640, 360);

    m_capturedLabel->setMinimumSize(240, 180);
    m_capturedLabel->setAlignment(Qt::AlignCenter);
    m_capturedLabel->setStyleSheet("border: 1px solid #cccccc; background-color: #f8f8f8;");

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(m_viewfinder, 3);
    topLayout->addWidget(m_capturedLabel, 2);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_captureButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);

    m_captureSession.setCamera(m_camera);
    m_captureSession.setImageCapture(m_imageCapture);
    m_captureSession.setVideoOutput(m_viewfinder);

    connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)),
            this, SLOT(processCapturedImage(int,QImage)));

    connect(m_captureButton, SIGNAL(clicked()), this, SLOT(onCaptureClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    m_camera->start();
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
    m_capturedLabel->setPixmap(QPixmap::fromImage(image).scaled(
                                m_capturedLabel->size(),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation));

    if (m_currentImage.isNull()) {
        QMessageBox::warning(this, "Erreur", "Impossible de capturer l'image du visage.");
        return;
    }

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

    if (!m_currentImage.save(fullPath)) {
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

    QMessageBox::information(this,
                             "Enregistrement du visage",
                             QString("Le visage de l'employe a ete enregistre dans:\n%1\n\nEt sauvegarde dans la base de donnees!").arg(fullPath));
    accept();
}
