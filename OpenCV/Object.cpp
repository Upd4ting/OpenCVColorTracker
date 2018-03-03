#include "Object.h"

Object::Object(std::string name, Scalar hsvMin, Scalar hsvMax, Scalar color)
{
	setType(name);
	setHSVmin(hsvMin);
	setHSVmax(hsvMax);
	setColor(color);
}

Object::~Object(void)
{
}

int Object::getXPos() {
	return this->xPos;
}

void Object::setXPos(int x) {
	this->xPos = x;
}

int Object::getYPos() {
	return this->yPos;
}

void Object::setYPos(int y) {
	this->yPos = y;
}

Scalar Object::getHSVmin() {
	return this->HSVmin;
}

Scalar Object::getHSVmax() {
	return this->HSVmax;
}

void Object::setHSVmin(Scalar min) {
	this->HSVmin = min;
}

void Object::setHSVmax(Scalar max) {
	this->HSVmax = max;
}

string Object::getType() {
	return type; 
}

void Object::setType(string t) {
	type = t; 
}

Scalar Object::getColor() {
	return Color;
}

void Object::setColor(Scalar c) {
	Color = c;
}