#ifndef _TRIANGLE_PATCH_H_
#define _TRIANGLE_PATCH_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include "Vector2.h"

/**
 * this class defines a triangle patch that can have textures and animations,
 * inherted from abstract shape class
 * what's the different between triangle and triangle patch?
 */
class TrianglePatch:public Shape{
public:
	// constructors
	TrianglePatch(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &n1,const Vector3 &n2,const Vector3 &n3,
		const Vector2 &uv1,const Vector2 &uv2,const Vector3 &uv3,const Material &_material,DynamicVector3Type _animation);
	TrianglePatch(const TrianglePatch &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	Vector3 getVertex(int i);
	void setNormal(int i,const Vector3 &_normal);
	void getNormal(int i);
	void setTexCoord(int i,const Vector2 &_texCoord);
	Vector2 getTexCoord(int i);
	void setMaterial(const Material &_material);
	Material getMaterial()const;

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmin,double tmax,double time,HitRecord &record);
private:
	Vector3 vertex[3];
	Vector3 normal[3];
	Vector2 texCoord[3];
	Material material;
};
#endif // _TRIANGLE_PATCH_H_