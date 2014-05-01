#include "Ray.h"
#include "Vector3.h"

Ray::Ray(const Vector3 &_orig,const Vector3 &_direction)
	:orig(_orig),direction(_direction){}

Ray::Ray(const Ray &_orig)
	:orig(_orig.getOrigin()),direction(_orig.getDirection()){}

void Ray::setOrigin(const Vector3 &_orig){
	orig=_orig;
}

Vector3 Ray::getOrigin()const{
	return orig;
}

void Ray::setDirection(const Vector3 &_direction){
	direction=_direction;
}

Vector3 Ray::getDirection()const{
	return direction;
}

bool operator==(const Ray &r1,const Ray &r2){
	if(r1.orig!=r2.orig)
		return false;
	if(r1.direction!=r2.direction)
		return false;
}

bool operator!=(const Ray &r1,const Ray &r2){
	return !(r1==r2);
}

Vector3 Ray::pointAt(double t)const{
	return orig+t*direction;
}