#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace cv;
using namespace std;

Mat dst_norm_scaled;
vector<String> image;
vector<int> noveltyDetect;

vector<String> dataOpen1() {

	string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\TEST\\*.jpg"); // sciezka do uzupelnienia
	cv::glob(path, image, true); // recurse
	return image;
}

void txtWrite(int no) {
	ofstream txtFile;
	switch (no) {
	case 1:
		txtFile.open("C:\\Users\\Olka\\Desktop\\TEST.txt"); //przygotowanie pliku tekstowego
		break;
	case 2:
		txtFile.open("C:\\Users\\Olka\\Desktop\\TESTING1.txt"); //przygotowanie pliku tekstowego
		break;
	case 3:
		txtFile.open("C:\\Users\\Olka\\Desktop\\NOVELTY1.txt"); //przygotowanie pliku tekstowego
		break;
	}

	txtFile << left << setw(10) << "No.: ";
	txtFile << left << setw(15) << "Figure name: ";
	txtFile << left << setw(15) << "Novelty dedtection: \n" << endl;

	for (size_t k = 0; k < noveltyDetect.size(); ++k) {
		string fileName = image[k].substr(image[k].find_last_of('\\') + 1);
		if (noveltyDetect.at(k) == 1) {
			txtFile << left << setw(10) << k + 1;
			txtFile << left << setw(15) << fileName;
			txtFile << left << setw(15) << "0 - Nonnovelty" << endl;
		}
		else {
			txtFile << left << setw(10) << k + 1;
			txtFile << left << setw(15) << fileName;
			txtFile << left << setw(15) << "1 - Novelty detected" << endl;
		}
	}
	txtFile.close();
}

Mat cornerDetection(Mat SourceImg) {

	Mat gray, dst, dst_norm;
	Scalar colour = Scalar(147, 20, 255);
	cvtColor(SourceImg, gray, CV_BGR2GRAY);
	dst = Mat::zeros(SourceImg.size(), CV_32FC1);

	// Detecting corners
	cornerHarris(gray, dst, 15, 5, 0, BORDER_DEFAULT);

	// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > 8)
			{
				circle(dst_norm_scaled, Point(i, j), 2, colour, 1, 8, 0);
			}
		}
	}

	return dst_norm_scaled;
}

void corners(Mat dst_norm_scaled) {

	int thresh = 200;
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny and find contours
	Canny(dst_norm_scaled, canny_output, thresh, thresh * 2, 3);
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int corners = contours.size() / 2;
	//cout << "corners: " << corners << endl;

	if (corners == 3 || corners == 4)
	{
		noveltyDetect.push_back(1);
	}
	else
	{
		noveltyDetect.push_back(0);
	}
}

void sort(vector<String>image) {

	Mat SourceImg;
	for (size_t k = 0; k<image.size(); ++k)
	{
		SourceImg = imread(image[k]);
		if (SourceImg.empty()) continue; //only proceed if sucsessful
		cornerDetection(SourceImg);
		corners(dst_norm_scaled);
	}
}



int main()
{
	dataOpen1();
	sort(image);
	txtWrite(1);

	waitKey(0);
}
