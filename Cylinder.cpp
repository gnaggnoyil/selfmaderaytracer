#include "Cylinder.h"
#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"

Cylinder::Cylinder(const Vector3 &_base,double _baseRadius,const Vector3 &_apex,double _apexRadius,const Material &_material,const DynamicVector3Type &_animation)
	:base(_base),baseRadius(_baseRadius),apex(_apex),apexRadius(_apexRadius),material(_material),animation(_animation){
	if((baseRadius<0.0f)&&(apexRadius<0.0f))
		inside=true;
	else
		inside=false;
}

Cylinder::Cylinder(const Cylinder &_orig)
	:base(_orig.getBase()),baseRadius(_orig.getBaseRadius()),apex(_orig.getApex()),apexRadius(_orig.getApexRadius()),material(_orig.getMaterial()),
	animation(_orig.getAnimation()){
	if((baseRadius<0.0f)&&(apexRadius<0.0f))
		inside=true;
	else
		inside=false;
}

void Cylinder::setBase(const Vector3 &_base){
	base=_base;
}

Vector3 Cylinder::getBase()const{
	return base;
}

void Cylinder::setBaseRadius(double _baseRadius){
	baseRadius=_baseRadius;
}

double Cylinder::getBaseRadius()const{
	return baseRadius;
}

void Cylinder::setApex(const Vector3 &_apex){
	apex=_apex;
}

Vector3 Cylinder::getApex()const{
	return apex;
}

void Cylinder::setApexRadius(double _apexRadius){
	apexRadius=_apexRadius;
}

double Cylinder::getApexRadius()const{
	return apexRadius;
}

void Cylinder::setMaterial(const Material &_material){
	material=_material;
}

Material Cylinder::getMaterial()const{
	return material;
}

void Cylinder::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type Cylinder::getAnimation()const{
	return animation;
}

bool Cylinder::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	Vector3 _base=animation(base,time);
	Vector3 _apex=animation(apex,time);

	/*Vector3 _orig=baseRadius/apexRadius*_apex+(apexRadius-baseRadius)/apexRadius*_base;
	Vector3 d=base-apex;
	Vector3 o=r.getOrigin()-_orig;
	Vector3 dir=r.getDirection();
	double tan2=sqr(baseRadius-apexRadius)/dot(d,d);
	double cos2=1/(tan2+1);

	double a=sqr(dot(dir,d))-dot(dir,dir)*dot(d,d)*cos2;
	double b=2.0f*dot(dir,d)*dot(o,d)-sqr(dot(dir,o))*dot(d,d)*cos2;
	double c=sqr(dot(o,d))-dot(o,o)*dot(d,d)*cos2;*/

	Vector3 pa=base+(apex-base)*baseRadius/(baseRadius-apexRadius);
	Vector3 _orig=pa;
	Vector3 d=apex-base;
	double tan2=sqr(apexRadius-baseRadius)/d.sqrLength();
	Vector3 va=normalize(d);

	Vector3 p=r.getOrigin();
	Vector3 v=r.getDirection();
	double cos2=1/(tan2+1);
	double sin2=1-cos2;
	Vector3 deltap=p-pa;

	double a=cos2*sqr(v-dot(v,va)*va)-sin2*sqr(dot(v,va));
	double b=2*cos2*dot(v-dot(v,va)*va,deltap-dot(deltap,va)*va)-2*sin2*dot(v,va)*dot(deltap,va);
	double c=cos2*sqr(deltap-dot(deltap,va)*va)-sin2*sqr(dot(deltap,va));

	double discriminant=b*b-4*a*c;
	if(discriminant>=0.0f){
		discriminant=sqrt(discriminant);
		double t1=(-b-discriminant)/(2.0f*a);
		double t2=(-b+discriminant)/(2.0f*a);
		auto lambda=[&](const Vector3 &pp){
			double t=-dot(pp-_orig,d)/d.length();
			if(t<apexRadius/(baseRadius-apexRadius)*d.length())
				return false;
			if(t>baseRadius/(baseRadius-apexRadius)*d.length())
				return false;
			return true;
		};

		if(!lambda(r.pointAt(t1)))
			t1=tmax+1.0f;
		if(!lambda(r.pointAt(t2)))
			t2=tmax+1.0f;
		if(t1<=0.0f)
			t1=tmax+1.0f;
		if(t2<=0.0f)
			t2=tmax+0.0f;
		double t=(t1<t2)?t1:t2;
		if(t1>tmax)
			return false;

		record.t=t;
		record.hitpoint=r.pointAt(t);
		record.material=std::make_shared<Material>(material);
		Vector3 l=cross(d,record.hitpoint-_orig);
		record.normal=normalize(cross(l,record.hitpoint-_orig));
		if(inside)
			record.normal=-record.normal;
		d=-d;
		Vector3 U=cross(d,Vector3(1.0f,0.0f,0.0f));
		if(U.length()<DBL_EPSILON)
			U=cross(d,Vector3(0.0f,1.0f,0.0f));
		Vector3 p=r.pointAt(t)-base;
		double sintheta2=sqr(dot(p,d))/(p.sqrLength()*d.sqrLength());
		double costheta2=sqrt(1-sintheta2);
		double costheta1=dot(p,U)/(p.length()*U.length());
		double theta=acos(costheta1/costheta2);
		record.UVcoord=Vector2(theta*2.0f/M_PI,dot(p,d)/d.length());
		return true;
	}
	return false;
}