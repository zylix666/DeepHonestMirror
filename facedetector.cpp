#include "facedetector.h"

FaceDetector::FaceDetector()
{
    initial_FaceDetector();
}

FaceDetector::~FaceDetector()
{

}

void FaceDetector::initial_FaceDetector()
{
    if( !cascade.load( cascadeName ) )
    {
        qDebug() << "ERROR: Could not load classifier cascade";
        exit(0);
    }
}

cv::Mat FaceDetector::get_annotated_image(cv::Mat original, double scale_factor, bool tryflip){
    original.copyTo(Annotated);
    detectAndDraw(Annotated, cascade, nestedCascade, scale_factor, false);
    return Annotated;
}

cv::Mat FaceDetector::get_roi(){
    return RGBroi;
}

void FaceDetector::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip ){
    int i = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }

    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r->width/r->height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            //Prepare crop buffer
            cv::Point top_left = Point(cvRound(r->x*scale), cvRound(r->y*scale));
            cv::Point bottom_right = Point(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale));
            //qDebug() << "ROI width = " << cvRound((r->x + r->width-1)*scale)-cvRound(r->x*scale);
            //qDebug() << "ROI height = " << cvRound((r->y + r->height-1)*scale)-cvRound(r->y*scale);
            cv::Rect  obj_area(top_left, bottom_right);
            //crop
            RGBroi = img(obj_area).clone();
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                       cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                       color, 3, 8, 0);
        }
    }
}
