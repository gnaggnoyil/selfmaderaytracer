#ifndef _POLYGONAL_PATCH_H_
#define _POLYGONAL_PATCH_H_

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include <vector>

/**
 * this is the polygonal patch class inherited form the abstract shape class
 * what's the different between polygon and polygonal patch?
 */
class PolygonalPatch:public Shape{
public:
	// constructors
	PolygonalPatch(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const Material &_material,DynamicVector3Type _animation);
	PolygonalPatch(const PolygonalPatch &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	std::vector<Vector3> getVertex(int i);
	void setNormal(int i,const Vector3 &_normal);
	std::vector<Vector3> getNormal(int i);
	void setMaterial(const Material &_material);
	Material getMaterial()const;
	void setAnimation(const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation()const;

	void addVertex(const Vector3 &_vertex);
	int getVertexNumber();
	void addNormal(const Vector3 &_normal);
	int getNormalNumber();

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmin,double tmax,double time,HitRecord &record)const;
private:
	std::vector<Vector3> vertex;
	std::vector<Vector3> normal;
	std::tr1::shared_ptr<Material> Material;
	DynamicVector3Type animation;	// the function that calculates the transform shift of this shape at a certain time
};
#endif // _POLYGONAL_PATCH_H_