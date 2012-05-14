//fait une différence ponderee entre deux matrices opencv.
//ENTREES:
//img1 : cv::Mat, premiere matrice
//img2 : cv:Mat, deuxieme matrice
//coeff1 : coefficient de img1
//coeff2 : coefficient de img2
//SORTIES:
//retourne : cv::Mat, difference des deux matrices
cv::Mat diffR(const cv::Mat img1,const cv::Mat img2,double coeff1,double coeff2)
{
	cv::Mat img3;
	
	img3=coeff1*img1-coeff2*img2;
	
	return img3;
}

cv::Mat contrastR(cv::Mat img1,cv::Mat img2,double coeff1,double coeff2)
{
	cv::Mat img3(img1.rows,img1.cols,CV_32FC1);
	float a,b,min=10000;
	
	//cv::Mat img3;
	
	cv::MatIterator_<uchar> it1 = img1.begin<uchar>(),it_end1 = img1.end<uchar>();
	cv::MatIterator_<uchar> it2 = img2.begin<uchar>(),it_end2 = img2.end<uchar>();
	cv::MatIterator_<float> it3 = img3.begin<float>(),it_end3 = img3.end<float>();
	
	for(; it1 != it_end1; ++it1)
	{
		a=coeff1*(*it1)-coeff2*(*it2);
		b=coeff1*(*it1)+coeff2*(*it2);
		if (b<10)
		{
			*it3=0;
		}
		else
		{
			*it3=510*a/b;
		}
		if (*it3<min) min=*it3;
		it2++;it3++;
	}
	std::cout<<min<<std::endl;
	it3 = img3.begin<float>();
	it_end3 = img3.end<float>();
	
	for(; it3 != it_end3; ++it3)
	{
		*it3=*it3-min;
	}
	
	return img3;
}

cv::Mat contrastR2(cv::Mat img1,cv::Mat img2,double coeff1,double coeff2)
{
	cv::Mat img3(img1.rows,img1.cols,CV_32FC1);
	float a,b,min=10000;
	
	cv::MatIterator_<uchar> it1 = img1.begin<uchar>(),it_end1 = img1.end<uchar>();
	cv::MatIterator_<uchar> it2 = img2.begin<uchar>(),it_end2 = img2.end<uchar>();
	cv::MatIterator_<float> it3 = img3.begin<float>(),it_end3 = img3.end<float>();
	
	for(; it1 != it_end1; ++it1)
	{
		a=2*coeff1*(*it1)-coeff2*(*it2);
		b=coeff1*(*it2)+5;
		if (b<1)
		{
			*it3=510;
		}
		else
		{
			*it3=510*a/b;
		}
		if (*it3<min) min=*it3;
		it2++;it3++;
	}
	std::cout<<min<<std::endl;
	it3 = img3.begin<float>();
	it_end3 = img3.end<float>();
	
	for(; it3 != it_end3; ++it3)
	{
		*it3=*it3-min;
	}
	
	return img3;
}


//calcule le coefficient de img2 qui minimise la norme 2 de la différence de img1 et img2
//lorsque le coefficient de img1 est 1.
//ENTREES:
//img1 : cv::Mat
//img1 : cv::Mat
//SORTIES:
//retourne le coefficient de img2
double coeffDiffR(cv::Mat img1,cv::Mat img2)
{
	double coeff;
	float sumsq2=0;
	float sum12=0;
	
	cv::MatIterator_<uchar> it1 = img1.begin<uchar>(),it_end1 = img1.end<uchar>();
	cv::MatIterator_<uchar> it2 = img2.begin<uchar>(),it_end2 = img2.end<uchar>();
	
	for(; it1 != it_end1; ++it1)
	{
		sumsq2+=(*it2)*(*it2);
		sum12+=(*it1)*(*it2);		
		it2++;
	}
	
	std::cout<<sum12<<"	"<<sumsq2<<std::endl;
	
	coeff=sum12/sumsq2;
	
	return coeff;
}

//renormalise img tel que son maximum soit nv_max
void renormU(cv::Mat img,double nv_max)
{
	double max=0,mult=1;
	cv::MatIterator_<uchar> it = img.begin<uchar>(),it_end = img.end<uchar>();
	
	for(; it != it_end; ++it)
	{
		if (*it>max) max=*it;
	}
	mult=nv_max/max;
	it = img.begin<uchar>();
	it_end = img.end<uchar>();
	for(; it != it_end; ++it)
	{
		*it=*it*mult;
	}
}

//renormalise img tel que son maximum soit nv_max
void renormF(cv::Mat img,double nv_max)
{
	double max=0,mult=1;
	cv::MatIterator_<float> it = img.begin<float>(),it_end = img.end<float>();
	
	for(; it != it_end; ++it)
	{
		if (*it>max) max=*it;
	}
	mult=nv_max/max;
	it = img.begin<float>();
	it_end = img.end<float>();
	for(; it != it_end; ++it)
	{
		*it=*it*mult;
	}
}

