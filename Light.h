#ifndef _LIGHT_H_
#define _LIGHT_H_ 1

#include "Vector3.h"
#include "rgb.h"
#include "Common.h"

/**
 * this class defines what a light source should behave
 */
class Light{
public:
	// constructors
	Light(const rgb &_color,const Vector3 &_p,const DynamicVector3Type &_animation);
	Light(const Light &_orig);

	void setColor(const rgb &_color);
	rgb getColor()const;
	void setP(const Vector3 &_p);
	Vector3 getP()const;
	void setAnimation(const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation()const;
private:
	rgb color;
	Vector3 p;
	DynamicVector3Type animation;
};
#endif // _LIGHT_H_