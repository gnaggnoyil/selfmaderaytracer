#ifndef _MESH_TRIANGLE_H_
#define _MESH_TRIANGLE_H_ 1

#include "Mesh.h"
#include "Shape.h"
#include "Ray.h"
#include "Vector3.h"
#include <memory>

/**
 * this class defines mesh triangles that uses a specfic mesh.
 * inherted from Shape class
 */
class MeshTriangle:public Shape{
public:
	// constructors
	MeshTriangle(int p0,int p1,int p2,int n0,int n1,int n2,int t0,int t1,int t2,std::tr1::shared_ptr<Mesh> &_mesh,
		const DynamicVector3Type &a1,const DynamicVector3Type &a2,const DynamicVector3Type &a3);
	MeshTriangle(const MeshTriangle &_orig);

	void setVertexIndex(int i,int _vertexIndex);
	int getVertexIndex(int i)const;
	void setNormalIndex(int i,int _normalIndex);
	int getNormalIndex(int i)const;
	void setTexCoordIndex(int i,int _texCoordIndex);
	int getTexCoordIndex(int i)const;
	void setMesh(std::tr1::shared_ptr<Mesh> _mesh);
	std::tr1::shared_ptr<Mesh> getMesh()const;
	void setAnimation(int i,const DynamicVector3Type &_animation);
	DynamicVector3Type getAnimation(int i)const;

	// implements the hit method of the Shape class
	bool hit(const Ray &r,double tmax,double time,HitRecord &record)const;
private:
	std::tr1::shared_ptr<Mesh> mesh;
	int vertexIndex[3];
	int normalIndex[3];
	int texCoordIndex[3];
	DynamicVector3Type animation[3];	// the function that calculates the transform shift of this shape at a certain time
};
#endif // _MESH_TRIANGLE_H_