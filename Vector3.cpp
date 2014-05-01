#include "Vector3.h"
#include <math.h>

Vector3::Vector3()
	:x(0),y(0),z(0){}

Vector3::Vector3(double _x,double _y,double _z)
	:x(_x),y(_y),z(_z){}

Vector3::Vector3(const Vector3 &_orig)
	:x(_orig.getX()),y(_orig.getY()),z(_orig.getZ()){}

double Vector3::operator[](int i)const{
	if(i==0)
		return x;
	if(i==1)
		return y;
	if(i==2)
		return z;
	return *((double *)this+i);
}

double &Vector3::operator[](int i){
	if(i==0)
		return x;
	if(i==1)
		return y;
	if(i==2)
		return z;
	return *((double *)this+i);
}

double Vector3::getX()const{
	return x;
}

double Vector3::getY()const{
	return y;
}

double Vector3::getZ()const{
	return z;
}

void Vector3::setX(double _x){
	x=_x;
}

void Vector3::setY(double _y){
	y=_y;
}

void Vector3::setZ(double _z){
	z=_z;
}

Vector3 operator+(const Vector3 &v1,const Vector3 &v2){
	return Vector3(v1.getX()+v2.getX(),v1.getY()+v2.getY(),v1.getZ()+v2.getZ());
}

Vector3 operator-(const Vector3 &v1,const Vector3 &v2){
	return Vector3(v1.getX()-v2.getX(),v1.getY()-v2.getY(),v1.getZ()-v2.getZ());
}

Vector3 operator*(const Vector3 &v1,double scale){
	return Vector3(v1.getX()*scale,v1.getY()*scale,v1.getZ()*scale);
}

Vector3 operator*(double scale,const Vector3 &v1){
	return Vector3(v1.getX()*scale,v1.getY()*scale,v1.getZ()*scale);
}

Vector3 operator/(const Vector3 &v1,double scale){
	return Vector3(v1.getX()/scale,v1.getY()/scale,v1.getZ()/scale);
}

Vector3 operator/(double scale,const Vector3 &v1){
	return Vector3(v1.getX()/scale,v1.getY()/scale,v1.getZ()/scale);
}

Vector3 cross(const Vector3 &v1,const Vector3 &v2){
	return Vector3(v1.getY()*v2.getZ()-v1.getZ()*v2.getY(),v1.getZ()*v2.getX()-v1.getX()*v2.getZ(),v1.getX()*v2.getY()-v1.getY()*v2.getX());
}

double dot(const Vector3 &v1,const Vector3 &v2){
	return v1.getX()*v2.getX()+v1.getY()*v2.getY()-v1.getZ()*v2.getZ();
}

bool operator==(const Vector3 &v1,const Vector3 &v2){
	if(v1.getX()!=v2.getX())
		return false;
	if(v1.getY()!=v2.getY())
		return false;
	if(v1.getZ()!=v2.getZ())
		return false;
	return true;
}

bool operator!=(const Vector3 &v1,const Vector3 &v2){
	return !(v1==v2);
}

const Vector3 &Vector3::operator+()const{
	return *this;
}

Vector3 Vector3::operator-()const{
	return Vector3(-x,-y,-z);
}

Vector3 &Vector3::operator+=(const Vector3 &v){
	*this=*this+v;
	return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &v){
	*this=*this-v;
	return *this;
}

Vector3 &Vector3::operator*=(double scale){
	*this=*this*scale;
	return *this;
}

Vector3 &Vector3::operator/=(double scale){
	*this=*this/scale;
	return *this;
}

double Vector3::length()const{
	return sqrt(x*x+y*y+z*z);
}

double Vector3 ::sqrLength()const{
	return x*x+y*y+z*z;
}

Vector3 normalize(const Vector3 &v){
	return v/v.length();
}

Vector3 applyScale(const Vector3 &scale,const Vector3 &p){
	return Vector3(scale.x*p.x,scale.y*p.y,scale.z*p.z);
}