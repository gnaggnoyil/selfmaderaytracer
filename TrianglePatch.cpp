#include "TrianglePatch.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include "Vector3.h"
#include "Vector2.h"

TrianglePatch::TrianglePatch(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &n1,const Vector3 &n2,const Vector3 &n3,
	const Vector2 &uv1,const Vector2 &uv2,const Vector2 &uv3,const Material &_material,
	const DynamicVector3Type &a1,const DynamicVector3Type &a2,const DynamicVector3Type &a3)
	:material(_material){
	vertex[0]=p1;
	vertex[1]=p2;
	vertex[2]=p3;
	normal[0]=n1;
	normal[1]=n2;
	normal[2]=n3;
	texCoord[0]=uv1;
	texCoord[1]=uv2;
	texCoord[2]=uv3;
	animation[0]=a1;
	animation[1]=a2;
	animation[2]=a3;
}

TrianglePatch::TrianglePatch(const TrianglePatch &_orig)
	:material(_orig.getMaterial()){
	for(int i=0;i<3;i++){
		vertex[i]=_orig.getVertex(i);
		normal[i]=_orig.getNormal(i);
		texCoord[i]=_orig.getTexCoord(i);
		animation[i]=_orig.getAnimation(i);
	}
}

void TrianglePatch::setVertex(int i,const Vector3 &_vertex){
	vertex[i]=_vertex;
}

Vector3 TrianglePatch::getVertex(int i)const{
	return vertex[i];
}

void TrianglePatch::setNormal(int i,const Vector3 &_normal){
	normal[i]=_normal;
}

Vector3 TrianglePatch::getNormal(int i)const{
	return normal[i];
}

void TrianglePatch::setTexCoord(int i,const Vector2 &_texCoord){
	texCoord[i]=_texCoord;
}

Vector2 TrianglePatch::getTexCoord(int i)const{
	return texCoord[i];
}

void TrianglePatch::setMaterial(const Material &_material){
	material=_material;
}

Material TrianglePatch::getMaterial()const{
	return material;
}

void TrianglePatch::setAnimation(int i,const DynamicVector3Type &_animation){
	animation[i]=_animation;
}

DynamicVector3Type TrianglePatch::getAnimation(int i)const{
	return animation[i];
}

bool TrianglePatch::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	Vector3 p0=animation[0](vertex[0],time);
	Vector3 p1=animation[1](vertex[1],time);
	Vector3 p2=animation[2](vertex[2],time);

	Vector3 n0=animation[0](vertex[0]+normal[0],time)-p0;
	Vector3 n1=animation[1](vertex[1]+normal[1],time)-p1;
	Vector3 n2=animation[2](vertex[2]+normal[2],time)-p2;

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
		record.normal=normalize(n0+n1+n2);
		record.UVcoord=(1.0f-beta-gamma)*texCoord[0]+beta*texCoord[1]+gamma*texCoord[2];
		return true;
	}

	return false;
}