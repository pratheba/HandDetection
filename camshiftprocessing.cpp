#include "camshiftprocessing.h"

using namespace std;
using namespace cv;

Mat image;

bool backprojMode = false;
bool selectObject = false;
bool flowMode = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;



CamshiftProcessing::CamshiftProcessing()
{
    videoProcessorClass = VideoProcessorClass::getInstance();
    lkPyramidClass      = new LKPyramid();
    motionMask = cv::Mat();
    cv::namedWindow("ROI",0);
}


void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessorClass->facedetectionClass != NULL) {
        HSVmaskValue = videoProcessorClass->facedetectionClass->GetHSVmaskForFace();
    }
    else {
        std::cout << "No processing has been done to get the HSV value"  << std::endl;
        return;
    }
}


cv::Mat CamshiftProcessing::GetOpticalFlow() {
    if(motionMask.empty())
        motionMask = cv::Mat(CurrentFrame.rows, CurrentFrame.cols, CV_8U, cv::Scalar(1.0));
    motionMask = cv::Scalar(1.0);
    lkPyramidClass->CalculateOpticalFlow(CurrentFrame.clone());
    motionMask = lkPyramidClass->GetTheMotionMask();
    return motionMask;
}



static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
        break;
    }
}

static void help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./camshiftdemo [camera number]\n";

    cout << "\n\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tc - stop the tracking\n"
            "\tb - switch to/from backprojection view\n"
            "\th - show/hide object histogram\n"
            "\tp - pause video\n"
            "To initialize tracking, select the object with mouse\n";
}



bool CamshiftProcessing::IsBothMatrixOfSameSize(cv::Mat& inputMat1, cv::Mat& inputMat2) {
    if((inputMat1.rows == inputMat2.rows && inputMat1.cols == inputMat2.cols) &&
            (!inputMat1.empty() && !inputMat2.empty()))
        return true;
    return false;
}


cv::Point2i CamshiftProcessing::GetStepSizeForROIselection(int rowWidth, int colWidth) {

    cv::Point2i stepSize;
    if(rowWidth != 0 && colWidth !=0) {
        stepSize.y = CurrentFrame.rows / rowWidth;
        stepSize.x = CurrentFrame.cols / colWidth;
    }

    return stepSize;
}


cv::Rect CamshiftProcessing::GetROIForProcessing(int row, int col, int height, int width) {

    cv::Rect ROIRectangle;
    ROIRectangle.x = MIN(col, col+width);
    ROIRectangle.y = MIN(row, row+height);
    ROIRectangle.width = width;
    ROIRectangle.height = height;

   // std::cout << ROIRectangle.x << "::" << ROIRectangle.y << "::" << ROIRectangle.width << "::" << ROIRectangle.height << std::endl;

    //ROIRectangle &= cv::Rect(0,0,motionMask.cols, motionMask.rows);

    return ROIRectangle;
}


void CamshiftProcessing::SelectRegionOfInterest() {

    //cv::destroyAllWindows();


    if(!IsBothMatrixOfSameSize(CurrentFrame, motionMask))
        return;
    cv::Point2i stepSize = GetStepSizeForROIselection(50,50);

    cv::Rect ROIRectangle;
    cv::Mat RegionOfInterest;
    cv::Scalar averagePixelIntensity(255,255,255);
    int finalRow = -1;
    int finalCol = -1;

    for(int row =   0; row < CurrentFrame.rows; row = row + stepSize.y) {
        for(int col  =    0; col < CurrentFrame.cols; col = col + stepSize.x) {

            if(row+stepSize.y < motionMask.rows && col+stepSize.x < motionMask.cols) {
                ROIRectangle = GetROIForProcessing(row, col, stepSize.y, stepSize.x);

                //std::cout << motionMask.at<cv::Vec3b>(row+stepSize.y, col+stepSize.x) << std::endl;
                 RegionOfInterest = cv::Mat(motionMask, ROIRectangle);

                //cv::cvtColor(RegionOfInterest, RegionOfInterest, CV_BGR2GRAY);
                cv::Scalar avg = cv::mean(RegionOfInterest);

                if(avg.val[0] < averagePixelIntensity[0]) {
                    averagePixelIntensity = cv::mean(RegionOfInterest);
                    //selectionRegion = CurrentFrame(cv::Rect(row, col, row+rowStepSize, col+colStepSize));
                    finalRow = row;
                    finalCol = col;
                    //std::cout << row << "::" << col << std::endl;
                }
            }
        }
    }

    //std::cout << finalRow << "::" << finalCol << std::endl;

        cv::rectangle(CurrentFrame, cv::Point(finalCol, finalRow), cv::Point(finalCol + 50, finalRow + 50), CV_RGB(255,0,0),2, CV_AA);
        cv::imshow("ROI", CurrentFrame);

}

