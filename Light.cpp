#include "Light.h"
#include "rgb.h"
#include "Vector3.h"
#include "Common.h"

Light::Light(const rgb &_color,const Vector3 &_p,const DynamicVector3Type &_animation)
	:color(_color),p(_p),animation(_animation){}

Light::Light(const Light &_orig)
	:color(_orig.getColor()),p(_orig.getP()),animation(_orig.getAnimation){}

void Light::setColor(const rgb &_color){
	color=_color;
}

rgb Light::getColor()const{
	return color;
}

void Light::setP(const Vector3 &_p){
	p=_p;
}

Vector3 Light::getP()const{
	return p;
}

void Light::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type Light::getAnimation()const{
	return animation;
}

Vector3 Light::getCenter(double time){
	return animation(p,time);
}