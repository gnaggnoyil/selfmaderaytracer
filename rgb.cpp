#include "rgb.h"

rgb::rgb(double _r,double _g,double _b)
	:r(_r),g(_g),b(_b){}

rgb::rgb(const rgb &_orig):
	r(_orig.getR()),g(_orig.getG()),b(_orig.getB()){}

rgb::rgb()
	:r(0.0f),g(0.0f),b(0.0f){}

double rgb::operator[](int i)const{
	if(i==0)
		return r;
	if(i==1)
		return g;
	if(i==2)
		return b;
	return *((double *)this+i);
}

double &rgb::operator[](int i){
	if(i==0)
		return r;
	if(i==1)
		return g;
	if(i==2)
		return b;
	return *((double *)this+i);
}

double rgb::getR()const{
	return r;
}

double rgb::getG()const{
	return g;
}

double rgb::getB()const{
	return b;
}

void rgb::setR(double _r){
	r=_r;
}

void rgb::setG(double _g){
	g=_g;
}

void rgb::setB(double _b){
	b=_b;
}

rgb operator+(const rgb &c1,const rgb &c2){
	return rgb(c1.getR()+c2.getR(),c1.getG()+c2.getG(),c1.getB()+c2.getB());
}

rgb operator-(const rgb &c1,const rgb &c2){
	return rgb(c1.getR()-c2.getR(),c1.getG()-c2.getG(),c1.getB()-c2.getB());
}

rgb operator*(const rgb &c1,double scale){
	return rgb(c1.getR()*scale,c1.getG()*scale,c1.getB()*scale);
}

rgb operator*(double scale,const rgb &c1){
	return rgb(c1.getR()*scale,c1.getG()*scale,c1.getB()*scale);
}

rgb operator*(const rgb &c1,const rgb &c2){
	return rgb(c1.getR()*c2.getR(),c1.getG()*c2.getG(),c1.getB()*c2.getB());
}

rgb operator/(const rgb &c1,double scale){
	return rgb(c1.getR()/scale,c1.getG()/scale,c1.getB()/scale);
}

rgb operator/(double scale,const rgb &c1){
	return rgb(c1.getR()/scale,c1.getG()/scale,c1.getB()/scale);
}

rgb operator/(const rgb &c1,const rgb &c2){
	return rgb(c1.getR()/c2.getR(),c1.getG()/c2.getG(),c1.getB()/c2.getB());
}

const rgb &rgb::operator+()const{
	return *this;
}

rgb rgb::operator-()const{
	return rgb(-r,-g,-b);
}

rgb &rgb::operator+=(const rgb &c){
	*this=*this+c;
	return *this;
}

rgb &rgb::operator-=(const rgb &c){
	*this=*this-c;
	return *this;
}

rgb &rgb::operator*=(double scale){
	*this=*this*scale;
	return *this;
}

rgb &rgb::operator*=(const rgb &c){
	*this=*this*c;
	return *this;
}

rgb &rgb::operator/=(double scale){
	*this=*this/scale;
	return *this;
}

rgb &rgb::operator/=(const rgb &c){
	*this=*this/c;
	return *this;
}