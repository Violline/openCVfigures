//extract circle - white

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>;

using namespace cv;
using namespace std;

Mat SourceImg;

int main()
{
	string path("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\NOVELTY_DATA\\*.jpg"); // sciezka do uzupelnienia
    FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\NOVELTY.txt","w"); //przygotowanie pliku tekstowego
	//string pathToData("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\TRAINING_DATA\\*.jpg"); // sciezka do uzupelnienia
    //FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\TRAINING.txt","w"); //przygotowanie pliku tekstowego
	//string pathToData("C:\\Users\\Olka\\Desktop\\NoveltyDetectionData\\Set1\\TESTING_DATA\\*.jpg"); // sciezka do uzupelnienia
    //FILE* txtFile = fopen("C:\\Users\\Olka\\Desktop\\TESTING.txt","w"); //przygotowanie pliku tekstowego

	
	vector<String> image;
	vector<Mat> data;
	cv::glob(path, image, true); // recurse
	for (size_t k = 0; k<image.size(); ++k)
	{
		SourceImg = imread(image[k]);
		if (SourceImg.empty()) continue; //only proceed if sucsessful
		cout << "Zdjecia zaladowane o Pani" << endl;
										 
										 
										 
										 
										 
										 // you probably want to do some preprocessing
		data.push_back(SourceImg);
	}

	namedWindow("WINDOW", CV_WINDOW_AUTOSIZE);
	imshow("WINDOW", data[1]);

	waitKey(0);
}
