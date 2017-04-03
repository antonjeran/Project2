#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

using namespace cv;
using namespace std;
int pause;
Mat image1, image2;
vector<Point2f> points1, points2, points_morph;
void InputPoints();
void CallBackFunc(int event, int x, int y, int flags, void* ptr);
Mat morphTriangle(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha);
void applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri);
string image_name1, image_name2;
float x, y;
int index1,n1,n2;
int click = 1;
bool isFirst;
bool endOfLineDet = true;
float alpha;
ofstream input1,input2;
vector<Point2f> d1, d2, d_morph;
vector<int> indices;

vector<Point2f> Delaunay(vector<Point2f> delaunay, vector<Point2f> point, Size sze);
vector<Point2f> Delaunay_Map(vector<Point2f> delaunay, vector<Point2f> point, vector<int> indices);
int main()
{
	cout << " what is the aplha value " << endl;
	cin >> alpha;
	Size morph,im1,im2;

	image_name1 = "arun";
	image_name2 = "parrot";

	image1 = imread(image_name1 + ".jpg");
	image2 = imread(image_name2+".jpg");

	im1 = image1.size();
	im2 = image2.size();

	morph.height = ((1-alpha)*im1.height + alpha*im2.height) ;
	morph.width = ((1-alpha)*im1.width + alpha*im2.width) ;
	image1.convertTo(image1, CV_32F);
	image2.convertTo(image2, CV_32F);

	Mat Morphed_Image = Mat::zeros(image1.size(), CV_32FC3);

	InputPoints();

	//Compute weighted average
	
	for (int i = 0; i < points1.size(); i++)
	{
		
		x = (1 - alpha) * points1[i].x + alpha * points2[i].x;
		y = (1 - alpha) * points1[i].y + alpha * points2[i].y;

		points_morph.push_back(Point2f(x, y));
	}
	d1 = Delaunay(d1, points1, im1);
	d2 = Delaunay_Map(d2, points2, indices);
	d_morph = Delaunay_Map(d_morph, points_morph, indices);
	
	for (int j = 0; j <= (d1.size() / 3)+1; j++)
	{
		vector<Point2f> tri1, tri2, tri_morph;
		
		tri1.push_back(d1[j * 3]);
		tri1.push_back(d1[(3 * j) + 1]);
		tri1.push_back(d1[(3 * j) + 2]);

		tri2.push_back(d2[j * 3]);
		tri2.push_back(d2[(3 * j) + 1]);
		tri2.push_back(d2[(3 * j) + 2]);

		tri_morph.push_back(d_morph[j * 3]);
		tri_morph.push_back(d_morph[(3 * j) + 1]);
		tri_morph.push_back(d_morph[(3 * j) + 2]);
		
	
		Morphed_Image=morphTriangle(image1, image2, Morphed_Image, tri1, tri2, tri_morph, alpha);


	}
	
	cin >> x;


}

void InputPoints()
{

	cout << "Do you want to input points manually (True/False)" << endl;
	bool choice;
	cin >> choice;

	imshow("1st Image", image1/255.0);
	
	imshow("2nd Image", image2/255.0);
	waitKey(0);

	if (choice)
	{
		ifstream text1(image_name1 + ".txt");
		ifstream text2(image_name2 + ".txt");
	
		while (text1 >> x >> y)
		{
			points1.push_back(Point2f(x, y));
		}

		while (text2 >> x >> y)
		{
			points2.push_back(Point2f(x, y));
		}


	}
	else
	{
		namedWindow("1st Image", CV_WINDOW_AUTOSIZE);
		namedWindow("2nd Image", CV_WINDOW_AUTOSIZE);

		isFirst = true;
		input1.open( image_name1 + ".txt", std::fstream::out);
		input2.open(image_name2 + ".txt", std::fstream::out);
		while (click<2)
		{
			n1 = 0;
			setMouseCallback("1st Image", CallBackFunc,&n1);
			n2 = 1;
			endOfLineDet = true;
			setMouseCallback("2nd Image", CallBackFunc,&n2);
			endOfLineDet = true;
			waitKey(0);
			
			input1.close();
			input2.close();
			ifstream text1(image_name1 + ".txt");
			ifstream text2(image_name2 + ".txt");

			while (text1 >> x >> y)
			{
				points1.push_back(Point2f(x, y));
			}

			while (text2 >> x >> y)
			{
				points2.push_back(Point2f(x, y));
			}
			
		}

	}
}

