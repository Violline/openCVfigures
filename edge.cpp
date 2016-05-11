#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat OurImage, gray, edges;

int main()
{
	string Destination = "C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\NOVELTY_DATA\\301.jpg";
	OurImage = imread(Destination, CV_LOAD_IMAGE_COLOR);
	if (!OurImage.data)
	{
		printf("No image!");
		getchar();
		return -1;
	}

	threshold(OurImage, gray, 125, 255, 0);
	Canny(gray, edges, 50, 200, 3);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_TC89_L1, Point(0, 0));
	RNG rng(time(NULL));

	Mat drawing = Mat::zeros(edges.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	cout <<"contour size is: " << contours.size() << endl;

	vector<Moments> mu(contours.size());
	for (int i = 0; i<contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	namedWindow("WINDOW", CV_WINDOW_AUTOSIZE);
	imshow("WINDOW", drawing);
	waitKey(0);
}
