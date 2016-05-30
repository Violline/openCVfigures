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

Mat SourceImg;
Mat OurImage, thresh, gray, edges, morph, morph2;

int area, figure_no;
int area_max = 0;

vector<Point2f> fig_mass_center;
vector<int> fig_area;


int main()
{
	string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set2\\TESTING_DATA_ROI\\*.jpg"); // sciezka do uzupelnienia

	Mat src_base, hsv_base;
	Mat src_test1, hsv_test1;
	Mat src_test2, hsv_test2;
	Mat src_test3, hsv_test3;
	Mat hsv_half_down;
	vector<String> image;
	vector<Mat> data;
	cv::glob(path, image, true); // recurse
	//for (size_t k = 0; k<image.size(); ++k)
	//{
		src_base = imread(image.at(1));
		src_test1 = imread(image.at(2));
		src_test2 = imread(image.at(3));
		src_test3 = imread(image.at(4));


		cout << "Zdjecia zaladowane o Pani" << endl;

		/// Convert to HSV
		cvtColor(src_base, hsv_base, COLOR_BGR2HSV);
		cvtColor(src_test1, hsv_test1, COLOR_BGR2HSV);
		cvtColor(src_test2, hsv_test2, COLOR_BGR2HSV);
		cvtColor(src_test3, hsv_test3, COLOR_BGR2HSV);

		//hsv_half_down = hsv_base(Range(hsv_base.rows / 2, hsv_base.rows - 1), Range(0, hsv_base.cols - 1));

		/// Using 50 bins for hue and 60 for saturation
		int h_bins = 50; int s_bins = 60;
		int histSize[] = { h_bins, s_bins };

		// hue varies from 0 to 179, saturation from 0 to 255
		float h_ranges[] = { 0, 180 };
		float s_ranges[] = { 0, 256 };

		const float* ranges[] = { h_ranges, s_ranges };

		// Use the o-th and 1-st channels
		int channels[] = { 0, 1 };


		/// Histograms
		MatND hist_base;
		MatND hist_half_down;
		MatND hist_test1;
		MatND hist_test2;
		MatND hist_test3;


		/// Calculate the histograms for the HSV images
		calcHist(&hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
		normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

		//calcHist(&hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
		//normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
		normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false);
		normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_test3, 1, channels, Mat(), hist_test3, 2, histSize, ranges, true, false);
		normalize(hist_test3, hist_test3, 0, 1, NORM_MINMAX, -1, Mat());

		/// Apply the histogram comparison methods
		for (int i = 0; i < 4; i++)
		{
			int compare_method = i;
			double base_base = compareHist(hist_base, hist_base, compare_method);
			//double base_half = compareHist(hist_base, hist_half_down, compare_method);
			double base_test1 = compareHist(hist_base, hist_test1, compare_method);
			double base_test2 = compareHist(hist_base, hist_test2, compare_method);
			double base_test3 = compareHist(hist_base, hist_test3, compare_method);

			printf(" Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, base_base, base_test1, base_test2, base_test3);
		}

		printf("Done \n");







		/// Display
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", histImage);

		waitKey(0);


		//namedWindow("WINDOW", CV_WINDOW_AUTOSIZE);
		//imshow("WINDOW", thresh);

		// Showing the result
		data.push_back(SourceImg);
	//}

	waitKey(0);
}
