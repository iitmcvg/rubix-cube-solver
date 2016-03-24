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

int findcolourbgr(float b,float g,float r)
{
	if(((b-150)>=0&&(b-255)<=0)&&((g-150)>=0&&(g-255)<=0)&&((r-150)>=0&&(r-255)<=0)) return 1;
	if((b>=0&&(b-39)<=0)&&((g-70)>=0&&(g-148)<=0)&&((r-200)>=0&&(r-255)<=0)) return 2;
	if((b>=0&&(b-39)<=0)&&((g-170)>=0&&(g-255)<=0)&&((r-180)>=0&&(r-255)<=0)) return 3;
	if((b>=0&&(b-46)<=0)&&((g-130)>=0&&(g-220)<=0)&&(r>=0&&(r-41)<=0)) return 4;
	if(((b-170)>=0&&(b-255)<=0)&&((g-75)>=0&&(g-180)<=0)&&(r>=0&&(r-45)<=0)) return 5;
	if(((b-5)>=0&&(b-70)<=0)&&((g-5)>=0&&(g-75)<=0)&&((r-125)>=0&&(r-255)<=0)) return 6;
	
}
/*int findcolourhsv(float h,float s,float v)
{
	float e=0.0000001;
	if((fabs(h-0)<e)||((-s+70)>=e)) return 1;//white
	if(((h-23)<=0)&&((h-5)>=0)) return 2;//orange
	if(((h-38)<=0)&&((h-27)>=0)) return 3;//yellow
	if(((h-75)<=0)&&((h-60)>=0)) return 4;//green
	if(((h-130)<=0)&&((h-80)>=0)) return 5;//blue
	//if((((h-179)<=0)&&((h-160)>=0))||(fabs(h-0)<5)) return 6;//red
	if(((h-60)<=0)&&((h-14)>=0)) return 6;//red
	return -1;
}*/
int main(int argc,char** argv)
{
	if(argc!=7)
	{ 	
		cout<<"\nPut 6 images";return -1;
	}
	Mat src,orig,cdst,bw;
	vector<Mat> channels;
	char str[54],s[10];
	str[0]='\0';
	for(int j=1;j<=6;j++){
		src = imread(argv[j]);
		if (src.empty())
			return -1;
		cdst=src.clone();
		orig=src.clone();
		GaussianBlur(src, src, cv::Size(7,7), 0);
		//imshow("blurimg",src); 
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
		//imshow("threshholdedimg",bw);
		//finding contours
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(bw,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
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

		//drawing contours	
		/*Mat drawing=Mat::zeros(src.size(),CV_8UC3);
		RNG rng(12345);
		for(int i=1;i<=9;i++)
		{
			//cout<<index[i]<<"\n";
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours(drawing,contours,contours.size()-index[i],color,-1,8,hierarchy,0,Point());
		}*/

		//imshow("contours",drawing);
		//imshow("original",orig);
	
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
		
		Mat cleanimg = Mat::zeros(500,500 , CV_8UC3);

		//Finding the corresponding 4 coordinates in the cleaned image
		vector<Point2f> quad_pts;
		quad_pts.push_back(Point(0, 0));
		quad_pts.push_back(Point(cleanimg.cols, 0));
		quad_pts.push_back(Point(0, cleanimg.rows));
		quad_pts.push_back(Point(cleanimg.cols, cleanimg.rows));
		Mat perspect=getPerspectiveTransform(pt,quad_pts);
		warpPerspective(cdst,cleanimg,perspect,cleanimg.size());
		//imshow("cleanimg",cleanimg);
	//finding color
		for(int i=1;i<=9;i++) {
			Rect r=boundingRect(contours[contours.size()-index[i]]);
			Scalar mymean=mean(orig(r));
			//cout<<" "<<center[contours.size()-index[i]].x<<" "<<center[contours.size()-index[i]].y<<"\n";
			//cout<<mymean.val[0]<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\t";//<<"\t"<<mymean.val[1]<<"\t"<<mymean.val[2]<<"\n";
			int t=findcolourbgr(mymean.val[0],mymean.val[1],mymean.val[2]);
  			switch(t){
  				case 1: //cout<<"White"<<"\t"; 
  						s[i-1]='W';break;
  				case 2: //cout<<"Orange"<<"\t"; 
  						s[i-1]='O';break;
  				case 3: //cout<<"Yellow"<<"\t";
  						s[i-1]='Y';break;
  				case 4: //cout<<"Green"<<"\t"; 
  						s[i-1]='G';break;
  				case 5: //cout<<"Blue"<<"\t"; 
  						s[i-1]='B';break;
  				case 6: //cout<<"Red"<<"\t"; 
  						s[i-1]='R';break;
  			}
  		//if(i%3==0) cout<<"\n";
			}
		s[9]='\0';
		strcat(str,s);
	}
		ofstream fout("input.txt",ios::out);
		fout<<str;
		fout<<"\0";
		fout.close();
		//cout<<s<<"\n";
		//cout<<str<<"\n";

	//waitKey(0);
	return 0;
}
