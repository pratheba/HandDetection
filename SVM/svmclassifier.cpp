#include "svmclassifier.h"

SVMClassifier::SVMClassifier()
{
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

}

SVMClassifier::~SVMClassifier() {

}


void SVMClassifier::TrainData(cv::Mat trainingDataMat, cv::Mat labelsMat) {
    SVM.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);

    std::cout<<"Training complete" << std::endl;
    std::cout<<"Number of support vectors in the SVM: " << SVM.get_support_vector_count() <<std::endl;
}

void SVMClassifier::ClassifyImage(cv::Mat inputImage) {

    float result = SVM.predict(inputImage);
    std::cout << result << std::endl;

}
