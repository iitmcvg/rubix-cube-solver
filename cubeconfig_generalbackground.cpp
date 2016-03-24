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
bool xcomparerect(Point2f a,Point2f b)
{
	return(a.x<b.x);
	}
bool ycomparerect(Point2f a,Point2f b)
{
	return(a.y<b.y);
}
bool compareArea(vector<Point> contour1,vector<Point> contour2)
{
	double i = fabs( contourArea(Mat(contour1),true) );
    double j = fabs( contourArea(Mat(contour2),true) );
    return ( i > j );
}
int findcolourbgr(float b,float g,float r)
{
	if(((b-150)>=0&&(b-255)<=0)&&((g-150)>=0&&(g-255)<=0)&&((r-150)>=0&&(r-255)<=0)) return 1;
	if((b>=0&&(b-39)<=0)&&((g-70)>=0&&(g-148)<=0)&&((r-200)>=0&&(r-255)<=0)) return 2;
	if((b>=0&&(b-39)<=0)&&((g-170)>=0&&(g-255)<=0)&&((r-180)>=0&&(r-255)<=0)) return 3;
	if((b>=0&&(b-46)<=0)&&((g-130)>=0&&(g-220)<=0)&&(r>=0&&(r-41)<=0)) return 4;
	if(((b-170)>=0&&(b-255)<=0)&&((g-75)>=0&&(g-180)<=0)&&(r>=0&&(r-45)<=0)) return 5;
	if(((b-5)>=0&&(b-70)<=0)&&((g-5)>=0&&(g-75)<=0)&&((r-125)>=0&&(r-255)<=0)) return 6;
	
}
int main(int argc,char** argv)
{
	if(argc!=7)
	{ 	
		cout<<"\nPut 6 images";return -1;
	}
	char str[54],s[10];
	str[0]='\0';
	for(int k=1;k<=6;k++) 
	{
	Mat src=imread(argv[k],1); 
	Size size(500,500);
	resize(src,src,size);
	Mat bw,cdst;
	Mat orig=src.clone();
	cdst=src.clone();
	int squares[9],j=0,t=0;
	double area=0,prevarea=0;
	vector<Mat> channels;
	GaussianBlur(src, src, cv::Size(7,7), 0);
		cvtColor(src,bw,CV_BGR2GRAY);
		cvtColor(src,src,CV_BGR2HSV);
		split(src,channels);
		//Mat h=channels[0];
		//Mat s=channels[1];
		Mat v=channels[2];
		//imshow("hue",h);
		//imshow("saturation",s);
		//imshow("value",v);
		//thresholding
		threshold(v,bw,0,255,THRESH_BINARY|THRESH_OTSU);
vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(bw,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
		sort(contours.begin(),contours.end(),compareArea);
		vector<Moments> mu(contours.size() );
  		for( int i =0; i < contours.size(); i++ )
     		mu[i] = moments( contours[i], false );
     	vector<Point2f> center(contours.size() );
		for(int i=0;i<contours.size();i++)
		{
			center[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		}
Mat drawing=Mat::zeros(src.size(),CV_8UC3);
		RNG rng(12345);
		for(int i=0;i<contours.size();i++)
		{
			approxPolyDP(contours[i],contours[i],arcLength(contours[i],true)*0.1,true);
			//cout<<index[i]<<"\n";
			if((contours[i].size()==4)&&fabs(contourArea(Mat(contours[i],true)))>(0.008*src.rows*src.cols)) {
				//cout<<fabs(contourArea(Mat(contours[i],true)))<<"\t";
				area=fabs(contourArea(Mat(contours[i]),true) );
				if(fabs(t==0||area-prevarea)<100){ squares[t++]=i;}
				else{ if(t>=9) break; else t=0;}
				prevarea=area;
				//rect[j++]=i;
			}
		}
		for(int i=0;i<t;i++)
		{
			mid[i].x=center[squares[i]].x;
			mid[i].y=center[squares[i]].y;
		}
		//cout<<t<<"\n";
		int index[20],temp,save,ct;
		for(int i=0;i<t;i++) index[i]=i;
		sort(index,index+t,xcompare);
		ct=0;
		temp=t;
		for(int i=0;i<(t-1);i++)
		{
			if(fabs(center[squares[index[i]]].x-center[squares[index[i+1]]].x)<5) {if(ct==0) save=i;ct++;}
			else{ if(ct<2) {for(int j=save;j<(t-ct);j++) index[j]=index[ct+j]; temp-=ct;}ct=0;}
		}
		if(ct<2) {temp-=(ct+1);}
		t=temp;
		sort(index,index+t,ycompare);
		ct=0;
		temp=t;
		for(int i=0;i<(t-1);i++)
		{
			if(fabs(center[squares[index[i]]].y-center[squares[index[i+1]]].y)<5) {if(ct==0) save=i;ct++;}
			else{ if(ct<2) {for(int j=save;j<(t-ct);j++) index[j]=index[ct+j]; temp-=ct;}ct=0;}
		}
		if(ct<2) {temp-=(ct+1);}
		t=temp;
		//cout<<t<<"\n";
		sort(index,index+9,ycompare);
		sort(index,index+3,xcompare);
		sort(index+3,index+6,xcompare);
		sort(index+6,index+9,xcompare);

		vector<RotatedRect> rect(4);
		vector<Point2f> pt(4);
		rect[0]=minAreaRect(Mat(contours[squares[index[0]]]));
		rect[1]=minAreaRect(Mat(contours[squares[index[2]]]));
		rect[2]=minAreaRect(Mat(contours[squares[index[6]]]));
		rect[3]=minAreaRect(Mat(contours[squares[index[8]]]));
		for(int i=0;i<4;i++)
		{
			Point2f rectpoints[4];rect[i].points(rectpoints);
			sort(rectpoints,rectpoints+4,ycomparerect);
			sort(rectpoints,rectpoints+2,xcomparerect);
			sort(rectpoints+2,rectpoints+4,xcomparerect);
			pt[i].x=rectpoints[i].x;
			pt[i].y=rectpoints[i].y;
		}

		Mat cleanimg = Mat::zeros(500,500 , CV_8UC3);

		//Finding the corresponding 4 coordinates in the cleaned image
		vector<Point2f> quad_pts;
		quad_pts.push_back(Point(0, 0));
		quad_pts.push_back(Point(cleanimg.cols, 0));
		quad_pts.push_back(Point(0, cleanimg.rows));
		quad_pts.push_back(Point(cleanimg.cols, cleanimg.rows));
		Mat perspect=getPerspectiveTransform(pt,quad_pts);
		warpPerspective(cdst,cleanimg,perspect,cleanimg.size());

		for(int i=0;i<9;i++) {
			Rect r=boundingRect(contours[squares[index[i]]]);
			Scalar mymean=mean(orig(r));
			//cout<<" "<<center[contours.size()-index[i]].x<<" "<<center[contours.size()-index[i]].y<<"\n";
			//cout<<mymean.val[0]<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\t";//<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\n";
			int t=findcolourbgr(mymean.val[0],mymean.val[1],mymean.val[2]);
  			switch(t){
  				case 1: //cout<<"White"<<"\t"; 
  						s[i]='W';break;
  				case 2: //cout<<"Orange"<<"\t"; 
  						s[i]='O';break;
  				case 3: //cout<<"Yellow"<<"\t";
  						s[i]='Y';break;
  				case 4: //cout<<"Green"<<"\t"; 
  						s[i]='G';break;
  				case 5: //cout<<"Blue"<<"\t"; 
  						s[i]='B';break;
  				case 6: //cout<<"Red"<<"\t"; 
  						s[i]='R';break;
  			}
  			}
		s[9]='\0';
		strcat(str,s);
		//cout<<s<<"\n";
		/*for(int i=0;i<t;i++)
		{
			cout<<center[squares[index[i]]].x<<" "<<center[squares[index[i]]].y<<"\n";
			//cout<<squares[i];
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours(drawing,contours,squares[index[i]],color,-1,8,hierarchy,0,Point());
		}
		imshow("drawing",drawing);*/
	}
	ofstream fout("input.txt",ios::out);
		fout<<str;
		fout<<"\0";
		fout.close();
			
		waitKey(0);
		return 0;
			
	}