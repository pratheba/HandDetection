TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt




INCLUDEPATH += /usr/include/ \
               /usr/local/include/ \
               /usr/include/qt5/ \

LIBS += -L/usr/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_objdetect \
    -lopencv_ml \
    -lopencv_legacy \
    -lopencv_flann \
    -lopencv_contrib \
    -lopencv_video \
    -lopencv_gpu \
    -lopencv_photo \
    -lQtCore \
    -lQtGui \
    -lQtWebKit

LIBS += -L/usr/lib/x86_64-linux-gnu \
    -lglut \
    -lGL    \
    -lGLU

SOURCES += main.cpp \
    FingerTipDetection/contourclass.cpp \
    FingerTipDetection/imagesegmentation.cpp \
    Camshift/videoprocessorclass.cpp \
    Camshift/motiondetectionclass.cpp \
    Camshift/maskforcamshift.cpp \
    Camshift/lkpyramid.cpp \
    Camshift/handdetectionclass.cpp \
    Camshift/haarcascaseclassifierclass.cpp \
    Camshift/facedetectionclass.cpp \
    Camshift/camshiftprocessing.cpp \
    Camshift/regionofinterest.cpp \



HEADERS += \
    Camshift/haarcascaseclassifierclass.h \
    Camshift/videoprocessorclass.h \
    Camshift/facedetectionclass.h \
    Camshift/motiondetectionclass.h \
    Camshift/camshiftprocessing.h \
    Camshift/maskforcamshift.h \
    Camshift/lkpyramid.h \
    Camshift/handdetectionclass.h \
    Camshift/regionofinterest.h \
    FingerTipDetection/contourclass.h \
    FingerTipDetection/imagesegmentation.h

OTHER_FILES += \
    images/lena.bmp

RESOURCES += \
    Resources.qrc \
    ImageResources.qrc

