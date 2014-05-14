#ifndef _TRIANGLE_PATCH_H_
#define _TRIANGLE_PATCH_H_ 1

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
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
		const Vector2 &uv1,const Vector2 &uv2,const Vector2 &uv3,const Material &_material,
		const DynamicVector3Type &a1,const DynamicVector3Type &a2,const DynamicVector3Type &a3);
	TrianglePatch(const TrianglePatch &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	Vector3 getVertex(int i)const;
	void setNormal(int i,const Vector3 &_normal);
	Vector3 getNormal(int i)const;
	void setTexCoord(int i,const Vector2 &_texCoord);
	Vector2 getTexCoord(int i)const;
	void setMaterial(const Material &_material);
	Material getMaterial()const;
	void setAnimation(int i,const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation(int i)const;

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmax,double time,HitRecord &record)const;
private:
	Vector3 vertex[3];
	Vector3 normal[3];
	Vector2 texCoord[3];
	Material material;
	DynamicVector3Type animation[3];	// the function that calculates the transform shift of this shape at a certain time
};
#endif // _TRIANGLE_PATCH_H_