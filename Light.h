#ifndef _LIGHT_H_
#define _LIGHT_H_ 1

#include "Vector3.h"
#include "rgb.h"

/**
 * this class defines what a light source should behave
 */
class Light{
public:
	// constructors
	Light(const rgb &_color,const Vector3 &_p);
	Light(const Light &_orig);

	void setColor(const rgb &_color);
	rgb getColor()const;
	void setP(const Vector3 &_p);
	Vector3 getP()const;
private:
	rgb color;
	Vector3 p;
};
#endif // _LIGHT_H_