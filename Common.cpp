#include "Common.h"
#include <functional>
#include "Vector3.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include "Material.h"

int search(const std::vector<double> &t,double o){
	int st=0,en=t.size()-1,mid=0;
	for(;st<en;){
		mid=(st+en)>>1;
		if(fabs(t[mid]-o)<DBL_EPSILON)
			return mid;
		if(t[mid]>o)
			en=mid;
		else
			st=mid+1;
	}
	return en;
}

DynamicVector3Type operator*(DynamicVector3Type func1,DynamicVector3Type func2){
	return [func1,func2](const Vector3 &orig,double time){
		Vector3 temp=func1(orig,time);
		return func2(temp,time);
	};
}

DynamicVector3Type zeroAnimation=[](const Vector3 &_orig,double _time){return Vector3(_orig);};

DynamicVector3Type timeInterpolate(const std::vector<Vector3> &v,const std::vector<double> &t){
	return [v,t](const Vector3 &_orig,double _time){
		if(_time<t[0])
			return Vector3(_orig);
		if(_time>t[t.size()])
			return Vector3(v[v.size()]);
		int i=search(t,_time);
		if(fabs(t[i]-_time)<DBL_EPSILON)
			return Vector3(v[i]);
		double c=(t[i]-_time)/(t[i]-t[i-1]);
		return v[i-1]*c+v[i]*(1-c);
	};
}