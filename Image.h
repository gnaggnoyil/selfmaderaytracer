#ifndef _IMAGE_H_
#define _IMAGE_H_ 1

#include "rgb.h"
#include <string>
#include <iostream>

/**
 * this defines a image class, which is used to operate on images
 */
class Image{
public:
	// constructors
	Image(int _width,int _height,const rgb &background=rgb(0,0,0));
	Image(const Image &orig);
	// read a PPM image from a file
	Image(std::string filename);

	// destructor
	~Image();

	rgb getRGB(int x,int y)const;
	// returns false if fails to set, otherwise true
	bool setRGB(int x,int y,const rgb &color);
	int getWidth()const;
	int getHeight()const;

	// write the PPM data to a ostream, returns false if fails
	bool writePPM(std::ostream &out);
private:
	int nx,ny;
	rgb **data;
};
#endif // _IMAGE_H_