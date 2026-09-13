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
#include <QVideoSink>
#include <QVideoFrame>
#include <QDir.h>


CLiveCapture::CLiveCapture(QWidget* parent)
    : QMainWindow(parent),
      camera(nullptr),
      captureSession(nullptr),
      videoSink(nullptr)
{
    setWindowTitle("Live Capture Demo");
    resize(600, 400);

    startButton = new QPushButton("Start Capture", this);
    videoWidget = new QVideoWidget(this);
    
    startButton->setGeometry(200, 360, 200, 40);
    videoWidget->setGeometry(50, 20, 500, 300);

    videoSink = new QVideoSink(this);

    QObject::connect(startButton, &QPushButton::clicked,
        this, [this]()
        {
            if (camera)
                stopCapture();
            else
                startCapture();
        });

    QObject::connect(videoSink, &QVideoSink::videoFrameChanged,
        this,
        [this](const QVideoFrame& frame)
        {
            if (!fpsTimer.isValid())
                fpsTimer.start();

            ++frameCount;

            if (fpsTimer.elapsed() >= 1000)
            {
                qDebug() << "FPS:" << frameCount;

                frameCount = 0;
                fpsTimer.restart();
            }

            QVideoFrame copy(frame);

            if (!copy.map(QVideoFrame::ReadOnly))
            {
                qDebug() << "Could not map frame";
                return;
            }

            QImage image = copy.toImage();

            copy.unmap();

            if (image.isNull())
            {
                qDebug() << "Could not convert frame to image";
                return;
            }

            if (!frameSaved)
            {
                QVideoFrame copy(frame);

                if (!copy.map(QVideoFrame::ReadOnly))
                {
                    qDebug() << "Could not map frame";
                    return;
                }

                QImage image = copy.toImage();

                copy.unmap();

                if (!image.isNull())
                {
                    const QString fileName =
                        QDir::currentPath() + "/capture.png";

                    if (image.save(fileName))
                    {
                        frameSaved = true;
                        qDebug() << "Frame saved to:" << fileName;
                    }
                }
            }
            // !! commented: measure FPS, not: outtput to videoWidget
            //captureSession->setVideoOutput(videoWidget);        
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

    frameSaved = false;
    camera = new QCamera(cameras.first(), this);



    captureSession = new QMediaCaptureSession(this);
    captureSession->setCamera(camera);
    //captureSession->setVideoOutput(videoWidget);

    captureSession->setVideoOutput(videoSink);




    frameSaved = false;
    fpsTimer.invalidate();
    frameCount = 0;

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


