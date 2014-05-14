#include "Mesh.h"
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <memory>
#include "Material.h"

Mesh::Mesh(const std::vector<Vector3> &_vertex,const Material &_material)
	:vertex(_vertex),material(_material){}

Mesh::Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const Material &_material)
	:vertex(_vertex),normal(_normal),material(_material){}

Mesh::Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector2> &_texCoord,const Material &_material)
	:vertex(_vertex),texCoord(_texCoord),material(_material){}

Mesh::Mesh(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const std::vector<Vector2> &_texCoord,const Material &_material)
	:vertex(_vertex),normal(_normal),texCoord(_texCoord),material(_material){}

Mesh::Mesh(const Mesh &_orig)
	:material(_orig.getMaterial()){
	for(int i=0;i<_orig.getVertexNum();i++)
		vertex.push_back(_orig.getVertex(i));
	for(int i=0;i<_orig.getNormalNum();i++)
		normal.push_back(_orig.getNormal(i));
	for(int i=0;i<_orig.getTexCoordNum();i++)
		texCoord.push_back(_orig.getTexCoord(i));
}

void Mesh::setVertex(int i,const Vector3 &_vertex){
	vertex[i]=_vertex;
}

Vector3 Mesh::getVertex(int i)const{
	return vertex[i];
}

int Mesh::getVertexNum()const{
	return vertex.size();
}

void Mesh::setNormal(int i,const Vector3 &_normal){
	normal[i]=_normal;
}

Vector3 Mesh::getNormal(int i)const{
	return normal[i];
}

int Mesh::getNormalNum()const{
	return normal.size();
}

void Mesh::setTexCoord(int i,const Vector2 &_texCoord){
	texCoord[i]=_texCoord;
}

Vector2 Mesh::getTexCoord(int i)const{
	return texCoord[i];
}

int Mesh::getTexCoordNum()const{
	return texCoord.size();
}

void Mesh::setMaterial(const Material &_material){
	material=_material;
}

Material Mesh::getMaterial()const{
	return material;
}

bool Mesh::hasNormal()const{
	return normal.size()>0;
}

bool Mesh::hasTexCoord()const{
	return texCoord.size()>0;
}