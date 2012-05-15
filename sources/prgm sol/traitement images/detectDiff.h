#ifndef DETECT_DIFF_H_INCLUDED
#define DETECT_DIFF_H_INCLUDED
#include "highgui.h"
#include "cxcore.h"
#include <iostream>
#include <vector>

cv::Mat diffR(const cv::Mat img1,const cv::Mat img2,double coeff1,double coeff2);
cv::Mat contrastR(cv::Mat img1,cv::Mat img2,double coeff1,double coeff2);
double coeffDiffR(cv::Mat img1,cv::Mat img2);
double coeffDiffR2(cv::Mat img1,cv::Mat img2);
void renorm(cv::Mat img,double nv_max);

#include "detectDiff.cpp"
#endif

