#ifndef SVMCLASSIFIER_H
#define SVMCLASSIFIER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

class SVMClassifier
{
public:
    SVMClassifier();
    SVMClassifier(std::string TrainingSetImagelocation);

    ~SVMClassifier();

    void TrainData(cv::Mat TrainingDataMat, cv::Mat labelsMat);
    void ClassifyImage(cv::Mat inputImage);

private:
    cv::Mat TrainingSetTriangle ;
    cv::Mat TrainingSetCircle;
    cv::Mat TrainingSetNoClass;

    CvSVMParams params;
    CvSVM SVM;
};

#endif // SVMCLASSIFIER_H
