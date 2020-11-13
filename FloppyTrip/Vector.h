#ifndef HEADER_CONSTANTS	
#define HEADER_CONSTANTS

struct SDL_Rect;

class Vector {
private:
	double x;
	double y;
public:
	Vector();
	Vector(double,double);

	Vector operator+(const Vector&);
	Vector operator-(const Vector&);
	Vector operator*(const Vector&);
	Vector operator*(const double);
	Vector operator/(const Vector&);

	Vector normalize();
	Vector cross();
	double dot(Vector&);

	double getX();
	double getY();
	void setX(double);
	void setY(double);
	void set(double,double);

	void transferInRect(SDL_Rect*);
};


#endif