#include "Material.h"
#include <memory>
#include "Texture.h"
#include "rgb.h"
#include "Vector2.h"
#include "Vector3.h"

Material::Material(const std::tr1::shared_ptr<Texture> &_tex,const rgb &_color,double _diff,double _spec,double _shine,double _transmit,double _refractionIndex)
	:tex(_tex),color(_color),diff(_diff,_diff,_diff),spec(_spec,_spec,_spec),shine(_shine),transmit(_transmit),refractionIndex(_refractionIndex){}

Material::Material(const std::tr1::shared_ptr<Texture> &_tex,const rgb &_color,const rgb &_diff,const rgb &_spec,double _shine,double _transmit,
	double _refractionIndex)
	:tex(_tex),color(_color),diff(_diff),spec(_spec),shine(_shine),transmit(_transmit),refractionIndex(_refractionIndex){}

Material::Material(const Material &_orig)
	:tex(_orig.getTex()),color(_orig.getColor()),diff(_orig.getDiff()),spec(_orig.getSpec()),shine(_orig.getShine()),transmit(_orig.getTransmit()),
	refractionIndex(_orig.getRefractionIndex()){}

void Material::setTex(const std::tr1::shared_ptr<Texture> &_tex){
	tex=_tex;
}

std::tr1::shared_ptr<Texture> Material::getTex()const{
	return tex;
}

void Material::setColor(const rgb &_color){
	color=_color;
}

rgb Material::getColor()const{
	return color;
}

void Material::setSpec(const rgb &_spec){
	spec=_spec;
}

void Material::setSpec(double _spec){
	spec=rgb(_spec,_spec,_spec);
}

void Material::setSpec(int i,double _spec){
	spec[i]=_spec;
}

rgb Material::getSpec()const{
	return spec;
}

double Material::getSpec(int i)const{
	return spec[i];
}

void Material::setDiff(const rgb &_diff){
	diff=_diff;
}

void Material::setDiff(double _diff){
	diff=rgb(_diff,_diff,_diff);
}

void Material::setDiff(int i,double _diff){
	diff[i]=_diff;
}

rgb Material::getDiff()const{
	return diff;
}

double Material::getDiff(int i)const{
	return diff[i];
}

void Material::setShine(double _shine){
	shine=_shine;
}

double Material::getShine()const{
	return shine;
}

void Material::setTransmit(double _transmit){
	transmit=_transmit;
}

double Material::getTransmit()const{
	return transmit;
}

void Material::setRefractionIndex(double _refractionIndex){
	refractionIndex=_refractionIndex;
}

double Material::getRefractionIndex()const{
	return refractionIndex;
}

rgb Material::calcColor(const Vector2 &uv,const Vector3 &p){
	if(tex!=nullptr)
		return tex->value(uv,p);
	else
		return color;
}