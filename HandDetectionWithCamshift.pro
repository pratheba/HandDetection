TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


include(VideoProcessing.pri)
include(FaceDetection.pri)
include(LucasPyramid-opticalFlow.pri)
include(CamShift.pri)


INCLUDEPATH += /usr/include/ \
               /usr/local/include/ \
               /usr/include/qt5/ \
               /usr/share/doc/ \

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
    -lQtWebKit \
    -ltbb \

LIBS += -L/usr/lib/x86_64-linux-gnu \
    -lglut \
    -lGL    \
    -lGLU



SOURCES += main.cpp \
    FingerTipDetection/contourclass.cpp \
    FingerTipDetection/imagesegmentation.cpp \


HEADERS += \
    FingerTipDetection/contourclass.h \
    FingerTipDetection/imagesegmentation.h \


OTHER_FILES += \
    images/lena.bmp \
    VideoProcessing.pri \
    FaceDetection.pri \
    LucasPyramid-opticalFlow.pri \
    CamShift.pri

RESOURCES += \
    Resources.qrc \
    ImageResources.qrc