void CamshiftProcessing::TrackRegionOfInterest() {
    lkPyramidClass->SetCurrentFrameWindow("current frame optical window");
    lkPyramidClass->SetPrevFrameWindow("previous frame optical flow window");
    lkPyramidClass->SetOpticalFlowWindow("optical flow window");

    /*
    int iter = 0;

    while(true) {
    videoProcessorClass->capture.read(CurrentFrame);
    GetOpticalFlow();
    normalize(motionMask, motionMask, 0, 255, NORM_MINMAX);


    //SelectRegionOfInterest();
    char c = (char)waitKey(10);
    if( c == 27 )
        break;
    }
    */


   GetColorProbabilityMask();
    Rect trackWindow;
    int hsize = 180;
    float hranges[] = {0,180};
    const float* phranges = hranges;


    if( !videoProcessorClass->capture.isOpened() )
    {
        help();
        cout << "***Could not initialize capturing...***\n";
        cout << "Current parameter's value: \n";
        //parser.printMessage();
        return;
    }

    namedWindow( "Histogram", 0 );
    namedWindow( "CamShift Demo", 0 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

    Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool paused = false;
      cv::Mat feature = cv::Mat();

      cv::Mat element5(5,5, CV_8U, cv::Scalar(1));


    for(;;)
    {
        if( !paused )
        {
            videoProcessorClass->capture.read(frame);
            if( frame.empty() )
                break;
        }

        frame.copyTo(image);

        if( !paused )
        {
            CurrentFrame = frame.clone();

            cvtColor(image, hsv, COLOR_BGR2HSV);

            if( trackObject )
            {normalize(motionMask, motionMask, 0, 255, NORM_MINMAX);
                int _vmin = vmin, _vmax = vmax;

                //inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                  //      Scalar(180, 256, MAX(_vmin, _vmax)), mask);

                inRange(hsv, Scalar(HSVmaskValue.minhue, HSVmaskValue.minSat, HSVmaskValue.minValue),
                        Scalar(HSVmaskValue.maxhue, HSVmaskValue.maxSat, HSVmaskValue.maxvalue), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);

                if( trackObject < 0 )
                {
                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                    normalize(hist, hist, 0, 255, NORM_MINMAX);

                    trackWindow = selection;
                    trackObject = 1;

                    histimg = Scalar::all(0);
                    int binW = histimg.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, COLOR_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                        rectangle( histimg, Point(i*binW,histimg.rows),
                                   Point((i+1)*binW,histimg.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }

                calcBackProject(&hue, 1, 0, hist, backproj, &phranges);

                //cout << "motionMask" << std::endl << motionMask << std::endl << std::endl;
                backproj &= mask;


                if(feature.empty())
                    feature = cv::Mat(CurrentFrame.rows, CurrentFrame.cols, CV_8U);//0.7* backproj + 0.3 * motionMask;

                if(flowMode) {
                    GetOpticalFlow();
                    if(motionMask.rows != 0 && motionMask.cols !=0 ) {
                    std::cout << "with motion flow" << std::endl;
                    //normalize(motionMask, motionMask, 0, 255, NORM_MINMAX);
                    //cout << "motionMask" << std::endl << backproj << std::endl << std::endl;
                    cv::addWeighted(backproj, 0.4, motionMask, 0.9, 0.0, feature);
                    //feature = motionMask.clone();
                    //cv::addWeighted(backproj, 1.0, motionMask, 0.8, 0.0, feature);
                    }
                    else {
                        std::cout << "no motion mask available" <<std::endl;
                    }
                    }
                else {
                    std::cout << "with color distribution" << std::endl;
                    feature = backproj.clone();
                }

                cv::morphologyEx(feature, feature, cv::MORPH_CLOSE, element5);

                RotatedRect trackBox = CamShift(feature, trackWindow,
                                    TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                if( trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                                  Rect(0, 0, cols, rows);
                }

               // if( backprojMode )
                   // cvtColor( backproj, image, COLOR_GRAY2BGR );
                if( backprojMode )
                    cvtColor( feature, image, COLOR_GRAY2BGR );
                ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
            }
        }
        else if( trackObject < 0 )
            paused = false;

        if( selectObject && selection.width > 0 && selection.height > 0 )
        {
            Mat roi(image, selection);
            bitwise_not(roi, roi);
        }

        imshow( "CamShift Demo", image );
        imshow( "Histogram", histimg );


        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;
        case 'c':
            trackObject = 0;
            histimg = Scalar::all(0);
            break;
        case 'h':
            showHist = !showHist;
            if( !showHist )
                destroyWindow( "Histogram" );
            else
                namedWindow( "Histogram", 1 );
            break;
        case 'p':
            paused = !paused;
            break;
        case 'f':
            flowMode = !flowMode;
            break;
        default:
            ;
        }
    }

}



