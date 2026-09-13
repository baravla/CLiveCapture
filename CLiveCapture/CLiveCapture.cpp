#include "CLiveCapture.h"

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QDebug>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>




CLiveCapture::CLiveCapture(QWidget* parent)
    : QMainWindow(parent),
      camera(nullptr),
      captureSession(nullptr)
{
    setWindowTitle("Live Capture Demo");
    resize(600, 400);

    startButton = new QPushButton("Start Capture", this);
    videoWidget = new QVideoWidget(this);
    
    startButton->setGeometry(200, 360, 200, 40);
    videoWidget->setGeometry(50, 20, 500, 300);

    QObject::connect(startButton, &QPushButton::clicked,
        this, [this]()
        {
            if (camera)
                stopCapture();
            else
                startCapture();
        });

}

void CLiveCapture::startCapture()
{
    const QList<QCameraDevice> cameras =
        QMediaDevices::videoInputs();

    if (cameras.isEmpty())
    {
        qDebug() << "No cameras found";
        return;
    }

    camera = new QCamera(cameras.first(), this);

    captureSession = new QMediaCaptureSession(this);
    captureSession->setCamera(camera);
    captureSession->setVideoOutput(videoWidget);

    camera->start();
    startButton->setText("Stop Capture");

    setWindowTitle("Camera Started");
}

void CLiveCapture::stopCapture()
{
    camera->stop();

    camera->deleteLater();
    captureSession->deleteLater();

    camera = nullptr;
    captureSession = nullptr;

    startButton->setText("Start Capture");
    setWindowTitle("Live Capture Demo");
}

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    CLiveCapture window;
    window.show();

    return app.exec();
}


