#ifndef _COMMON_H_
#define _COMMON_H_ 1

#include <functional>
#include "Vector3.h"
#include "Texture.h"
#include <memory>

// defines a functor type used to calculate the vector3 point according time;
typedef std::function<Vector3(const Vector3 &,double)> DynamicVector3Type;

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