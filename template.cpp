#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <sstream>


using namespace cv;
using namespace std;

Point point1, point2; /* vertical points of the bounding box */
int drag = 0;
Rect rect; /* bounding box */
Mat img, roiImg; /* roiImg - the part of the image in the bounding box */
int select_flag = 0;
bool go_fast = false;

Mat mytemplate;


///------- template matching -----------------------------------------------------------------------------------------------

Mat TplMatch(Mat &img, Mat &mytemplate)
{
	Mat result;

	matchTemplate(img, mytemplate, result, CV_TM_CCOEFF); //CCORR_NORMED Works well, SQDIFF_NORMED ehh, CV_TM_CCOEFF real well
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
//	for (int i = 0; i < result.rows; i++){
//		for (int j = 0; j < result.cols; j++){
			
			cout << "result(0,0) = " << result.at<int>(0, 0) << "     " << endl;
//		}
//		cout << endl;

//	}
			imshow("result", result);

	return result;
}


///------- Localizing the best match with minMaxLoc ------------------------------------------------------------------------

Point minmax(Mat &result)
{
	double minVal, maxVal;
	Point  minLoc, maxLoc, matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	return matchLoc;
}


///------- tracking --------------------------------------------------------------------------------------------------------

void track()
{
	/*if (select_flag)
	{
		//roiImg.copyTo(mytemplate);
		//         select_flag = false;
		go_fast = true;
	}*/

	//     imshow( "mytemplate", mytemplate ); waitKey(0);

	Mat result = TplMatch(img, mytemplate);
	if (result.at<int>(0, 0) < 1050000000){
		Point match = minmax(result);

		rectangle(img, match, Point(match.x + mytemplate.cols, match.y + mytemplate.rows), CV_RGB(255, 255, 255), 0.5);

		cout << "match: " << match << endl;
		cout << "center at [" << (match.x + mytemplate.cols) / 2 << " , " << (match.y + mytemplate.rows) / 2 << "]" << endl;
	}
	/// latest match is the new template
	//Rect ROI = cv::Rect(match.x, match.y, mytemplate.cols, mytemplate.rows);
	//roiImg = img(ROI);
	//roiImg.copyTo(mytemplate);
	//imshow("roiImg", roiImg); //waitKey(0);
}


///------- Main() ----------------------------------------------------------------------------------------------------------

int main()
{
	int k;
	mytemplate = imread("google1.jpg", 1);
	imshow("template", mytemplate);
	///open webcam
	VideoCapture cap(0);
	if (!cap.isOpened())
	return 1;


	/// Set video to 320x240
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> img;
	//GaussianBlur(img, img, Size(7, 7), 3.0);
	imshow("image", img);

	while (1)
	{
		cap >> img;
		if (img.empty())
			break;

		// Flip the frame horizontally and add blur
		cv::flip(img, img, 1);
		//GaussianBlur(img, img, Size(7, 7), 3.0);

		//		if (rect.width == 0 && rect.height == 0)
		//			cvSetMouseCallback("image", mouseHandler, NULL);
		//		else
		track();

		imshow("image", img);
		//  waitKey(100);   k = waitKey(75);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	return 0;
}



