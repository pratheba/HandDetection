/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */


#include <iostream>
#include "videoprocessorclass.h"
#include "camshiftprocessing.h"

int main(int argc, const char *argv[]) {

   /* if(argc < 2) {
        std::cout << " Input Arguments required " << std::endl;
        return -1;
    }*/

    //std::cout<< argv[0] << std::endl;
    VideoProcessorClass* videoProcessor  =  VideoProcessorClass::getInstance(atoi(argv[1]));// new VideoProcessorClass(atoi(argv[1]));
    CamshiftProcessing* camshift = new CamshiftProcessing();


    /* Get the VideoProcessing */
    videoProcessor->SetVideoCaptureInstance();
    //videoProcessor->ChooseProcessorClass(atoi(argv[1]));
    videoProcessor->SetframeToStopProcessing(10);
    videoProcessor->displayInput("Input Frame");
    videoProcessor->displayOutput("Output Frame");
    videoProcessor->ProcessVideoFrame();

    /* Get the color Probability mask */
    //camshift->GetColorProbabilityMask();
    camshift->GetOpticalFlow();

    videoProcessor->release();
    delete camshift;

    return 0;
}
