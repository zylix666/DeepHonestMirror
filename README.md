# DeepHonestMirror
This is an application showing how to use openCV, Caffe, Qt to detect face and recognize age and gender.

# Installation guide
## Dependencies
   1. Caffe. https://github.com/BVLC/caffe
      My operating system is Ubuntu 16.04. Please see Caffe installation instruction. http://caffe.berkeleyvision.org/installation.html
   2. OpenCV 2.4. 
      You can use Ubuntu apt to install OpenCV libraries.
   3. Qt 5.8 or Qt 5.7.1
      You can download Qt from https://www.qt.io/download/
      This project is built with Qt Creator

## Model download.
* This project is inspired by "Age and Gender Classification using Convolutional Neural Networks". I cannot make it without the models. http://www.openu.ac.il/home/hassner/projects/cnn_agegender/
* This is a very interested research. I strongly recommend you to read through the paper.

I did not retrain the model. Just apply the pre-built model.
If you are interested in retraining the model to improve it, the author also provide the training script. https://github.com/GilLevi/AgeGenderDeepLearning

* To download the pre-built model, you can visit the Gist page. /home/simonli/build-DeepHonestMirror-Desktop_Qt_5_8_0_GCC_64bit-Debug/models

## The model folder.
You should download the model files and put them into the build folder like below:

build-DeepHonestMirror-Desktop_Qt_5_8_0_GCC_64bit-Debug
├── classifier.o
├── cvcamera.o
├── DeepHonestMirror
├── facedetector.o
├── main.o
├── mainwindow.o
├── Makefile
├── moc_cvcamera.cpp
├── moc_cvcamera.o
├── moc_mainwindow.cpp
├── moc_mainwindow.o
├── moc_predefs.h
├── moc_roi_analyzer.cpp
├── moc_roi_analyzer.o
├── models
│   ├── age
│   │   ├── age_net.caffemodel
│   │   ├── deploy_age.prototxt
│   │   ├── mean.binaryproto
│   │   └── taglist.txt
│   └── gender
│       ├── deploy_gender.prototxt
│       ├── gender_net.caffemodel
│       ├── mean.binaryproto
│       └── taglist.txt
├── roi_analyzer.o
└── ui_mainwindow.h
![alt text](https://github.com/zylix666/DeepHonestMirror/blob/master/2017-06-20%2023-46-02filestructure.png)
## Execution result.
https://goo.gl/photos/PKg3pbNnuBkuXhxW9

#TODO
There are still many bugs of this application. Will fix them in the future.
