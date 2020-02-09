#include<material/lambertain.h>
lambertain::lambertain(texture *albedo)
{
	_albedo=albedo;
	_emit=0;
	_intensity=0;
}
lambertain::lambertain(texture *albedo,texture *emit,value_type intensity)
{
	_albedo=albedo;
	_emit=emit;
	_intensity=intensity;
}
bool lambertain::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered)const
{
	vec_type target=info._p+info._n+random_unit_sphere();
	scattered=ray(info._p,target-info._p,InRay.time());
	attenuation=_albedo->value(info._u,info._v,info._p)*(1-_intensity);
	if(_emit)emit=_intensity*_emit->value(info._u,info._v,info._p);
	return 1;
}