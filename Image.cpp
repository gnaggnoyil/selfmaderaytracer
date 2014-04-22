#include "Image.h"
#include "rgb.h"
#include <string>
#include <fstream>

Image::Image(const Image &_orig){
	nx=_orig.getWidth();
	ny=_orig.getHeight();
	data=new rgb*[nx];
	for(int i=0;i<nx;i++){
		data[i]=new rgb[ny];
		for(int j=0;j<ny;j++)
			data[i][j]=_orig.getRGB(i,j);
	}
}

Image::Image(int _width,int _height,const rgb &background=rgb(0,0,0))
	:nx(_width),ny(_height){
	data=new rgb*[nx];
	for(int i=0;i<nx;i++){
		data[i]=new rgb[ny];
		for(int j=0;j<ny;j++)
			data[i][j]=background;
	}
}

Image::Image(std::string filename){
	std::ifstream in(filename,std::ios::in|std::ios::binary);
	char ch,type;
	in.get(ch);
	in.get(type);
	int num;
	in>>nx>>ny>>num;
	data=new rgb*[nx];
	for(int i=0;i<nx;i++)
		data[i]=new rgb[ny];

	in.get(ch);
	char r,g,b;
	for(int i=ny-1;i>0;--i)
		for(int j=0;j<nx;j++){
			in.get(r);
			in.get(g);
			in.get(b);
			data[j][i]=rgb((double)((unsigned char)r/255.0),(double)((unsigned char)g/255.0),(double)((unsigned char)b/255.0));
		}

	in.close();
}

Image::~Image(){
	for(int i=0;i<nx;i++)
		delete[] data[i];
	delete[] data;
}

inline rgb Image::getRGB(int i,int j)const{
	return data[i][j];
}

inline bool Image::setRGB(int i,int j,const rgb &_color){
	if((i<0)||(i>=nx))
		return false;
	if((j<0)||(j>=ny))
		return false;
	data[i][j]=_color;
	return true;
}

inline int Image::getWidth()const{
	return nx;
}

inline int Image::getHeight()const{
	return ny;
}

bool Image::writePPM(std::ostream &out){
	out<<"P6"<<std::endl;
	out<<nx<<' '<<ny<<std::endl;
	out<<"255"<<std::endl;
	unsigned int r,g,b;
	for(int i=ny-1;i>0;--i)
		for(int j=0;j<nx;j++){
			r=(unsigned int)(256*data[j][i].getR());
			g=(unsigned int)(256*data[j][i].getG());
			b=(unsigned int)(256*data[j][i].getB());
			r=r<256?r:255;
			g=g<256?g:255;
			b=b<256?b:255;
			out.put((unsigned char)r);
			out.put((unsigned char)g);
			out.put((unsigned char)b);
		}
}