#include "Camera.h"
#include "Vector3.h"
#include "Common.h"
#include "Vector2.h"
#include "Ray.h"

Camera::Camera(const Vector3 &_orig,const Vector3 &_lookat,const Vector3 &_up,double _width,double _height,double _hither,const DynamicVector3Type &_animation)
	:orig(_orig),lookat(_lookat),up(_up),cameraWidth(_width),cameraHeight(_height),hither(_hither),animation(_animation){}

Camera::Camera(const Camera &_orig)
	:orig(_orig.getOrig()),lookat(_orig.getLookat()),up(_orig.getUp()),cameraWidth(_orig.getCameraWidth()),cameraHeight(_orig.getCameraHeight()),
	hither(_orig.getHither()),animation(_orig.getAnimation()){}

void Camera::setOrig(const Vector3 &_orig){
	orig=_orig;
}

Vector3 Camera::getOrig()const{
	return orig;
}

void Camera::setLookat(const Vector3 &_lookat){
	lookat=_lookat;
}

Vector3 Camera::getLookat()const{
	return lookat;
}

void Camera::setUp(const Vector3 &_up){
	up=_up;
}

Vector3 Camera::getUp()const{
	return up;
}

void Camera::setCameraWidth(double _width){
	cameraWidth=_width;
}

double Camera::getCameraWidth()const{
	return cameraWidth;
}

void Camera::setCameraHeight(double _height){
	cameraHeight=_height;
}

double Camera::getCameraHeight()const{
	return cameraHeight;
}

void Camera::setHither(double _hither){
	hither=_hither;
}

double Camera::getHither()const{
	return hither;
}

void Camera::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type Camera::getAnimation()const{
	return animation;
}

Ray Camera::getRay(const Vector2 &uv,double time=0.0f){
	double u=uv.getX();
	double v=uv.getY();

	Vector3 _orig=animation(orig,time);
	Vector3 _lookat=animation(lookat,time);
	Vector3 _up=normalize(animation(up,time));

	Vector3 _right=normalize(cross(_up,_orig-_lookat));
	Vector3 point=_lookat+(u-0.5)*cameraWidth*_right+(v-0.5)*cameraHeight*_up;
	return Ray(_orig,normalize(point-_orig));
}

Ray Camera::getRay(double u,double v,double time=0.0f){
	Vector3 _orig=animation(orig,time);
	Vector3 _lookat=animation(lookat,time);
	Vector3 _up=normalize(animation(up,time));

	Vector3 _right=normalize(cross(_up,_orig-_lookat));
	Vector3 point=_lookat+(u-0.5)*cameraWidth*_right+(v-0.5)*cameraHeight*_up;
	return Ray(_orig,normalize(point-_orig));
}