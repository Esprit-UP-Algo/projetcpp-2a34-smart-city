#ifndef FACEENROLLDIALOG_H
#define FACEENROLLDIALOG_H

#include <QDialog>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QImage>

class QVideoWidget;
class QLabel;
class QPushButton;
class MXFaceAPI;

class FaceEnrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaceEnrollDialog(const QString &employeeId, QWidget *parent = nullptr);
    ~FaceEnrollDialog();

private slots:
    void onCaptureClicked();
    void onCancelClicked();
    void onEnrollClicked();
    void processCapturedImage(int id, const QImage &image);
    void onEnrollmentCompleted(bool success, const QString &message);

private:
    QString m_employeeId;

    QCamera *m_camera;
    QMediaCaptureSession m_captureSession;
    QImageCapture *m_imageCapture;

    QVideoWidget *m_viewfinder;
    QLabel *m_capturedLabel;
    QPushButton *m_captureButton;
    QPushButton *m_cancelButton;
    QPushButton *m_enrollButton;

    QImage m_currentImage;
    MXFaceAPI *m_faceAPI;
    
    void saveImageLocally(const QImage &image);
    void enrollWithMXFace(const QImage &image);
};

#endif // FACEENROLLDIALOG_H
