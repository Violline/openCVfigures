#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat SourceImg, gray, edges;
int thresh = 200;
int corners = 0;
Mat dst, dst_norm, dst_norm_scaled;
Scalar colour = Scalar(147, 20, 255);
RNG rng(12345);

int main()
{
	//string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\TRAINING_DATA\\*.jpg"); // sciezka do uzupelnienia
	//FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\TRAINING.txt", "w"); //przygotowanie pliku tekstowego
	//string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\TESTING_DATA\\*.jpg"); // sciezka do uzupelnienia
	//FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\TESTING.txt", "w"); //przygotowanie pliku tekstowego
	string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\NOVELTY_DATA\\*.jpg"); // sciezka do uzupelnienia
	FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\NOVELTY.txt", "w"); //przygotowanie pliku tekstowego


	vector<String> image;
	vector<Mat> data;
	cv::glob(path, image, true); // recurse
	for (size_t k = 0; k<image.size(); ++k)
	{
		SourceImg = imread(image[k]);
		if (SourceImg.empty()) continue; //only proceed if sucsessful
		cout << "Zdjecia zaladowane o Pani" << endl;

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
		fprintf(txtFile, "%18.5f ", dst);

		Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		/// Detect edges using canny
		Canny(dst_norm_scaled, canny_output, thresh, thresh * 2, 3);
		/// Find contours
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<Moments> mu(contours.size());

		/// Draw contours
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			corners++;
			//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}
		corners = corners/2; 
		cout << "corners: " << corners << endl;
		
		if (corners == 3 || corners == 4)
		{
			for (int i = 0; i < contours.size(); i++)
			{
				mu[i] = moments(contours[i], false);
				double hu[7];
				HuMoments(mu[i], hu);
				// zapis wybranych momentów do pliku tekstowego
				//fprintf(txtFile, "%Object Nr %d ", i);
				fprintf(txtFile, "%18.5f ", mu[i].m00);
				fprintf(txtFile, "%18.5f ", mu[i].mu11);
				fprintf(txtFile, "%18.5f ", mu[i].mu02);
				fprintf(txtFile, "%18.5f ", hu[1]);
				fprintf(txtFile, "%18.5f ", hu[2]);
				fprintf(txtFile, "%18.5f\n", hu[3]);
			}
		}
		else
		{
			fprintf(txtFile, "Picture improper\n");
		}
		corners = 0;

		namedWindow("corners_window", CV_WINDOW_AUTOSIZE);
		imshow("corners_window", dst_norm_scaled);
		waitKey(20);

		// Showing the result
		data.push_back(SourceImg);
	}

	namedWindow("corners_window", CV_WINDOW_AUTOSIZE);
	imshow("corners_window", dst_norm_scaled);

	waitKey(0);
}
