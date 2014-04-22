#ifndef _VECTOR3_H_
#define _VECTOR3_H_ 1

/**
 * this class defines a what a 3-D vector should behave
 */
class Vector3{
public:
	// constructors
	Vector3(double _x,double _y,double _z);
	Vector3(const Vector3 &_orig);

	double operator[](int i)const;
	double &operator[](int i);
	double getX()const;
	double getY()const;
	double getZ()const;
	void setX(double _x);
	void setY(double _y);
	void setZ(double _z);

	// operators
	friend Vector3 operator+(const Vector3 &v1,const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v1,const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v1,double scale);
	friend Vector3 operator*(double scale,const Vector3 &v1);
	friend Vector3 operator/(const Vector3 &v1,double scale);
	friend Vector3 operator/(double scale,const Vector3 &v1);
	friend Vector3 cross(const Vector3 &v1,const Vector3 &v2);
	friend double dot(const Vector3 &v1,const Vector3 &v2);
	friend bool operator==(const Vector3 &v1,const Vector3 &v2);
	friend bool operator!=(const Vector3 &v1,const Vector3 &v2);

	const Vector3 &operator+()const;
	Vector3 operator-()const;
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(double scale);
	Vector3 &operator/=(double scale);

	// return the length of the vector
	double length()const;
	// return the square of the length of the vector
	double sqrLength()const;
	// return the normalized vector of a given vector
	friend Vector3 normalize(const Vector3 &v);
private:
	double e[3];
};
#endif // _VECTOR3_H_