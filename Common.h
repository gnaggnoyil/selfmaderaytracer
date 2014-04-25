#ifndef _COMMON_H_
#define _COMMON_H_ 1

#include <functional>
#include "Vector3.h"
#include "Texture.h"
#include <memory>

#define EPSILON (1e-6)
#define M_PI (3.1415926535897932384626)

int search(const std::vector<double> &t,double o){
	int st=0,en=t.size()-1,mid=0;
	for(;st<en;){
		mid=(st+en)>>1;
		if(fabs(t[mid]-o)<EPSILON)
			return mid;
		if(t[mid]>o)
			en=mid;
		else
			st=mid+1;
	}
	return en;
}

// defines a functor type used to calculate the vector3 point according time;
typedef std::function<Vector3(const Vector3 &,double)> DynamicVector3Type;

DynamicVector3Type operator*(DynamicVector3Type func1,DynamicVector3Type func2){
	return [&func1,&func2](const Vector3 &orig,double time){
		func2(func1(orig,time),time);
	};
}


class ZeroAnimation:public DynamicVector3Type{
public:
	Vector3 operator()(const Vector3 &orig,double){
		return Vector3(orig);
	}
};

class TimeInterpolate:public DynamicVector3Type{
public:
	TimeInterpolate(const std::vector<Vector3> &_v,const std::vector<double> &_t):
		v(_v),t(_t){}
	Vector3 operator()(const Vector3 &_orig,double _time){
		if(_time<t[0])
			return Vector3(_orig);
		if(_time>t[t.size()])
			return v[v.size()];
		int i=search(t,_time);
		if(fabs(t[i]-_time)<EPSILON)
			return v[i];
		double c=(t[i]-_time)/(t[i]-t[i-1]);
		return v[i-1]*c+v[i]*(1-c);
	}
private:
	std::vector<Vector3> v;
	std::vector<double> t;
};

// the HitRecord struct is used to pass the data calculated in shape's intersection method
struct HitRecord{
	double t;
	Vector3 hitpoint;	// hitpoint=orig+t*direction
	Vector3 normal;		// the normal at the hitpoint
	Vector2 UVcoord;	// 2-D UV coordinate on a texture
	std::tr1::shared_ptr<Texture> texture;	// the hitten texture
};

#ifdef _WIN32
#define fscanf fscanf_s
#define fprintf fprintf_s
#endif // _WIN32
#endif // COMMON_H_