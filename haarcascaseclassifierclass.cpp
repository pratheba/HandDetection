#include "haarcascaseclassifierclass.h"
#include <QtCore/qdebug.h>
#include "qt5/QtCore/qdebug.h"

HaarCascaseClassifierClass::HaarCascaseClassifierClass() {
}

HaarCascaseClassifierClass::HaarCascaseClassifierClass(std::string cascadeXML_): cascadeXML(cascadeXML_) {
    loadCascadeClassifier();
}

HaarCascaseClassifierClass::~HaarCascaseClassifierClass() {
    itemsDetected.clear();
}

void HaarCascaseClassifierClass::loadCascadeClassifier() {
    if(cascadeXML.length() == 0) {
        std::cout << "No Classifier Loaded " << std::endl;
        return;
    }
    haarCascadeClassifier.load(cascadeXML);
}

void HaarCascaseClassifierClass::loadDetectedPointandWindowSize(cv::Mat& outputframe) {
    iter    =   itemsDetected.begin();
    iterEnd =   itemsDetected.end();
    cv::Rect item;
    //cv::Point2d XYlocation(-1,-1);
    int initialWidth    =   0;
    //PositionAndScale    posAndScale;

    for(; iter != iterEnd; iter++) {
        item    =   *iter;
        cv::rectangle(outputframe, item, CV_RGB(0, 255,0), 1);

        // Get the biggest rectangle identified
        if(item.size().width > initialWidth ) {
            posAndScale = item;//.SetValue(XYlocation(item.x, item.y), item.size());
            initialWidth = item.size().width;
        }
    }
    //posAndScale.push_back(posAndScale);
}

cv::Rect_<int> HaarCascaseClassifierClass::GetPositionAndScaleOfDetectedItem() {
    return posAndScale;
}

void HaarCascaseClassifierClass::reset() {
    //posAndScale.
    //processedFrame.release();
}

void HaarCascaseClassifierClass::ProcessFrame(cv::Mat& inputFrame, cv::Mat& outputframe) {
    processedFrame = inputFrame.clone();
    //loadCascadeClassifier();
    cv::cvtColor(inputFrame, processedFrame, CV_BGR2GRAY);
    haarCascadeClassifier.detectMultiScale(processedFrame, itemsDetected);
    loadDetectedPointandWindowSize(outputframe);
    processedFrame.release();
}


