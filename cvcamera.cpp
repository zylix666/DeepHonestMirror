#include "cvcamera.h"

CVcamera::CVcamera(QObject *parent):QThread(parent)
{
    isPreview = true;
    cFPS = 30;
    fdet = new FaceDetector();
}

CVcamera::CVcamera(int camera_fps, QObject *parent):QThread(parent){
    isPreview = true;
    cFPS = camera_fps;
    fdet = new FaceDetector();
}

CVcamera::~CVcamera()
{
    delete fdet;
    capture.release();
}

void CVcamera::prepareCamera(int cam){
    // Initialize camera.
    VideoCapture l_capture(cam);
    capture = l_capture;
    capture.set(CV_CAP_PROP_FRAME_WIDTH , 1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT , 720);
    if(!capture.isOpened()){
        qDebug() << "Openning camera failed.";
        exit(0);
    }
}


void CVcamera::run()
{
    while(!this->isInterruptionRequested()){
        while(!isPreview){
            capture.read(frame);
            RGBframe = fdet->get_annotated_image(frame,1.3, false);
            if (RGBframe.channels()== 3){
                cv::cvtColor(RGBframe, RGBframe, CV_BGR2RGB);
                img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
            }
            emit processedCamera(img);
            this->msleep(3);
        }
    }
}

void CVcamera::Start(){
    isPreview = false;
}

void CVcamera::Stop(){
    isPreview = true;
}

void CVcamera::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

Mat CVcamera::detect_face(Mat original, double scale){
    return fdet->get_annotated_image(original, scale, false);
}

Mat CVcamera::get_ROI(){
    return fdet->get_roi();
}
