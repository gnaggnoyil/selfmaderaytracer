#include "Triangle.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"
#include "Material.h"
#include <memory>

Triangle::Triangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector2 &uv1,const Vector2 &uv2,const Vector2 &uv3,
	const Material &_material,const DynamicVector3Type &a1,const DynamicVector3Type &a2,const DynamicVector3Type &a3)
	:material(_material){
	vertex[0]=p1;
	vertex[1]=p2;
	vertex[2]=p3;
	texCoord[0]=uv1;
	texCoord[1]=uv2;
	texCoord[2]=uv3;
	animation[0]=a1;
	animation[1]=a2;
	animation[2]=a3;
}

Triangle::Triangle(const Triangle &_orig)
	:material(_orig.getMaterial()){
	for(int i=0;i<3;i++){
		vertex[i]=_orig.getVertex(i);
		texCoord[i]=_orig.getTexCoord(i);
		animation[i]=_orig.getAnimation(i);
	}
}

void Triangle::setVertex(int i,const Vector3 &_vertex){
	vertex[i]=_vertex;
}

Vector3 Triangle::getVertex(int i)const{
	return vertex[i];
}

void Triangle::setTexCoord(int i,const Vector2 &_texCoord){
	texCoord[i]=_texCoord;
}

Vector2 Triangle::getTexCoord(int i)const{
	return texCoord[i];
}

void Triangle::setMaterial(const Material &_material){
	material=_material;
}

Material Triangle::getMaterial()const{
	return material;
}

void Triangle::setAnimation(int i,const DynamicVector3Type &_animation){
	animation[i]=_animation;
}

DynamicVector3Type Triangle::getAnimation(int i)const{
	return animation[i];
}

bool Triangle::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	Vector3 p0=animation[0](vertex[0],time);
	Vector3 p1=animation[1](vertex[1],time);
	Vector3 p2=animation[2](vertex[2],time);

	double A=p0.getX()-p1.getX();
	double B=p0.getY()-p1.getY();
	double C=p0.getZ()-p1.getZ();

	double D=p0.getX()-p2.getX();
	double E=p0.getY()-p2.getY();
	double F=p0.getZ()-p2.getZ();

	double G=r.getDirection().getX();
	double H=r.getDirection().getY();
	double I=r.getDirection().getZ();

	double J=p0.getX()-r.getOrigin().getX();
	double K=p0.getY()-r.getOrigin().getY();
	double L=p0.getZ()-r.getOrigin().getZ();

	double EIHF=E*I-H*F;
	double GFDI=G*F-D*I;
	double DHEG=D*H-E*G;

	double denom=(A*EIHF+B*GFDI+C*DHEG);

	double beta=(J*EIHF+K*GFDI+L*DHEG)/denom;
	if((beta<=0.0f)||(beta>=1.0f))
		return false;

	double AKJB=A*K-J*B;
	double JCAL=J*C-A*L;
	double BLKC=B*L-K*C;

	double gamma=(I*AKJB+H*JCAL+G*BLKC)/denom;
	if((gamma<=0.0f)||(gamma>=1.0f))
		return false;

	double t=-(F*AKJB+E*JCAL+D*BLKC)/denom;
	if((t>=0.0f)&&(t<=tmax)){
		record.t=t;
		record.material=std::make_shared<Material>(material);
		record.hitpoint=r.pointAt(t);
		record.normal=normalize(cross(p1-p0,p2-p0));
		record.UVcoord=(1.0f-beta-gamma)*texCoord[0]+beta*texCoord[1]+gamma*texCoord[2];
		return true;
	}

	return false;
}