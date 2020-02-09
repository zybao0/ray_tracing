#include<material/metal.h>
metal::metal(texture *albedo,const value_type f)
{
	_albedo=albedo;
	_fuzz=(f>=0&&f<1?f:1);
	_emit=0;
	_intensity=0;
}
metal::metal(texture *albedo,texture *emit,value_type intensity,const value_type f)
{
	_albedo=albedo;
	_fuzz=(f>=0&&f<1?f:1);
	_emit=emit;
	_intensity=intensity;
}
bool metal::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered)const
{
	vec_type target=reflect(InRay.direction().ret_unitization(),info._n);
	scattered=ray(info._p,target+_fuzz*random_unit_sphere(),InRay.time());
	attenuation=_albedo->value(info._u,info._v,info._p)*(1-_intensity);
	if(_emit)emit=_intensity*_emit->value(info._u,info._v,info._p);
	return 1;
	//return dot(scattered.direction(),info._n);
}