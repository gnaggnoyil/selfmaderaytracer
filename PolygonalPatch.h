#ifndef _POLYGONAL_PATCH_H_
#define _POLYGONAL_PATCH_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include "Vector3.h"
#include <vector>

/**
 * this is the polygonal patch class inherited form the abstract shape class
 * what's the different between polygon and polygonal patch?
 */
class PolygonalPatch:public Shape{
public:
	// constructors
	PolygonalPatch(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const Material &_material,const DynamicVector3Type &_animation);
	PolygonalPatch(const PolygonalPatch &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	Vector3 getVertex(int i)const;
	int getVertexNum()const;
	void setNormal(int i,const Vector3 &_normal);
	Vector3 getNormal(int i)const;
	int getNormalNum()const;
	void setMaterial(const Material &_material);
	Material getMaterial()const;
	void setAnimation(const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation()const;

	void addVertex(const Vector3 &_vertex);
	void addNormal(const Vector3 &_normal);

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmax,double time,HitRecord &record)const;
private:
	std::vector<Vector3> vertex;
	std::vector<Vector3> normal;
	Material material;
	DynamicVector3Type animation;	// the function that calculates the transform shift of this shape at a certain time
};
#endif // _POLYGONAL_PATCH_H_