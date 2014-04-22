#ifndef _CONSTANT_COLOR_TEXTURE_H_
#define _CONSTANT_COLOR_TEXTURE_H_ 1

#include "rgb.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"

/**
 * the ConstantColorTexture class is implemented from the abstract Texture class and it just returns a constant color
 */
class ConstantColorTexture:public Texture{
public:
	// constructors
	ConstantColorTexture(const rgb &_color);
	ConstantColorTexture(const ConstantColorTexture &_orig);

	void setColor(const rgb &_color);
	rgb getColor()const;

	// implements the value method of the Texture class
	rgb value(const Vector2 &,const Vector3 &);
private:
	rgb color;
};
#endif // _CONSTANT_COLOR_TEXTURE_H_