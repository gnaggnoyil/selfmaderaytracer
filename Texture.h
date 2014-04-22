#ifndef _TEXTURE_H_
#define _TEXTURE_H_ 1

#include "rgb.h"
#include "Vector2.h"
#include "Vector3.h"

/**
 * this is an abstract class/interface that defines what a texture should behave
 */
class Texture{
public:
	// should return the rgb color of the texture of a certain UV coordinate or a 3-D hitpoint or both
	virtual rgb value(const Vector2 &,const Vector3 &)=0;
};
#endif // _TEXTURE_H_