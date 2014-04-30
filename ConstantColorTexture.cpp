#include "ConstantColorTexture.h"
#include "rgb.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"

ConstantColorTexture::ConstantColorTexture(const rgb &_color)
	:color(_color){}

ConstantColorTexture::ConstantColorTexture(const ConstantColorTexture &_orig)
	:color(_orig.getColor()){}

void ConstantColorTexture::setColor(const rgb &_color){
	color=_color;
}

rgb ConstantColorTexture::getColor()const{
	return color;
}

rgb ConstantColorTexture::value(const Vector2 &,const Vector3 &){
	return color;
}