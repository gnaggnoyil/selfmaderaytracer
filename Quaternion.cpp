#include "Quaternion.h"
#include "Vector3.h"
#include <math.h>

Quaternion::Quaternion()
	:w(0.0f),x(0.0f),y(0.0f),z(0.0f){}

Quaternion::Quaternion(double _w,double _x,double _y,double _z)
	:w(_w),x(_x),y(_y),z(_z){}

Quaternion::Quaternion(double _angle,double _x,double _y,double _z){
	Vector3 _axis(_x,_y,_z);
	_axis=normalize(_axis);
	double sn=sin(0.5f*_angle);
	w=cos(0.5f*_angle);
	x=sn*_axis.getX();
	y=sn*_axis.getY();
	z=sn*_axis.getZ();
}

Quaternion::Quaternion(double _angle,const Vector3 &_axis){
	Vector3 axis=normalize(_axis);
	double sn=sin(0.5f*_angle);
	w=cos(0.5f*_angle);
	x=sn*axis.getX();
	y=sn*axis.getY();
	z=sn*axis.getZ();
}

Quaternion::Quaternion(const Quaternion &_orig)
	:w(_orig.getW()),x(_orig.getX()),y(_orig.getY()),z(_orig.getZ()){}

double Quaternion::operator[](int i)const{
	if(i==0)
		return w;
	if(i==1)
		return x;
	if(i==2)
		return y;
	if(i==3)
		return z;
	return *((double *)this+i);
}

double &Quaternion::operator[](int i){
	if(i==0)
		return w;
	if(i==1)
		return x;
	if(i==2)
		return y;
	if(i==3)
		return z;
	return *((double *)this+i);
}

void Quaternion::setW(double _w){
	w=_w;
}

void Quaternion::setX(double _x){
	x=_x;
}

void Quaternion::setY(double _y){
	y=_y;
}

void Quaternion::setZ(double _z){
	z=_z;
}

void Quaternion::setVector(const Vector3 &p){
	x=p.getX();
	y=p.getY();
	z=p.getZ();
}

double Quaternion::getW()const{
	return w;
}

double Quaternion::getX()const{
	return x;
}

double Quaternion::getY()const{
	return y;
}

double Quaternion::getZ()const{
	return z;
}

Vector3 Quaternion::getVector()const{
	return Vector3(x,y,z);
}

double Quaternion::getAngle()const{
	if(sqrNorm()>0.0f)
		return 2.0f*acos(w);
	else
		return 0.0f;
}

Vector3 Quaternion::getAxis()const{
	double l=sqrNorm();
	if(l>0.0f)
		return getVector()/sqrt(l);
	else
		return Vector3(1,0,0);
}

Quaternion operator+(const Quaternion &q1,const Quaternion &q2){
	return Quaternion(q1.w+q2.w,q1.x+q2.x,q1.y+q2.y,q1.z+q2.z);
}

Quaternion operator-(const Quaternion &q1,const Quaternion &q2){
	return Quaternion(q1.w-q2.w,q1.x-q2.x,q1.y-q2.y,q1.z-q2.z);
}

Quaternion operator*(const Quaternion &q1,double scale){
	return Quaternion(q1.w*scale,q1.x*scale,q1.y*scale,q1.z*scale);
}

Quaternion operator*(double scale,const Quaternion &q1){
	return Quaternion(q1.w*scale,q1.x*scale,q1.y*scale,q1.z*scale);
}

Quaternion operator*(const Quaternion &q1,const Quaternion &q2){
	return Quaternion(q1.w*q2.w-q1.x*q2.x-q1.y*q2.y-q1.z*q2.z,
		q1.w*q2.x+q1.x*q2.w+q1.y*q2.z-q1.z*q2.y,
		q1.w*q2.y+q1.y*q2.w+q1.z*q2.x-q1.x*q2.z,
		q1.w*q2.z+q1.z*q2.w+q1.x*q2.y-q1.y*q2.x);
}

Quaternion operator/(const Quaternion &q1,double scale){
	return Quaternion(q1.w/scale,q1.x/scale,q1.y/scale,q1.z/scale);
}

double dot(const Quaternion &q1,const Quaternion &q2){
	return q1.w*q2.w+q1.x*q2.x+q1.y*q2.y+q1.z*q2.z;
}

bool operator==(const Quaternion &q1,const Quaternion &q2){
	if(q1.w!=q2.w)
		return false;
	if(q1.x!=q2.x)
		return false;
	if(q1.y!=q2.y)
		return false;
	if(q1.z!=q2.z)
		return false;
	return true;
}

bool operator!=(const Quaternion &q1,const Quaternion &q2){
	return !(q1==q2);
}

Quaternion log(const Quaternion &q1){
	double q=q1.norm();
	double v=acos(q1.w/q)/sqrt(q1.x*q1.x+q1.y*q1.y+q1.z*q1.z);
	return Quaternion(log(q),q1.x*v,q1.y*v,q1.z*v);
}

Quaternion exp(const Quaternion &q1){
	double v=sqrt(q1.x*q1.x+q1.y*q1.y+q1.z*q1.z);
	double t=sin(v)/v;
	return Quaternion(cos(v),q1.x*t,q1.y*t,q1.z*t)*exp(q1.w);
}

const Quaternion &Quaternion::operator+()const{
	return *this;
}

Quaternion Quaternion::operator-()const{
	return Quaternion(-w,-x,-y,-z);
}

Quaternion &Quaternion::operator+=(const Quaternion &q1){
	*this=*this+q1;
	return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &q1){
	*this=*this-q1;
	return *this;
}

Quaternion &Quaternion::operator*=(double scale){
	*this=*this*scale;
	return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &q1){
	*this=*this*q1;
	return *this;
}

Quaternion &Quaternion::operator/=(double scale){
	*this=*this/scale;
	return *this;
}

double Quaternion::norm()const{
	return sqrt(w*w+x*x+y*y+z*z);
}

double Quaternion::sqrNorm()const{
	return w*w+x*x+y*y+z*z;
}

Quaternion Quaternion::inverse()const{
	return Quaternion(w,-x,-y,-z)/sqrNorm();
}

Vector3 applayRotate(Quaternion &q,Vector3 &p){
	Quaternion res=q*Quaternion(0.0f,p)*q.inverse();
	return res.getVector();
}

Quaternion slerp(double t,const Quaternion &p,const Quaternion &q){
	double dcos2=dot(p,q);
	dcos2=dcos2*dcos2/(p.sqrNorm()*q.sqrNorm());
	if(dcos2>1.0f)
		dcos2=1.0f;
	double dsin=sqrt(1-dcos2);
	double dangle=asin(dsin);
	double dC0=sin((1-t)*dangle)/dsin;
	double dC1=sin(t*dangle)/dsin;
	return dC0*p+dC1*q;
}