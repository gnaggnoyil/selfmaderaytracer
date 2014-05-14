#include "Polygon.h"
#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include <vector>
#include "Vector2.h"
#include <algorithm>

Polygon::Polygon(const std::vector<Vector3> &_vertex,const Material &_material,const DynamicVector3Type &_animation)
	:vertex(_vertex),material(_material),animation(_animation){}

Polygon::Polygon(const Polygon &_orig)
	:material(_orig.getMaterial()),animation(_orig.getAnimation()){
	for(int i=0;i<_orig.getVertexNum();i++)
		vertex.push_back(_orig.getVertex(i));
}

void Polygon::setVertex(int i,const Vector3 &_vertex){
	vertex[i]=_vertex;
}

Vector3 Polygon::getVertex(int i)const{
	return vertex[i];
}

int Polygon::getVertexNum()const{
	return vertex.size();
}

void Polygon::setMaterial(const Material &_material){
	material=_material;
}

Material Polygon::getMaterial()const{
	return material;
}

void Polygon::setAnimation(const DynamicVector3Type &_animation){
	animation=_animation;
}

DynamicVector3Type Polygon::getAnimation()const{
	return animation;
}

void Polygon::addVertex(const Vector3 &_vertex){
	vertex.push_back(_vertex);
}

bool Polygon::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	std::vector<Vector3> _vertex;
	for(auto it=vertex.begin();it!=vertex.end();++it)
		_vertex.push_back(animation(*it,time));

	Vector3 n=cross(_vertex[1]-_vertex[0],_vertex[2]-_vertex[1]);
	double t=(dot(n,vertex[vertex.size()-1])-dot(n,r.getOrigin()))/dot(n,r.getDirection());

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
	if(fabs(theta-2.0f*M_PI)<EPSILON){
		record.t=t;
		record.hitpoint=hit;
		record.material=std::make_shared<Material>(material);
		record.UVcoord=Vector2(0,0);
		record.normal=normalize(n);
		return true;
	}
	return false;
}