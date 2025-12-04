#ifndef FACEDIALOG_H
#define FACEDIALOG_H

#include <QDialog>
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QImage>

class QVideoWidget;

class QLabel;
class QLineEdit;
class QPushButton;

class FaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaceDialog(QWidget *parent = nullptr);
    ~FaceDialog();
    
    // ✅ Récupérer le CIN de l'employé reconnu
    QString getRecognizedCin() const { return m_recognizedCin; }

private slots:
    void onCaptureClicked();
    void onCancelClicked();
    void processCapturedImage(int id, const QImage &image);

private:
    QCamera *m_camera;
    QMediaCaptureSession m_captureSession;
    QImageCapture *m_imageCapture;

    QVideoWidget *m_viewfinder;
    QLabel *m_capturedLabel;
    QPushButton *m_captureButton;
    QPushButton *m_cancelButton;

    QImage m_currentImage;
    QString m_recognizedCin;  // ✅ Stocker le CIN reconnu
};

#endif // FACEDIALOG_H
