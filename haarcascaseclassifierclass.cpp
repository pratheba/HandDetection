#include "haarcascaseclassifierclass.h"
#include <QtCore/qdebug.h>
#include "qt5/QtCore/qdebug.h"

HaarCascaseClassifierClass::HaarCascaseClassifierClass() {
}

HaarCascaseClassifierClass::HaarCascaseClassifierClass(std::string cascadeXML_): cascadeXML(cascadeXML_) {
    loadCascadeClassifier();
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

    for(; iter != iterEnd; iter++) {
        cv::Rect item    =   *iter;
        cv::rectangle(outputframe, item, CV_RGB(0, 255,0), 1);
        posAndScale.push_back(PositionAndScale(cv::Point2d(item.x, item.y), item.size()));
    }
}
std::vector<PositionAndScale> HaarCascaseClassifierClass::GetPositionAndScaleOfDetectedItem() {
    return posAndScale;
}

void HaarCascaseClassifierClass::reset() {
    posAndScale.clear();
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


