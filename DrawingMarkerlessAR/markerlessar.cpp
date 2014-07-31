#include "markerlessar.h"


 MarkerLessAR::MarkerLessAR()
 {


  }

 MarkerLessAR::~MarkerLessAR() {
 }

int MarkerLessAR::startDrawing(int typeToProcess) {

    // Try to read the pattern:
    float distorsionCoeff[5] = {0.085271563843479975, -1.7576146699013042, 0., 0., 13.767884293894689};
    CameraCalibration calibration(718.96590212268165f, 718.96590212268165f, 319.50000000000000f, 239.50000000000000, distorsionCoeff);


    cv::VideoCapture imageframe = cv::VideoCapture(1);
    cv::Mat patternImage;
    imageframe >> patternImage;
    cv::imwrite("firstFrame.jpg",patternImage );

    //imageframe.release();

    switch(typeToProcess) {

    case 1:

        if (patternImage.empty())
        {
            std::cout << "Input image cannot be read" << std::endl;
        }
        else {
            //cv::VideoCapture cap = cv::VideoCapture(1);
            std::cout << patternImage.rows << "::" << patternImage.cols << std::endl;
            processVideo(patternImage, calibration, imageframe);
            break;
        }
    case 2:
        std::string input = "/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/DrawingMarkerlessAR/sample.jpg";
        cv::Mat testImage = cv::imread(input);
        if (!testImage.empty())
        {
            processSingleImage(patternImage, calibration, testImage);
        }
        else
        {
            cv::VideoCapture cap;
            if (cap.open(input))
            {
                processVideo(patternImage, calibration, cap);
            }
        }
        break;


    }

    return 0;
}

void MarkerLessAR::processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture)
{
    // Grab first frame to get the frame dimensions
    cv::Mat currentFrame;
    capture >> currentFrame;

    // Check the capture succeeded:
    if (currentFrame.empty())
    {
        std::cout << "Cannot open video capture device" << std::endl;
        return;
    }

    cv::Size frameSize(currentFrame.cols, currentFrame.rows);

    ARPipeline pipeline(patternImage, calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        capture >> currentFrame;
        if (currentFrame.empty())
        {
            shouldQuit = true;
            continue;
        }

        shouldQuit = processFrame(currentFrame, pipeline, drawingCtx);
    } while (!shouldQuit);
}

void MarkerLessAR::processSingleImage(const cv::Mat& patternImage, CameraCalibration& calibration, const cv::Mat& image)
{
    cv::Size frameSize(image.cols, image.rows);
    ARPipeline pipeline(patternImage, calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        shouldQuit = processFrame(image, pipeline, drawingCtx);
    } while (!shouldQuit);
}

bool MarkerLessAR::processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx)
{
    // Clone image used for background (we will draw overlay on it)
    cv::Mat img = cameraFrame.clone();

    // Draw information:
    if (pipeline.m_patternDetector.enableHomographyRefinement)
        cv::putText(img, "Pose refinement: On   ('h' to switch off)", cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));
    else
        cv::putText(img, "Pose refinement: Off  ('h' to switch on)",  cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    cv::putText(img, "RANSAC threshold: " + ToString(pipeline.m_patternDetector.homographyReprojectionThreshold) + "( Use'-'/'+' to adjust)", cv::Point(10, 30), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    // Set a new camera frame:
    drawingCtx.updateBackground(img);

    // Find a pattern and update it's detection status:
    drawingCtx.isPatternPresent = pipeline.processFrame(cameraFrame);

    // Update a pattern pose:
    drawingCtx.patternPose = pipeline.getPatternLocation();

    // Request redraw of the window:
    drawingCtx.updateWindow();

    // Read the keyboard input:
    int keyCode = cv::waitKey(5);

    bool shouldQuit = false;
    if (keyCode == '+' || keyCode == '=')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold += 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::min(10.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == '-')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold -= 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::max(0.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == 'h')
    {
        pipeline.m_patternDetector.enableHomographyRefinement = !pipeline.m_patternDetector.enableHomographyRefinement;
    }
    else if (keyCode == 27 || keyCode == 'q')
    {
        shouldQuit = true;
    }

    return shouldQuit;
}


