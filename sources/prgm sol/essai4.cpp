#include <iostream>
#include "detectDiff.h"

int main( int argc, char** argv ) 
{
	//double coeff;
	cv::Mat img3;
	IplImage *img1,*img2,image;
	std::vector<cv::Mat> planes1,planes2;
	if(argc<2){
	std::cout<<"Usage: main <image-file-name>"<<std::endl;
	exit(0);
	}
	
	img1=cvLoadImage(argv[1]);
	img2=cvLoadImage(argv[2]);
	
	cv::split(img1,planes1);
	cv::split(img2,planes2);

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	
	//coeff=coeffDiffR(planes1[2],planes2[2]);
	//std::cout<<coeff<<std::endl;
	img3=contrastR2(.33*planes1[0]+.33*planes1[1]+.33*planes1[2],.33*planes2[0]+.33*planes2[1]+.33*planes2[2],1,1);
	

	renormF(img3,255);
	image=IplImage(img3);
	cvSaveImage("test10044.png",&image);
	
	//cvSaveImage("test10041.jpg",static_cast<CvArr*>(&img3));
	for (int k=0;k<3;k++)
	{
		planes1[k].release();
		planes2[k].release();
	}
}

