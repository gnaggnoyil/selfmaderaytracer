#ifndef _COMMON_H_
#define _COMMON_H_ 1

#include <functional>
#include "Vector3.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include "Material.h"

#define M_PI (3.1415926535897932384626)
#define EPSILON (1e-6)

int search(const std::vector<double> &t,double o);

inline double sqr(double x){
	return x*x;
}

template<class T>
inline void swap(T &x,T &y){
	T tmp=x;
	x=y;
	y=tmp;
}

// defines a functor type used to calculate the vector3 point according time;
typedef std::function<Vector3(const Vector3 &,double)> DynamicVector3Type;

DynamicVector3Type operator*(DynamicVector3Type func1,DynamicVector3Type func2);

extern DynamicVector3Type zeroAnimation;

DynamicVector3Type timeInterpolate(const std::vector<Vector3> &v,const std::vector<double> &t);

// the HitRecord struct is used to pass the data calculated in shape's intersection method
struct HitRecord{
	double t;
	Vector3 hitpoint;	// hitpoint=orig+t*direction
	Vector3 normal;		// the normal at the hitpoint
	Vector2 UVcoord;	// 2-D UV coordinate on a texture
	std::tr1::shared_ptr<Material> material;	// the hitten material
};
#endif // COMMON_H_