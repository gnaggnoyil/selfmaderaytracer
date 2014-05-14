#include "PolygonalPatch.h"
#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include <vector>
#include "Vector2.h"
#include <algorithm>

PolygonalPatch::PolygonalPatch(const std::vector<Vector3> &_vertex,const std::vector<Vector3> &_normal,const Material &_material,
	const DynamicVector3Type &_animation)
	:vertex(_vertex),normal(_normal),material(_material),animation(_animation){}

PolygonalPatch::PolygonalPatch(const PolygonalPatch &_orig)
	:material(_orig.getMaterial()),animation(_orig.getAnimation()){
	for(int i=0;i<_orig.getVertexNum();i++)
		vertex.push_back(_orig.getVertex(i));
}

void PolygonalPatch::setVertex(int i,const Vector3 &_vertex){
	vertex[i]=_vertex;
}

Vector3 PolygonalPatch::getVertex(int i)const{
	return vertex[i];
}

int PolygonalPatch::getVertexNum()const{
	return vertex.size();
}

void PolygonalPatch::setNormal(int i,const Vector3 &_normal){
	normal[i]=_normal;
}

Vector3 PolygonalPatch::getNormal(int i)const{
	return normal[i];
}

int PolygonalPatch::getNormalNum()const{
	return normal.size();
}

void PolygonalPatch::setMaterial(const Material &_material){
	material=_material;
}

Material PolygonalPatch::getMaterial()const{
	return material;
}

void PolygonalPatch::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type PolygonalPatch::getAnimation()const{
	return animation;
}

void PolygonalPatch::addVertex(const Vector3 &_vertex){
	vertex.push_back(_vertex);
}

void PolygonalPatch::addNormal(const Vector3 &_normal){
	normal.push_back(_normal);
}

bool PolygonalPatch::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	std::vector<Vector3> _vertex;
	std::vector<Vector3> _normal;
	for(int i=0;i<vertex.size();i++){
		_vertex.push_back(animation(vertex[i],time));
		_normal.push_back(animation(vertex[i]+normal[i],time)-_vertex[i]);
	}

	Vector3 n=cross(_vertex[1]-_vertex[0],_vertex[2]-_vertex[1]);
	double t=(dot(n,*_vertex.end())-dot(n,r.getOrigin()))/dot(n,r.getDirection());

	if((t<=0.0f)||(t>tmax))
		return false;

	Vector3 hit=r.pointAt(t);
	double theta=0.0f;
	for(int i=0;i<_vertex.size();i++){
		Vector3 e1=_vertex[i]-hit;
		Vector3 e2=_vertex[(i+1)%_vertex.size()]-hit;
		double cos0=dot(e1,e2)/(e1.length()*e2.length());
		theta+=acos(cos0);
	}
	if(fabs(theta-2.0f*M_PI)<DBL_EPSILON){
		record.t=t;
		record.hitpoint=hit;
		record.material=std::make_shared<Material>(material);
		record.UVcoord=Vector2(0,0);
		for_each(_normal.begin(),_normal.end(),[&record](Vector3 p){record.normal+=p;});
		record.normal=normalize(record.normal);
		return true;
	}
	return false;
}