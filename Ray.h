#ifndef _RAY_H_
#define _RAY_H_ 1

#include "Vector3.h"

/**
 * this class defines a ray class. a ray is something like o+tv, where o and v are vector and t is a variable
 */
class Ray{
public:
	// constructors
	Ray(const Vector3 &_orig,const Vector3 &_direction);
	Ray(const Ray &_orig);

	void setOrigin(const Vector3 &_orig);
	Vector3 getOrigin()const;
	void setDirection(const Vector3 &_direction);
	Vector3 getDirection()const;

	//operators
	friend bool operator==(const Ray &r1,const Ray &r2);
	friend bool operator!=(const Ray &r1,const Ray &r2);

	// res=orig+t*direction
	Vector3 pointAt(double t)const;
private:
	Vector3 orig;
	Vector3 direction;
};
#endif // _RAY_H_