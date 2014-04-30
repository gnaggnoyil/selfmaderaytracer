#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_ 1

#include "Texture.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Image.h"
#include <string>
#include "rgb.h"
#include <memory>

/**
 * this class defines a image texture inherited from abstract Texture class
 */
class ImageTexture:public Texture{
public:
	// constructors
	ImageTexture(std::string filename);
	ImageTexture(const ImageTexture &_orig);

	void setImage(const std::tr1::shared_ptr<Image> &_image);
	std::tr1::shared_ptr<Image> getImage()const;

	// implement the value() method from the abstract Texture class
	rgb value(const Vector2 &uv,const Vector3 &);
private:
	std::tr1::shared_ptr<Image> image;
};
#endif // _IMAGE_TEXTURE_H_