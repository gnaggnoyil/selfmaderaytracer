#include "ImageTexture.h"
#include <string>
#include "Image.h"
#include "rgb.h"
#include "Vector3.h"
#include "Vector2.h"
#include <memory>

ImageTexture::ImageTexture(std::string filename)
	:image(new Image(filename)){}

ImageTexture::ImageTexture(const ImageTexture &_orig)
	:image(_orig.getImage()){}

void ImageTexture::setImage(const std::tr1::shared_ptr<Image> &_image){
	image=_image;
}

std::tr1::shared_ptr<Image> ImageTexture::getImage()const{
	return image;
}

rgb ImageTexture::value(const Vector2 &uv,const Vector3 &){
	double u=uv.getX()-(int)uv.getX();
	double v=uv.getY()-(int)uv.getY();
	u*=image->getWidth();
	v*=image->getHeight();
	int iu=(int)u;
	int iv=(int)v;
	double tu=u-iu;
	double tv=v-iv;
	rgb c=image->getRGB(iu,iv)*(1-tu)*(1-tv)+
		image->getRGB(iu+1,iv)*tu*(1-tv)+
		image->getRGB(iu,iv+1)*(1-tu)*tv+
		image->getRGB(iu+1,iv+1)*tu*tv;
	return c;
}