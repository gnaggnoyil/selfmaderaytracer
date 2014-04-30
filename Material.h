#ifndef _MATERIAL_H_
#define _MATERIAL_H_ 1

#include "Texture.h"
#include <memory>
#include "rgb.h"
#include "Vector2.h"
#include "Vector3.h"

/**
 * this is the material class defining what a shape would behave on colors and lights
 */
class Material{
public:
	// constructors
	Material(const std::tr1::shared_ptr<Texture> &_tex,const rgb &_color,double _diff,double _spec,double _shine,double _transmit,double _refractionIndex);
	Material(const std::tr1::shared_ptr<Texture> &_tex,const rgb &_color,const rgb &_diff,const rgb &_spec,double _shine,double _transmit,double _refractionIndex);
	Material(const Material &_orig);

	void setTex(const std::tr1::shared_ptr<Texture> &_tex);
	std::tr1::shared_ptr<Texture> getTex()const;
	void setColor(const rgb &_color);
	rgb getColor()const;
	void setSpec(const rgb &_spec);
	void setSpec(double _spec);
	void setSpec(int i,double _spec);
	rgb getSpec()const;
	double getSpec(int i)const;
	void setDiff(const rgb &_diff);
	void setDiff(double _diff);
	void setDiff(int i,double _diff);
	rgb getDiff()const;
	double getDiff(int i)const;
	void setShine(double _shine);
	double getShine()const;
	void setTransmit(double _transmit);
	double getTransmit()const;
	void setRefractionIndex(double _refractionIndex);
	double getRefractionIndex()const;

	// returns the color intersected by a ray
	rgb calcColor(const Vector2 &uv,const Vector3 &p);
private:
	std::tr1::shared_ptr<Texture> tex;
	rgb color;
	rgb spec,diff;
	double shine;	// Phong cosine power for highlights
	double transmit;	// shapes with a meterial whose trasmit is nonzero is supposed to have both sides producing ray
	double refractionIndex;
};
#endif // _MATERIAL_H_