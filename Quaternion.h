#ifndef _QUATERNION_H_
#define _QUATERNION_H_ 1

#include "Vector3.h"

class Quaternion{
public:
	Quaternion();
	Quaternion(double _w,double _x,double _y,double _z);
	Quaternion(double _angle,double _x,double _y,double _z);
	Quaternion(double _angle,const Vector3 &_axis);
	Quaternion(const Quaternion &_orig);

	double operator[](int i)const;
	double &operator[](int i);
	void setW(double _w);
	void setX(double _x);
	void setY(double _y);
	void setZ(double _z);
	void setVector(const Vector3 &p);
	double getW()const;
	double getX()const;
	double getY()const;
	double getZ()const;
	Vector3 getVector()const;

	double getAngle()const;
	Vector3 getAxis()const;

	friend Quaternion operator+(const Quaternion &q1,const Quaternion &q2);
	friend Quaternion operator-(const Quaternion &q1,const Quaternion &q2);
	friend Quaternion operator*(const Quaternion &q1,double scale);
	friend Quaternion operator*(double scale,const Quaternion &q1);
	friend Quaternion operator*(const Quaternion &q1,const Quaternion &q2);
	friend Quaternion operator/(const Quaternion &q1,double scale);
	friend double dot(const Quaternion &q1,const Quaternion &q2);
	friend bool operator==(const Quaternion &q1,const Quaternion &q2);
	friend bool operator!=(const Quaternion &q1,const Quaternion &q2);
	
	friend Quaternion log(const Quaternion &q1);
	friend Quaternion exp(const Quaternion &q1);

	const Quaternion &operator+()const;
	Quaternion operator-()const;
	Quaternion &operator+=(const Quaternion &q1);
	Quaternion &operator-=(const Quaternion &q1);
	Quaternion &operator*=(double scale);
	Quaternion &operator*=(const Quaternion &q1);
	Quaternion &operator/=(double scale);

	double norm()const;
	double sqrNorm()const;
	Quaternion inverse()const;

	friend Vector3 applyRotate(const Quaternion &q,const Vector3 &p);
	friend Quaternion slerp(double t,const Quaternion &p,Quaternion &q);
private:
	double w,x,y,z;
};
#endif // _QUATERNION_H_