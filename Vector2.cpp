#include "Vector2.h"
#include <math.h>

Vector2::Vector2()
	:x(0.0f),y(0.0f){}

Vector2::Vector2(double _x,double _y)
	:x(_x),y(_y){}

Vector2::Vector2(const Vector2 &_orig)
	:x(_orig.getX()),y(_orig.getY()){}

double Vector2::operator[](int i)const{
	if(i==0)
		return x;
	if(i==1)
		return y;
	return *((double *)this+i);
}

double &Vector2::operator[](int i){
	if(i==0)
		return x;
	if(i==1)
		return y;
	return *((double *)this+i);
}

double Vector2::getX()const{
	return x;
}

double Vector2::getY()const{
	return y;
}

void Vector2::setX(double _x){
	x=_x;
}

void Vector2::setY(double _y){
	y=_y;
}

Vector2 operator+(const Vector2 &v1,const Vector2 &v2){
	return Vector2(v1.x+v2.x,v1.y+v2.y);
}

Vector2 operator-(const Vector2 &v1,const Vector2 &v2){
	return Vector2(v1.x-v2.x,v1.y-v2.y);
}

Vector2 operator*(const Vector2 &v1,double scale){
	return Vector2(v1.x*scale,v1.y*scale);
}

Vector2 operator*(double scale,const Vector2 &v1){
	return Vector2(v1.x*scale,v1.y*scale);
}

Vector2 operator/(const Vector2 &v1,double scale){
	return Vector2(v1.x/scale,v1.y/scale);
}

Vector2 operator/(double scale,const Vector2 &v1){
	return Vector2(v1.x/scale,v1.y/scale);
}

double dot(const Vector2 &v1,const Vector2 &v2){
	return v1.x*v2.x+v1.y*v2.y;
}

double cross(const Vector2 &v1,const Vector2 &v2){
	return v1.x*v2.y-v1.y*v2.x;
}

bool operator==(const Vector2 &v1,const Vector2 &v2){
	if(v1.x!=v2.x)
		return false;
	if(v1.y!=v2.y)
		return false;
	return true;
}

bool operator!=(const Vector2 &v1,const Vector2 &v2){
	return !(v1==v2);
}

const Vector2 &Vector2::operator+()const{
	return *this;
}

Vector2 Vector2::operator-()const{
	return Vector2(-x,-y);
}

Vector2 &Vector2::operator+=(const Vector2 &v1){
	*this=*this+v1;
	return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &v1){
	*this=*this-v1;
	return *this;
}

Vector2 &Vector2::operator*=(double scale){
	*this=*this*scale;
	return *this;
}

Vector2 &Vector2::operator/=(double scale){
	*this=*this/scale;
	return *this;
}

double Vector2::length()const{
	return sqrt(x*x+y*y);
}

double Vector2::sqrLength()const{
	return x*x+y*y;
}

Vector2 normalize(const Vector2 &v){
	return v/v.length();
}