void CallBackFunc(int event, int x, int y, int flags, void* ptr)
{

	int* h1;
	int k, l;
	h1 = (int*)ptr;

	/*********************************************/
	// Mouse click call back function 
	/*********************************************/

	if (event == EVENT_LBUTTONDOWN)//Left click detect
	{
		k = (int)x;
		l = (int)y;
		
		if (*h1 == 0)
		{
			input1 << k << "  " << l << endl;
			cout <<"1st  "<< k << "  " << l << endl;
			//indicating in the image
			image1.at<uchar>(y, x) = 255;
		/*	image1.at<uchar>(y - 1, x) = 255;
			image1.at<uchar>(y + 1, x) = 255;
			image1.at<uchar>(y, x - 1) = 255;
			image1.at<uchar>(y, x + 1) = 255;
			image1.at<uchar>(y - 2, x) = 255;
			image1.at<uchar>(y + 2, x) = 255;
			image1.at<uchar>(y, x - 2) = 255;
			image1.at<uchar>(y, x + 2) = 255;*/

			imshow("1st Image", image1/255.0);
		}

		if (*h1 == 1)
		{
			input2 << k << "  " << l << endl;
			cout << "2nd  " << k << "  " << l << endl;

			//indicating in the image
			image2.at<uchar>(y, x) = 255;
			/*image2.at<uchar>(y - 1, x) = 255;
			image2.at<uchar>(y + 1, x) = 255;
			image2.at<uchar>(y, x - 1) = 255;
			image2.at<uchar>(y, x + 1) = 255;
			image2.at<uchar>(y - 2, x) = 255;
			image2.at<uchar>(y + 2, x) = 255;
			image2.at<uchar>(y, x - 2) = 255;
			image2.at<uchar>(y, x + 2) = 255;*/

			imshow("2nd Image", image2/255.0);
		}	

		
	}
	else if (event == EVENT_RBUTTONDOWN)//Right click detect
	{
		endOfLineDet = false;
		click = 5;
		cout << "Enter to clicking" << endl;
	}
}

vector<Point2f> Delaunay(vector<Point2f> delaunay, vector<Point2f> point, Size sze)
{
	int numbers = 0;
	Rect rect(0, 0, sze.width, sze.height);

	Subdiv2D subdiv(rect);

	for (vector<Point2f>::iterator it = point.begin(); it != point.end(); it++)
	{
		subdiv.insert(*it);
	}

	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point> pt(3);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
	
		Vec6f t = triangleList[i];
		if ((cvRound(t[0]) <= sze.width) && (cvRound(t[2]) <= sze.width) && (cvRound(t[4]) <= sze.width) && (cvRound(t[0]) >= 0) && (cvRound(t[2]) >= 0) && (cvRound(t[4]) >= 0) && (cvRound(t[1]) <= sze.height) && (cvRound(t[3]) <= sze.height) && (cvRound(t[5]) <= sze.height) && (cvRound(t[1]) >= 0) && (cvRound(t[3]) >= 0) && (cvRound(t[5]) >= 0))
		{	
			
			delaunay.push_back(Point(cvRound(t[0]), cvRound(t[1])));
			delaunay.push_back(Point(cvRound(t[2]), cvRound(t[3])));
			delaunay.push_back(Point(cvRound(t[4]), cvRound(t[5])));
			for (int n = 0; n < 3; n++)
			{
				auto it = std::find(point.begin(), point.end(), delaunay[numbers+n]);
				auto index = std::distance(point.begin(), it);
				indices.push_back(index);
			}
			numbers = numbers + 3;
					
		}		
	}
	
	return delaunay;
}

vector<Point2f> Delaunay_Map(vector<Point2f> delaunay, vector<Point2f> point, vector<int> indices)
{	
	for (vector<int>::iterator it = indices.begin(); it != indices.end(); it++)
	{
		delaunay.push_back(point[*it]);	
	}

	return delaunay;
}


// Apply affine transform calculated using srcTri and dstTri to src
void applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri)
{

	// Given a pair of triangles, find the affine transform.
	Mat warpMat = getAffineTransform(srcTri, dstTri);

	// Apply the Affine Transform just found to the src image
	warpAffine(src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}

// Warps and alpha blends triangular regions from img1 and img2 to img
Mat morphTriangle(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha)
{
	
	// Find bounding rectangle for each triangle
	Rect r = boundingRect(t);
	Rect r1 = boundingRect(t1);
	Rect r2 = boundingRect(t2);

	// Offset points by left top corner of the respective rectangles
	vector<Point2f> t1Rect, t2Rect, tRect;
	vector<Point> tRectInt;
	for (int i = 0; i < 3; i++)
	{
	
		tRect.push_back(Point2f(t[i].x - r.x, t[i].y - r.y));
		tRectInt.push_back(Point(t[i].x - r.x, t[i].y - r.y)); // for fillConvexPoly

		t1Rect.push_back(Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
		t2Rect.push_back(Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
	}

	// Get mask by filling triangle
	Mat mask = Mat::zeros(r.height, r.width, CV_32FC3);
	fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);
	
	imshow("Image", mask );
	waitKey(5);

	// Apply warpImage to small rectangular patches
	Mat img1Rect, img2Rect;
	img1(r1).copyTo(img1Rect);
	img2(r2).copyTo(img2Rect);
	
	Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());
	Mat warpImage2 = Mat::zeros(r.height, r.width, img2Rect.type());
	
	applyAffineTransform(warpImage1, img1Rect, t1Rect, tRect);
	applyAffineTransform(warpImage2, img2Rect, t2Rect, tRect);
	imshow("warp1", warpImage1 / 255.0);
	waitKey(5);
	imshow("warp2", warpImage2 / 255.0);
	waitKey(5);

	// Alpha blend rectangular patches
	Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;

	// Copy triangular region of the rectangular patch to the output image
	multiply(imgRect, mask, imgRect);
	multiply(img(r), Scalar(1.0, 1.0, 1.0) - mask, img(r));
	img(r) = img(r) + imgRect;
	imshow("full", img/255.0);
	waitKey(5);
	imwrite("Alex_Morphed.jpg", img);
	return img;
}
