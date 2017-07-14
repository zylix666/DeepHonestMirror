#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    myCamera = new CVcamera();
    myCamera->prepareCamera(0);
    sampling_timer = new QTimer(this);
    roi_ana = new roi_analyzer();
    QObject::connect(sampling_timer, SIGNAL(timeout()), this, SLOT(on_sampling_timeout()));
    QObject::connect(myCamera, SIGNAL(processedCamera(QImage)),     \
                      this, SLOT(updateCameraUI(QImage)));
}

MainWindow::~MainWindow()
{
    myCamera->Stop();
    delete myCamera;
    delete ui;
}
static QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
static cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        break;
    }
    return mat;
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}


void MainWindow::updateCameraUI(QImage image){
    if(!image.isNull()){
            ui->preview_label->setAlignment(Qt::AlignCenter);
            ui->preview_label->setPixmap(QPixmap::fromImage(image).scaled(ui->preview_label->size(),
                                   Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(myCamera->isPreview){
        myCamera->isPreview = false;
        myCamera->start();
    }else{
        myCamera->Stop();
    }
}

void MainWindow::predict_gender_age(Mat roi)
{
    std::vector<Prediction> detect_result = roi_ana->analyze_frame(roi);
    Prediction g_predict = detect_result.at(0);
    Prediction a_predict = detect_result.at(1);
    int g = std::stoi(g_predict.first);
    float g_conf = g_predict.second;
    int a = std::stoi(a_predict.first);
    float a_conf = a_predict.second;
    ui->textBrowser->append("Gender : " + QString(gender[g].c_str()));
    ui->textBrowser->append("Confident : " + QString::number(g_conf));
    ui->textBrowser->append("Age : " + QString(age[a].c_str()));
    ui->textBrowser->append("Confident : " + QString::number(a_conf));
}

void MainWindow::on_analyze_pushButton_clicked()
{
    QImage roi_img;
    if(!ui->Image_checkBox->isChecked()){
        ui->textBrowser->clear();
        myCamera->Stop();
        roi = myCamera->get_ROI();
        if (roi.channels()== 3){
            cv::cvtColor(roi, ROI_buf, CV_BGR2RGB);
            roi_img = QImage((const unsigned char*)(ROI_buf.data),
                               ROI_buf.cols,ROI_buf.rows,QImage::Format_RGB888);
        }
        if(!roi_img.isNull()){
                ui->face_label->setAlignment(Qt::AlignCenter);
                ui->face_label->setPixmap(QPixmap::fromImage(roi_img).scaled(ui->face_label->size(),
                                          Qt::KeepAspectRatio, Qt::FastTransformation));
        }
        predict_gender_age(roi);
        myCamera->Start();
    }else{
        cv::Mat image_to_detect =  QImage2cvMat(image);        
        image = cvMat2QImage(myCamera->detect_face(image_to_detect, 1.3));
        ui->preview_label->setPixmap(QPixmap::fromImage(image).scaled(ui->preview_label->width(),ui->preview_label->height(),    \
                                                                         Qt::IgnoreAspectRatio, Qt::FastTransformation));
        ui->preview_label->adjustSize();
        cv::Mat face_to_eval = myCamera->get_ROI();
        roi_img = cvMat2QImage(face_to_eval);
        ui->face_label->setPixmap(QPixmap::fromImage(roi_img).scaled(ui->face_label->size(),
                                                  Qt::KeepAspectRatio, Qt::FastTransformation));
        predict_gender_age(face_to_eval);

    }
}

void MainWindow::on_stop_pushButton_clicked()
{
    myCamera->Stop();
}

void MainWindow::on_sampling_timeout(){

}

bool MainWindow::loadFile(const QString &fileName){
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);
    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
    statusBar()->showMessage(message);
    return true;
}


void MainWindow::setImage(const QImage &newImage){

    image = newImage;
    ui->preview_label->setPixmap(QPixmap::fromImage(newImage).scaled(ui->preview_label->width(),ui->preview_label->height(),    \
                                                                     Qt::IgnoreAspectRatio, Qt::FastTransformation));
    ui->preview_label->adjustSize();
}

void MainWindow::open(){
    qDebug() << __FUNCTION__;
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::createActions(){
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    //QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &MainWindow::close);
    exitAct->setShortcut(tr("Ctrl+Q"));
}

void MainWindow::on_Image_checkBox_toggled(bool checked)
{
    if(checked){
        ui->pushButton_3->setEnabled(false);
        ui->stop_pushButton->setEnabled(false);
        myCamera->Stop();
    }else{
        ui->pushButton_3->setEnabled(true);
        ui->stop_pushButton->setEnabled(true);
        myCamera->Start();
    }
}


