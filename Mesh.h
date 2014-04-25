#ifndef _MESH_H_
#define _MESH_H_ 1

#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <memory>
#include "Material.h"

/**
 * this class defines a mesh class with a set of vertexs and normals and texture coordinates
 */
class Mesh{
public:
	// constructors
	Mesh(const std::vector<Vector3> &_vertex,const Material &_material);
	Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const Material &_material);
	Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector2> &_texCoord,const Material &_material);
	Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const std::vector<Vector2> &_texCoord,const Material &_material);
	Mesh(const Mesh &_orig);

	void setVertex(int i,const Vector3 &_vertex);
	Vector3 getVertex(int i);
	void setNormal(int i,const Vector3 &_normal);
	Vector3 getNormal(int i);
	void setTexCoord(int i,const Vector2 &_texCoord);
	Vector2 getTexCoord(int i);
	void setMaterial(const Material &_material);
	Material getMaterial()const;

	bool hasNormal()const;
	bool hasTexCoord()const;
private:
	std::vector<Vector3> vertex;
	std::vector<Vector3> normal;
	std::vector<Vector2> texCoord;
	std::tr1::shared_ptr<Material> material;
};
#endif // _MESH_H_