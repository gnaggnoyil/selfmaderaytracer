#ifndef _POLYGON_H_
#define _POLYGON_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include <vector>

/**
 * this is the polygon class inherited form the abstract shape class
 */
class Polygon:public Shape{
public:
	// constructors
	Polygon(const std::vector<Vector3> &_vertex,const Material &_material,DynamicVector3Type _animation);
	Polygon(const Polygon &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	std::vector<Vector3> getVertex(int i);
	void setMaterial(const Material &_material);
	Material getMaterial()const;
	void setAnimation(const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation()const;

	void addVertex(const Vector3 &_vertex);
	int getVertexNumber();

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmin,double tmax,double time,HitRecord &record)const;
private:
	std::vector<Vector3> vertex;
	Material material;
	DynamicVector3Type animation;	// the function that calculates the transform shift of this shape at a certain time
};
#endif // _POLYGON_H_