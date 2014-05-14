#include "MeshTriangle.h"
#include "Ray.h"
#include "Vector3.h"
#include <memory>

MeshTriangle::MeshTriangle(int p0,int p1,int p2,int n0,int n1,int n2,int t0,int t1,int t2,std::tr1::shared_ptr<Mesh> &_mesh,
	const DynamicVector3Type &a0,const DynamicVector3Type &a1,const DynamicVector3Type &a2)
	:mesh(_mesh){
	vertexIndex[0]=p0;
	vertexIndex[1]=p1;
	vertexIndex[2]=p2;
	normalIndex[0]=n0;
	normalIndex[1]=n1;
	normalIndex[2]=n2;
	texCoordIndex[0]=t0;
	texCoordIndex[1]=t1;
	texCoordIndex[2]=t2;
	animation[0]=a0;
	animation[1]=a1;
	animation[2]=a2;
}

MeshTriangle::MeshTriangle(const MeshTriangle &_orig)
	:mesh(_orig.getMesh()){
	for(int i=0;i<3;i++)
		vertexIndex[i]=_orig.getVertexIndex(i);
	if(mesh->hasNormal())
		for(int i=0;i<3;i++)
			normalIndex[i]=_orig.getNormalIndex(i);
	if(mesh->hasTexCoord())
		for(int i=0;i<3;i++)
			texCoordIndex[i]=_orig.getTexCoordIndex(i);
	for(int i=0;i<3;i++)
		animation[i]=_orig.getAnimation(i);
}

void MeshTriangle::setVertexIndex(int i,int _vertexIndex){
	vertexIndex[i]=_vertexIndex;
}

int MeshTriangle::getVertexIndex(int i)const{
	return vertexIndex[i];
}

void MeshTriangle::setNormalIndex(int i,int _normalIndex){
	normalIndex[i]=_normalIndex;
}

int MeshTriangle::getNormalIndex(int i)const{
	return normalIndex[i];
}

void MeshTriangle::setTexCoordIndex(int i,int _texCoordIndex){
	texCoordIndex[i]=_texCoordIndex;
}

int MeshTriangle::getTexCoordIndex(int i)const{
	return texCoordIndex[i];
}

void MeshTriangle::setMesh(std::tr1::shared_ptr<Mesh> _mesh){
	mesh=_mesh;
}

std::tr1::shared_ptr<Mesh> MeshTriangle::getMesh()const{
	return mesh;
}

void MeshTriangle::setAnimation(int i,const DynamicVector3Type &_animation){
	animation[i]=_animation;
}

DynamicVector3Type MeshTriangle::getAnimation(int i)const{
	return animation[i];
}

bool MeshTriangle::hit(const Ray &r,double tmax,double time,HitRecord &record)const{
	Vector3 v0=mesh->getVertex(vertexIndex[0]);
	Vector3 v1=mesh->getVertex(vertexIndex[1]);
	Vector3 v2=mesh->getVertex(vertexIndex[2]);

	Vector3 p0=animation[0](v0,time);
	Vector3 p1=animation[1](v1,time);
	Vector3 p2=animation[2](v2,time);

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
		record.material=std::make_shared<Material>(mesh->getMaterial());
		record.hitpoint=r.pointAt(t);
		if(mesh->hasNormal()){
			Vector3 n0=animation[0](v0+mesh->getNormal(normalIndex[0]),time)-p0;
			Vector3 n1=animation[1](v1+mesh->getNormal(normalIndex[1]),time)-p1;
			Vector3 n2=animation[2](v2+mesh->getNormal(normalIndex[2]),time)-p2;
			record.normal=normalize(n0+n1+n2);
		}
		else
			record.normal=normalize(cross(p1-p0,p2-p0));
		if(mesh->hasTexCoord()){
			Vector2 t0=mesh->getTexCoord(texCoordIndex[0]);
			Vector2 t1=mesh->getTexCoord(texCoordIndex[1]);
			Vector2 t2=mesh->getTexCoord(texCoordIndex[2]);
			record.UVcoord=(1.0f-beta-gamma)*t0+beta*t1+gamma*t2;
		}
		else
			record.UVcoord=Vector2(0.0f,0.0f);
		return true;
	}

	return false;
}