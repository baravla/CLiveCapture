#pragma once
#include <QMainWindow>

class QPushButton;
class QCamera;
class QMediaCaptureSession;
class QVideoWidget;

class CLiveCapture : public QMainWindow
{

    Q_OBJECT

public:
    explicit CLiveCapture(QWidget* parent = nullptr);

private slots:
    void startCapture();
    void stopCapture();


signals:
    void captureStarted(const QString& deviceName);

private:
    QPushButton* startButton;
    QCamera* camera;
    QMediaCaptureSession* captureSession;
    QVideoWidget* videoWidget;
};
