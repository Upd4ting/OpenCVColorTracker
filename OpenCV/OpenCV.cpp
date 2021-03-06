#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Object.h"

// default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;

// minimum and maximum object area
const int MIN_OBJECT_AREA = 10 * 10;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

// names that will appear at the top of each window
const string windowName = "Original Image";
const string threesholdName = "Threeshold Image";

// List of objects to track
std::vector<Object> objects;


string intToString(int number) {

	std::stringstream ss;
	ss << number;
	return ss.str();
}

Object* getObjectByCoord(int x, int y) {
	for (Object &o : objects) {
		if (o.getXPos() == x && o.getYPos() == y)
			return &o;
	}

	return NULL;
}

void drawObject(Object o, int index, Mat &frame, vector< vector<Point> > contours, vector<Vec4i> hierarchy) {
	cv::Rect rec = cv::boundingRect(contours[index]);
	cv::rectangle(frame, rec, o.getColor(), 3, 8);
	//cv::drawContours(frame, contours, index, o.getColor(), 3, 8, hierarchy);
	//cv::circle(frame, cv::Point(o.getXPos(), o.getYPos()), 5, o.getColor());
	cv::putText(frame, intToString(o.getXPos()) + " , " + intToString(o.getYPos()), cv::Point(o.getXPos(), o.getYPos() + 20), 1, 1, o.getColor());
	cv::putText(frame, o.getType(), cv::Point(o.getXPos(), o.getYPos() - 20), 1, 2, o.getColor());
}

void morphOps(Mat &thresh) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}

void trackFilteredObject(Object theObject, Mat threshold, Mat &cameraFeed) {
	Mat temp;
	threshold.copyTo(temp);

	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find our filtered object
	double refArea = 0;

	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

		for (int index = 0; index >= 0; index = hierarchy[index][0]) {

			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;

			if (area>MIN_OBJECT_AREA) {
				int x = moment.m10 / area;
				int y = moment.m01 / area;

				if (getObjectByCoord(x, y) != NULL)
					continue;

				theObject.setXPos(x);
				theObject.setYPos(y);

				drawObject(theObject, index, cameraFeed, contours, hierarchy);

				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	namedWindow(windowName, 0);

	// Preparing all the objects to track
	// Color in BGR and HSV -> SV are to 255 and H is up to 180 !!
	objects.push_back(Object("green", Scalar(40, 0, 0), Scalar(60, 255, 255), Scalar(0, 255, 73)));

	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture object at location zero (default location for webcam)
	capture.open(0);

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	waitKey(1000);

	while (1) {
		capture.read(cameraFeed);

		if (!cameraFeed.data)
		{
			return -1;
		}

		// Track objects
		for (Object &o : objects) {
			cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
			inRange(HSV, o.getHSVmin(), o.getHSVmax(), threshold);
			morphOps(threshold);
			imshow(threesholdName, threshold);
			trackFilteredObject(o, threshold, cameraFeed);
		}

		imshow(windowName, cameraFeed);

		//delay 30ms so that screen can refresh.
		waitKey(30);
	}
	return 0;
}