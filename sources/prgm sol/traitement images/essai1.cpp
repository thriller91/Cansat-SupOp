#include "highgui.h"
#include "cxcore.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main( int argc, char** argv ) 
{
	int rows,cols;
	cv::Mat img3;
	IplImage *img1,*img2,image;
	std::vector<Mat> planes1,planes2;
	if(argc<2){
	std::cout<<"Usage: main <image-file-name>"<<std::endl;
	exit(0);
	}
	img1=cvLoadImage(argv[1]);
	img2=cvLoadImage(argv[2]);
	/*if(!img1){
	std::cout<<"Could not load image file:"<<argv[1])<<std::endl;
	exit(0);
	}
	if(!img2){
	std::cout<<"Could not load image file:"<<argv[2])<<std::endl;
	exit(0);
	}*/
	
	split(img1,planes1);
	split(img2,planes2);
	
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	
	
	cols=planes1[2].cols;
	rows=planes1[2].rows;
	
	if ((planes2[2].cols!=cols)|(planes2[2].rows!=rows)){
	std::cout<<"Images should have the same size"<<std::endl;
	}
	
	img3=1.1*planes1[2]-1.09*planes2[2];
	
	for (int k=0;k<3;k++)
	{
		planes1[k].release();
		planes2[k].release();
	}
	
	/*img3=Mat(rows,cols,CV_8UC1);
	
	MatIterator_<uchar> it1 = planes1[2].begin<uchar>(),it_end1 = planes1[2].end<uchar>();
	MatIterator_<uchar> it2 = planes2[2].begin<uchar>(),it_end2 = planes2[2].end<uchar>();
	MatIterator_<uchar> it3 = img3.begin<uchar>(),it_end3 = img3.end<uchar>();
	
	
	for(; it1 != it_end1; ++it1)
	{
		if (*it1>*it2)
		{
			*it3=*it1-*it2;
		}
		else
		{
			*it3=0;
		}
		++it2;
		++it3;
	}*/
	image=IplImage(img3);
	//cvShowImage("test",&image);
	cvSaveImage("test10039.jpg",&image);
}

