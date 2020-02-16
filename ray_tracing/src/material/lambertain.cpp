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
	if(emit)_emit=emit;
	else _emit=albedo;
	_intensity=intensity;
}
bool lambertain::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered)const
{
	bool fg=(dot(info._n,InRay.direction())>0);//一般来讲，光线不会再漫反射表面穿过物体，若光线与法向量同向，则翻转法向量
	vec_type target=info._p+(fg?-info._n:info._n)+random_unit_sphere();
	scattered=ray(info._p,target-info._p,InRay.time());
	attenuation=_albedo->value(info._u,info._v,info._p)*(1-_intensity);
	if(_emit)emit=_intensity*_emit->value(info._u,info._v,info._p);
	return 1;
}