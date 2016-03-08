#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
using namespace cv;
using namespace std;
vector<Point2f> mid(10);
bool xcompare(int a,int b)
{
	return (mid[a].x<mid[b].x);
}
bool ycompare(int a,int b)
{
	return (mid[a].y<mid[b].y);
}
bool compareContourArea ( vector<Point> contour1, vector<Point> contour2 ) 
{
    double i = fabs( contourArea(Mat(contour1),true) );
    double j = fabs( contourArea(Mat(contour2),true) );
    return ( i < j );
}
bool xcomparerect(Point2f a,Point2f b)
{
	return(a.x<b.x);
}
bool ycomparerect(Point2f a,Point2f b)
{
	return(a.y<b.y);
}

int findcolourhsv(float h,float s,float v)
{
	float e=0.0000001;
	if((fabs(h-0)<e)||((-s+80)>=e)) return 1;//white
	if(((h-23)<=0)&&((h-5)>=0)) return 2;//orange
	if(((h-38)<=0)&&((h-27)>=0)) return 3;//yellow
	if(((h-75)<=0)&&((h-60)>=0)) return 4;//green
	if(((h-130)<=0)&&((h-80)>=0)) return 5;//blue
	//if((((h-179)<=0)&&((h-160)>=0))||(fabs(h-0)<5)) return 6;//red
	if(((h-60)<=0)&&((h-14)>=0)) return 6;//red
	return -1;
}

int main(int argc,char** argv)
{
	RNG rng(12345);
	Mat src = imread(argv[1]);
	if (src.empty())
		return -1;
	Mat cdst=src.clone();
	Mat orig=src.clone();
	GaussianBlur(src, src, cv::Size(7,7), 0);
	imshow("img",src);
	Mat bw;
	std::vector<Mat> channels; 
	cvtColor(src,bw,CV_BGR2GRAY);
	cvtColor(orig,orig,CV_BGR2HSV);
		//thresholding
	threshold(bw,bw,70,255,THRESH_BINARY);
	//imshow("thresh",bw);
	//Canny(bw, bw, 100, 200, 3);
	//imshow("Canny",bw);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(bw,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	Mat drawing=Mat::zeros(src.size(),CV_8UC3);
	//Scalar color=Scalar(0,0,255);
	sort(contours.begin(), contours.end(), compareContourArea);
	 vector<Moments> mu(contours.size() );
  	for( int i =0; i < contours.size(); i++ )
     	mu[i] = moments( contours[i], false );
     vector<Point2f> center(contours.size() );
	for(int i=0;i<contours.size();i++)
	{
		center[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}
	for(int i=1;i<=9;i++)
	{
		mid[i].x=center[contours.size()-i].x;
		mid[i].y=center[contours.size()-i].y;
	}
	int index[10];
	for(int i=1;i<=9;i++) index[i]=i;
	sort(index+1,index+10,ycompare);
	sort(index+1,index+4,xcompare);
	sort(index+4,index+7,xcompare);
	sort(index+7,index+10,xcompare);

	/*for(int i=1;i<=9;i++)
	{
		cout<<index[i];
		cout<<" "<<center[contours.size()-index[i]].x<<" "<<center[contours.size()-index[i]].y<<"\n";
	}*/

	for(int i=1;i<=9;i++)
	{
		//cout<<index[i]<<"\n";
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours(drawing,contours,contours.size()-index[i],color,-1,8,hierarchy,0,Point());
	}

	imshow("contours",drawing);
	//imshow("original",orig);
	//cleaning cube
	vector<RotatedRect> rect(4);
	vector<Point2f> pt(4);
	rect[0]=minAreaRect(Mat(contours[contours.size()-index[1]]));
	rect[1]=minAreaRect(Mat(contours[contours.size()-index[3]]));
	rect[2]=minAreaRect(Mat(contours[contours.size()-index[7]]));
	rect[3]=minAreaRect(Mat(contours[contours.size()-index[9]]));
	for(int i=0;i<4;i++)
	{
		Point2f rectpoints[4];rect[i].points(rectpoints);
		sort(rectpoints,rectpoints+4,ycomparerect);
		sort(rectpoints,rectpoints+2,xcomparerect);
		sort(rectpoints+2,rectpoints+4,xcomparerect);
		pt[i].x=rectpoints[i].x;
		pt[i].y=rectpoints[i].y;
	}
	//for(int i=0;i<4;i++)
	//	cout<<pt[i].x<<" "<<pt[i].y<<"\n";

	Mat cleanimg = Mat::zeros(500,500 , CV_8UC3);

	//Finding the corresponding 4 coordinates in the final image to be displayed 
	vector<Point2f> quad_pts;
	quad_pts.push_back(Point(0, 0));
	quad_pts.push_back(Point(cleanimg.cols, 0));
	quad_pts.push_back(Point(0, cleanimg.rows));
	quad_pts.push_back(Point(cleanimg.cols, cleanimg.rows));
	Mat perspect=getPerspectiveTransform(pt,quad_pts);
	warpPerspective(cdst,cleanimg,perspect,cleanimg.size());
	//cvtColor(cleanimg,cleanimg,CV_BGR2HSV);
	imshow("cleanimg",cleanimg);
	//finding color
	/*bw=cleanimg.clone();
	cvtColor(bw,bw,CV_BGR2GRAY);
	threshold(bw,bw,70,255,THRESH_BINARY);
	findContours(bw,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	sort(contours.begin(),contours.end(),compareContourArea);
	Mat final=Mat::zeros(cleanimg.size(),CV_8UC3);
	for(int i=1;i<=9;i++)
	{
		//cout<<index[i]<<"\n";
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours(final,contours,contours.size()-i,color,-1,8,hierarchy,0,Point());
	}
	imshow("contours1",final);*/
	for(int i=1;i<=9;i++) {
		Rect r=boundingRect(contours[contours.size()-index[i]]);
		Scalar mymean=mean(orig(r));
		//cout<<" "<<center[contours.size()-index[i]].x<<" "<<center[contours.size()-index[i]].y<<"\n";
		//cout<<mymean.val[0]<<"\t"<<mymean.val[1];//<<"\t"<<mymean.val[2]<<"\n";//<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\n";
		int t=findcolourhsv(mymean.val[0],mymean.val[1],mymean.val[2]);
  			switch(t){
  				case 1: cout<<"White"<<"\t"; break;
  				case 2: cout<<"Orange"<<"\t"; break;
  				case 3: cout<<"Yellow"<<"\t"; break;
  				case 4: cout<<"Green"<<"\t"; break;
  				case 5: cout<<"Blue"<<"\t"; break;
  				case 6: cout<<"Red"<<"\t"; break;
  			}
  		if(i%3==0) cout<<"\n";
	}
	waitKey(0);
	return 0;
}