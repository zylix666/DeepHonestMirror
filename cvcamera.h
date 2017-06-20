#ifndef CVCAMERA_H
#define CVCAMERA_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QQueue>
#include <QWaitCondition>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <ctime>
#include <iostream>
#include "facedetector.h"
using namespace cv;

class CVcamera : public QThread
{   Q_OBJECT
public:
    bool isPreview;
    CVcamera(QObject *parent = 0);
    CVcamera(int camera_fps, QObject *parent = 0);
    ~CVcamera();
    void prepareCamera(int cam = 0);
    void Start();
    //Stop camera preview
    void Stop();
    //check if the camera has been stopped
    bool isStopped() const;
    Mat get_ROI();
    Mat detect_face(Mat original, double scale);

private:
    VideoCapture capture;
    int cFPS;
    QMutex mutex;
    QWaitCondition condition;
    FaceDetector *fdet;
    Mat frame;
    int frameRate;
    Mat RGBframe;
    QImage img;
    CascadeClassifier cascade, nestedCascade;

signals:
    //Signal to output frame to be displayed
     void processedCamera(const QImage &image);

protected:
    void run();
    void msleep(int ms);
};

#endif // CVCAMERA_H
