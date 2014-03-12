TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    haarcascaseclassifierclass.cpp \
    videoprocessorclass.cpp \
    facedetectionclass.cpp \
    motiondetectionclass.cpp \
    camshiftprocessing.cpp \
    maskforcamshift.cpp



INCLUDEPATH += /usr/include/ \
               /usr/local/include/ \
               /usr/include/qt5/

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

HEADERS += \
    haarcascaseclassifierclass.h \
    videoprocessorclass.h \
    facedetectionclass.h \
    motiondetectionclass.h \
    camshiftprocessing.h \
    maskforcamshift.h

OTHER_FILES += \
    images/lena.bmp

RESOURCES += \
    Resources.qrc

