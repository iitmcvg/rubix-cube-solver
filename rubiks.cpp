#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;
int findcolourhsv(float h,float s,float v)
{
	float e=0.0000001;
	if((fabs(h-0)<e)&&(fabs(s-0)<e)) return 1;//white
	if(((h-22)<=0)&&((h-5)>=0)) return 2;//orange
	if(((h-38)<=0)&&((h-22)>=0)) return 3;//yellow
	if(((h-75)<=0)&&((h-38)>=0)) return 4;//green
	if(((h-130)<=0)&&((h-75)>=0)) return 5;//blue
	if((((h-179)<=0)&&((h-160)>=0))||(fabs(h-0)<5)) return 6;//red
	return -1;
}

bool compareContourLen ( vector<Point> contour1, vector<Point> contour2 ) 
{
    double i = fabs( arcLength(Mat(contour1),false) );
    double j = fabs( arcLength(Mat(contour2),false) );
    return ( i < j );
}
int main(int argc,char**argv)
{
	int i,t=0;
	Mat sub;
	//RNG rng(12345);
	Mat img = imread(argv[1]);
	if (img.empty())
		return -1;
	Mat orig=img.clone();
	cvtColor(orig,orig,CV_BGR2HSV);
	imshow("orig",img);
	cvtColor(img, img, CV_BGR2GRAY);
	//GaussianBlur(img, img, cv::Size(11, 11),2,2);
	Canny(img, img, 10, 100, 3);
	//imshow("canny",img);
	vector<vector<Point> > contours;
	//vector<vector<Point> > contour;
  	vector<Vec4i> hierarchy;
  	Mat test = Mat::zeros( img.size()/3, CV_8UC3 );
  	Mat drawing;
  	//cout<<img.rows<<"\t"<<img.cols<<"\n";
  	for(i=0;i<=(img.cols-img.cols/3);i+=img.cols/3)
  	{
  		for(int j=0;j<=(img.rows-(img.rows)/3);j+=img.rows/3)
  		{	//i++;
  			//t++;
  			//cout<<i<<j<<"\n";
  			sub=img(Rect(i,j,img.cols/3,img.rows/3));
  			test=orig(Rect(i,j,img.cols/3,img.rows/3));
  			imshow("sub",sub);
  			//waitKey(0);
  			findContours( sub, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  			sort(contours.begin(), contours.end(), compareContourLen);
  			//contour[t++]=contours[contours.size()-1];
  			drawing = Mat::zeros( img.size()/3, CV_8UC1 );
  			drawContours( drawing, contours, contours.size()-1,Scalar(t),CV_FILLED);
  			Rect r=boundingRect(contours[contours.size()-1]);
  			Scalar mymean=mean(test(r));
  			cout<<mymean.val[0]<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\n";//<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\n";
  			//t++;
  			t=findcolourhsv(mymean.val[0],mymean.val[1],mymean.val[2]);
  			cout<<t<<"\n";
  			//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      		//drawContours( drawing, contours, contours.size()-1, color, 2, 8, hierarchy, 0, Point() );
  			
  		}
  	}
  	//cout<<t<<"\n";
     namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  waitKey(0);
  return 0;
}
