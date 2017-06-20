#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <QObject>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <ctime>
#include <iostream>
using namespace cv;

class FaceDetector
{
public:
    const string cascadeName = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
    const string nestedCascadeName = "/usr/share/opencv/haarcaddscades/haarcascade_eye_tree_eyeglasses.xml";
public:
    FaceDetector();
    ~FaceDetector();
    Mat get_annotated_image(cv::Mat original, double scale_factor, bool tryflip);
    Mat get_roi();
private:
    CascadeClassifier cascade, nestedCascade;
    Mat Annotated;
    Mat RGBroi;
    void initial_FaceDetector();
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                        CascadeClassifier& nestedCascade,
                        double scale, bool tryflip );
};

#endif // FACEDETECTOR_H
