#include "facedialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVideoWidget>
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDebug>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {

// Conversion QImage -> cv::Mat (format 8 bits, 4 canaux ou 3 canaux)
cv::Mat qimageToMat(const QImage &image)
{
    QImage img = image.convertToFormat(QImage::Format_RGBA8888);
    cv::Mat mat(img.height(), img.width(), CV_8UC4,
                const_cast<uchar*>(img.bits()), img.bytesPerLine());
    cv::Mat matBGR;
    cv::cvtColor(mat, matBGR, cv::COLOR_RGBA2BGR);
    return matBGR.clone();
}

// Prétraitement des visages : niveaux de gris + redimensionnement
cv::Mat preprocessFace(const cv::Mat &src)
{
    cv::Mat gray, resized;
    if (src.channels() == 3)
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    else if (src.channels() == 4)
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    else
        gray = src.clone();

    cv::resize(gray, resized, cv::Size(100, 100));
    resized.convertTo(resized, CV_32F);
    return resized;
}

// Distance euclidienne entre deux images (même taille, 1 canal float)
double imageDistance(const cv::Mat &a, const cv::Mat &b)
{
    cv::Mat diff;
    cv::absdiff(a, b, diff);
    return cv::norm(diff, cv::NORM_L2);
}

} // namespace

FaceDialog::FaceDialog(QWidget *parent)
    : QDialog(parent),
      m_camera(new QCamera(this)),
      m_imageCapture(new QImageCapture(this)),
      m_viewfinder(new QVideoWidget(this)),
      m_capturedLabel(new QLabel(this)),
      m_captureButton(new QPushButton("Capturer", this)),
      m_cancelButton(new QPushButton("Annuler", this))
{
    setWindowTitle("Reconnaissance faciale - Camera");
    resize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Zone du flux video + image capturee
    m_viewfinder->setMinimumSize(640, 360);

    m_capturedLabel->setMinimumSize(240, 180);
    m_capturedLabel->setAlignment(Qt::AlignCenter);
    m_capturedLabel->setStyleSheet("border: 1px solid #cccccc; background-color: #f8f8f8;");

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(m_viewfinder, 3);
    topLayout->addWidget(m_capturedLabel, 2);

    // Boutons d'action
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_captureButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);

    // Configuration de la camera (Qt6)
    m_captureSession.setCamera(m_camera);
    m_captureSession.setImageCapture(m_imageCapture);
    m_captureSession.setVideoOutput(m_viewfinder);

    connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)),
            this, SLOT(processCapturedImage(int,QImage)));

    connect(m_captureButton, SIGNAL(clicked()), this, SLOT(onCaptureClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    m_camera->start();
}

FaceDialog::~FaceDialog()
{
    if (m_camera && m_camera->isActive()) {
        m_camera->stop();
    }
}

void FaceDialog::onCaptureClicked()
{
    if (m_imageCapture && m_imageCapture->isReadyForCapture()) {
        m_imageCapture->capture();
    }
}

void FaceDialog::onCancelClicked()
{
    reject();
}

void FaceDialog::processCapturedImage(int, const QImage &image)
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

    // Conversion de l'image capturée en cv::Mat prétraité
    cv::Mat capturedMat = qimageToMat(m_currentImage);
    cv::Mat capturedFace = preprocessFace(capturedMat);

    // ✅ REVENIR À LA RECHERCHE DE FICHIERS (qui fonctionne)
    QDir appDir(QCoreApplication::applicationDirPath());
    QDir facesDir(appDir.filePath("faces"));

    if (!facesDir.exists()) {
        QMessageBox::warning(this, "Aucun visage",
                             "Aucun visage n'a été enregistré. Veuillez d'abord enregistrer le visage d'un employé.");
        return;
    }

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";
    QFileInfoList files = facesDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    if (files.isEmpty()) {
        QMessageBox::warning(this, "Aucun visage",
                             "Aucun fichier de visage n'a été trouvé dans le dossier 'faces'.");
        return;
    }

    QString bestCin;
    double bestDistance = std::numeric_limits<double>::max();

    for (const QFileInfo &info : files) {
        // Le nom de fichier est du type CIN_timestamp.png -> CIN = partie avant le premier '_'
        QString baseName = info.baseName();
        QStringList parts = baseName.split('_');
        if (parts.isEmpty())
            continue;

        QString cin = parts.first();

        cv::Mat img = cv::imread(info.absoluteFilePath().toStdString(), cv::IMREAD_COLOR);
        if (img.empty())
            continue;

        cv::Mat face = preprocessFace(img);
        if (face.size() != capturedFace.size())
            continue;

        double dist = imageDistance(capturedFace, face);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestCin = cin;
        }
    }

    if (bestCin.isEmpty()) {
        QMessageBox::warning(this,
                             "Echec",
                             "Aucun visage correspondant n'a ete trouve.");
        return;
    }

    // Seuil très simple : plus la distance est faible, meilleure est la correspondance.
    // Les valeurs exactes dependent de la camera; ici on met un seuil empirique.
    const double threshold = 3000.0;

    if (bestDistance <= threshold) {
        double confidence = 100.0 * std::max(0.0, (threshold - bestDistance) / threshold);
        
        // ✅ STOCKER LE CIN RECONNU
        m_recognizedCin = bestCin;
        
        QMessageBox::information(this,
                                 "Succes",
                                 QString("Visage reconnu.\nCIN : %1\nConfiance approx. : %2%")
                                     .arg(bestCin)
                                     .arg(confidence, 0, 'f', 1));
        accept();
    } else {
        QMessageBox::information(this,
                                 "Echec",
                                 "Aucun visage correspondant n'a ete trouve avec un niveau de confiance suffisant.");
    }
}
