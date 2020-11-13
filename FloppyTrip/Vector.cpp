#include "Vector.h"
#include <iostream>
#include "SDL.h"
#include <exception>

Vector::Vector() {
	x = 0.707;
	y = 0.707;
}

Vector::Vector(double pX,double pY) {
	x = pX;
	y = pY;
}


Vector Vector::operator+(const Vector& v) {
	Vector n;
	n.x = x + v.x;
	n.y = y + v.y;
	return n;
}

Vector Vector::operator*(const Vector& v) {
	Vector n;
	n.x = x * v.x;
	n.y = y * v.y;
	return n;
}

Vector Vector::operator-(const Vector& v) {
	Vector n;
	n.x = x - v.x;
	n.y = y - v.y;
	return n;
}

Vector Vector::operator/(const Vector& v) {
	Vector n;
	if (v.x == 0 || v.y == 0) {
		throw std::runtime_error("Nullvector");
	}
	n.x = x / v.x;
	n.y = y / v.y;
	return n;
}

Vector Vector::operator*(const double val) {
	Vector n;
	n.x = x * val;
	n.y = y * val;

	return n;
}

double Vector::getX() {
return x;
}

double Vector::getY() {
return y;
}

void Vector::setX(double n) {
	x = n;
}

void Vector::setY(double n) {
	y = n;
}

Vector Vector::normalize() {
	double mag = sqrt(x*x + y*y);
	if (mag == 0.0) {
		throw std::runtime_error("Nullvector");
	}
	
	Vector n(x/mag,y/mag);
	return n;
}

Vector Vector::cross() {
	throw std::runtime_error("to be implemented\n");
}

double Vector::dot(Vector& v) {
	return x*v.x + y*v.y;
}

void Vector::set(double X,double Y) {
	x = X;
	y = Y;
}

void Vector::transferInRect(SDL_Rect* rect) {
	rect->x = (int) floor(x);
	rect->y = (int) floor(y);
}