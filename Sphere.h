#ifndef _SPHERE_H_
#define _SPHERE_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"

/**
 * this is the sphere class inherited form the abstract shape class
 */
class Sphere:public Shape{
public:
	// constructors
	Sphere(const Vector3 &_ocenter,const Material &_material,DynamicVector3Type _animation);
	Sphere(const Sphere &_orig);

	void setOCenter(const Vector3 &_ocenter);
	Vector3 getOCenter()const;
	void setMaterial(const Material &_material);
	Material getMaterial()const;
	void setRadius(double _radius);
	double getRadius()const;

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmin,double tmax,double time,HitRecord &record)const;
private:
	Material material;
	Vector3 ocenter;
	double radius;
};
#endif // _SPHERE_H_