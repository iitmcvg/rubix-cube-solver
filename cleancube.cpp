#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;
//function to sort corner points of the sheet in cyclic order
bool compareContourLen ( vector<Point> contour1, vector<Point> contour2 ) 
{
    double i = fabs( arcLength(Mat(contour1),false) );
    double j = fabs( arcLength(Mat(contour2),false) );
    return ( i < j );
}

void sortcorners(vector<Point2f>& contour, 
                 Point2f center)
{
	//Declaring variables top and bot to hold the top 2 and bottom 2 points respectively
	vector<Point> top, bot;

	//top and bot are filled by comparing y coordinate of the points with that of the center
	for (int i = 0; i < contour.size(); i++)
	{
		if (contour[i].y < center.y)
			top.push_back(contour[i]);
		else
			bot.push_back(contour[i]);
	}

	//Clearing contour
	contour.clear();
	
	if (top.size() == 2 && bot.size() == 2){

		//Assigning corner points as top left(tl), top right(tr) and so on
		Point tl = top[0].x > top[1].x ? top[1] : top[0];
		Point tr = top[0].x > top[1].x ? top[0] : top[1];
		Point bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		Point br = bot[0].x > bot[1].x ? bot[0] : bot[1];
	
		//Filling contour with the corner sorted in right order
		contour.push_back(tl);
		contour.push_back(tr);
		contour.push_back(br);
		contour.push_back(bl);
	}
}

int main(int argc,char**argv)
{
	Mat src = imread(argv[1]);
	if (src.empty())
		return -1;
	Mat orig=src.clone();
	imshow("orig",orig);
	
	//Resize the image
	resize(src,src,orig.size()/3);
	
	Mat bw;

	//Convert to grayscale
	cvtColor(src, bw, CV_BGR2GRAY);

	//Blur the image using GuassianBlur
	GaussianBlur(bw, bw, cv::Size(7, 7),2,2);
	
	//Edge detection using Canny
	Canny(bw, bw, 10, 100, 3);
	
	vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;

  	//Find contours in the image
  	findContours( bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  	//Sort contours in order of increasing length
  	sort(contours.begin(), contours.end(), compareContourLen);
  	
  	vector<Point> biggestContour;
  	vector<Point2f> contour;
  	
  	int t=1;
  	//Checking the contours from the last to find the biggest contour having 4 corner points
  	do{
  		//Assigning biggestContour to an element of contours
  		biggestContour=contours[contours.size()-t];

  		//Approximating biggestContour to a polygon and saving the approximated result into contour
  		cv::approxPolyDP(cv::Mat(biggestContour), contour, cv::arcLength(cv::Mat(biggestContour), true) * 0.02, true);
  		t++;
  		
  		}while((t>=contours.size())&&(contour.size()!=4));


  	//Checking if contour has 4 corner points
  	if(contour.size()!=4)
  		return -1;
  	
  	// Get center
  	Point2f center(0,0);
	for (int i = 0; i < contour.size(); i++)
		{
			center.x += contour[i].x;
			center.y += contour[i].y;
		}
	center.x = center.x/4;
	center.y = center.y/4;
	
	//Sort the corners in cyclic order
	sortcorners(contour, center);
	if (contour.size() == 0)
		return -1;
	
	//Finding the corners in the original image(before resizing)
	for(int i=0;i<4;i++)
  		contour[i]*=3;
  	for (int i = 0; i < contour.size(); i++)
		{
			cout<<contour[i].x<<"\t"<<contour[i].y<<"\n";
		}
	//Rect myrect(contour[0].x,contour[0].y,(contour[1].x-contour[0].x),(contour[3].y-contour[0].y));
	Rect myrect(675,135,450,450);
	Mat final;
	Mat(orig(myrect)).copyTo(final);
	imshow("final",final);
	waitKey(0);
	return 0;
}

