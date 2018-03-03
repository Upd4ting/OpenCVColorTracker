#pragma once
#include <string>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

class Object
{
public:
	Object(std::string name, Scalar hsvMin, Scalar hsvMax, Scalar color);
	~Object(void);

	int getXPos();
	void setXPos(int x);

	int getYPos();
	void setYPos(int y);

	Scalar getHSVmin();
	Scalar getHSVmax();

	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

	string getType();
	void setType(string t);

	Scalar getColor();
	void setColor(Scalar c);

private:
	int xPos, yPos;
	string type;
	Scalar HSVmin, HSVmax;
	Scalar Color;
};