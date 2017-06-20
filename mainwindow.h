#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include "cvcamera.h"
#include <caffe/caffe.hpp>
#include "roi_analyzer.h"
#include "labels.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Dispaly camera frame in preview UI
    void updateCameraUI(QImage image);

    void on_pushButton_3_clicked();

    void on_analyze_pushButton_clicked();

    void on_stop_pushButton_clicked();

    void on_sampling_timeout();

    void open();

    void on_Image_checkBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    CVcamera*     myCamera;
    cv::Mat       ROI_buf;
    cv::Mat roi;
    QTimer *sampling_timer;
    roi_analyzer *roi_ana;
    QString file_name;
    QImage image;
    QString curFile;

private:
    void createActions();
    bool loadFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void predict_gender_age(Mat roi);

};

#endif // MAINWINDOW_H
