#ifndef _VECTOR2_H_
#define _VECTOR2_H_ 1

/**
 * this class defines a what a 2-D vector should behave
 */
class Vector2{
public:
	//constructors
	Vector2();
	Vector2(double _x,double _y);
	Vector2(const Vector2 &orig);

	double operator[](int i)const;
	double &operator[](int i);
	double getX()const;
	double getY()const;
	void setX(double _x);
	void setY(double _y);

	// operators
	friend Vector2 operator+(const Vector2 &v1,const Vector2 &v2);
	friend Vector2 operator-(const Vector2 &v1,const Vector2 &v2);
	friend Vector2 operator*(const Vector2 &v1,double scale);
	friend Vector2 operator*(double scale,const Vector2 &v1);
	friend Vector2 operator/(const Vector2 &v1,double scale);
	friend Vector2 operator/(double scale,const Vector2 &v1);
	friend double dot(const Vector2 &v1,const Vector2 &v2);
	friend double corss(const Vector2 &v1,const Vector2 &v2);
	friend bool operator==(const Vector2 &v1,const Vector2 &v2);
	friend bool operator!=(const Vector2 &v1,const Vector2 &v2);

	const Vector2 &operator+()const;
	Vector2 operator-()const;
	Vector2 &operator+=(const Vector2 &v1)const;
	Vector2 &operator-=(const Vector2 &v1)const;
	Vector2 &operator*=(double scale)const;
	Vector2 &operator/=(double scale);

	// return the length of the vector
	double length()const;
	// return the square length of the vector
	double sqrLength()const;
	// return the normalized vector of a given vector
	friend Vector2 normalize(const Vector2 &v);
private:
	double e[2];
};
#endif // VECTOR2_H_