#include "Sphere.h"
#include "Ray.h"
#include "Material.h"
#include "Vector3.h"
#include "Common.h"

Sphere::Sphere(const Vector3 &_ocenter,double _radius,const Material &_material,const DynamicVector3Type &_animation)
	:ocenter(_ocenter),radius(_radius),material(_material),animation(_animation){}

Sphere::Sphere(const Sphere &_orig)
	:ocenter(_orig.getOCenter()),radius(_orig.getRadius()),material(_orig.getMaterial()),animation(_orig.getAnimation()){}

void Sphere::setOCenter(const Vector3 &_ocenter){
	ocenter=_ocenter;
}

Vector3 Sphere::getOCenter()const{
	return ocenter;
}

void Sphere::setRadius(double _radius){
	radius=_radius;
}

double Sphere::getRadius()const{
	return radius;
}

void Sphere::setMaterial(const Material &_material){
	material=_material;
}

Material Sphere::getMaterial()const{
	return material;
}

void Sphere::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type Sphere::getAnimation()const{
	return animation;
}

bool Sphere::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	Vector3 center=animation(ocenter,time);
	Vector3 temp=r.getOrigin()-center;

	double a=dot(r.getDirection(),r.getDirection());
	double b=2.0f*dot(r.getDirection(),temp);
	double c=dot(temp,temp)-radius*radius;

	double discriminant=b*b-4*a*c;
	if(discriminant>0.0f){
		bool inhit=false;
		discriminant=sqrt(discriminant);
		double t=(-b-discriminant)/(2.0f*a);
		if(t<0.0f){
			inhit=true;
			t=(-b+discriminant)/(2.0f*a);
		}
		if((t<0.0f)||(t>tmax))
			return false;

		record.hitpoint=r.pointAt(t);
		if(inhit)
			record.normal=center-record.hitpoint;
		else
			record.normal=record.hitpoint-center;
		record.normal/=radius;
		record.t=t;
		double twopi=M_PI*2;
		double theta=acos(record.normal.getZ());
		double phi=atan2(record.normal.getY(),record.normal.getX());
		if(phi<0.0f)
			phi+=twopi;
		record.UVcoord=Vector2(phi/twopi,(M_PI-theta)/M_PI);
		record.material=std::make_shared<Material>(material);
		return true;
	}

	return false;
}