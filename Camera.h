#ifndef _CAMERA_H_
#define _CAMERA_H_ 1

#include "Vector3.h"
#include "Common.h"
#include "Vector2.h"
#include "Ray.h"

/**
 * This defines a camera class, dealing with generating primitive rays and something else
 */
class Camera{
public:
	// constructors
	Camera(const Vector3 &_orig,const Vector3 &_lookat,double _width,double _height);
	Camera(const Camera &_orig);

	void setOrig(const Vector3 &_orig);
	Vector3 getOrig()const;
	void setLookat(const Vector3 &_lookat);
	Vector3 getLookat()const;
	void setCameraWidth(double _width);
	double getCameraWidth()const;
	void setCameraHeight(double _height);
	double getCameraHeight()const;
	void setAnimation(const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation()const;

	// get the primitive ray at a certain time and at a certian UV coordinate varying in [0,1]x[0,1]
	Ray getRay(const Vector2 &uv,double time=0.0f);
	Ray getRay(double u,double v,double time=0.0f);
private:
	Vector3 orig;	// the point at the eye
	Vector3 lookat;	// the point of the middle of the screen
	double cameraWidth,cameraHeight;	// the width and height of the screen
	DynamicVector3Type animation;	// the function that calculates the transform shift of this camera
};
#endif // _CAMERA_H_