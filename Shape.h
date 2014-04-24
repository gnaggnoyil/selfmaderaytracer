#ifndef _SHAPE_H_
#define _SHAPE_H_ 1

#include "Common.h"
#include "Ray.h"
#include "Vector3.h"

/**
 * this is an abstract class/interface that defines what a single shape should behave
 */
class Shape{
public:
	// returns true if the ray at time _time successfully intersects with a t in [tmin,tmax], otherwise return false
	virtual bool hit(const Ray &r,double tmin,double tmax,double _time,HitRecord &record)const=0;
};
#endif // _SHAPE_H_