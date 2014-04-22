#ifndef _CYLINDER_H_
#define _CYLINDER_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"

/**
 * this is the cylinder class inherited form the abstract shape class
 * cone can be treated as a special kind of cylinder
 * note the surfaces does not include endcaps
 */
class Cylinder:public Shape{
public:
	// constructors
	Cylinder(const Vector3 &_base,double _baseRadius,const Vector3 &_apex,double _apexRadius,const Material &_material,DynamicVector3Type _animation);
	Cylinder(const Cylinder &_orig);

	void setBase(const Vector3 &_base);
	Vector3 getBase()const;
	void setBaseRadius(double _baseRadius);
	double getBaseRadius()const;
	void setApex(const Vector3 &_apex);
	Vector3 getApex()const;
	void setApexRadius(double _apexRadius);
	double getApexRadius()const;
	void setMaterial(const Material &_material);
	Material getMaterial()const;

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmin,double tmax,double time,HitRecord &record);
private:
	Vector3 base;
	double baseRadius;
	Vector3 apex;
	double apexRadius;
	Material material;
};
#endif // _CYLINDER_H_