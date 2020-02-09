#include<material/dielectric.h>
dielectric::dielectric(value_type RI,texture *albedo):_RI(RI)
{
	_albedo=albedo;
	_emit=0;
	_intensity=0;
}
dielectric::dielectric(value_type RI,texture *albedo,texture *emit,value_type intensity):_RI(RI)
{
	_albedo=albedo;
	_emit=emit;
	_intensity=intensity;
}
bool dielectric::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered)const
{
	vec_type outward_normal,refracted,reflected=reflect(InRay.direction(),info._n);
	value_type eta,cos,refract_prob=1.;
	attenuation=_albedo->value(info._u,info._v,info._p)*(1-_intensity);
	if(_emit)emit=_intensity*_emit->value(info._u,info._v,info._p);
	if(dot(InRay.direction(),info._n)>0)
	{
		outward_normal=-info._n;
		eta=_RI;
		cos=eta*dot(InRay.direction(),info._n)/InRay.direction().normal();
	}
	else
	{
		outward_normal=info._n;
		eta=1./_RI;
		cos=-eta*dot(InRay.direction(),info._n)/InRay.direction().normal();
	}
	if(refract(InRay.direction(),outward_normal,eta,refracted))refract_prob=schlick(cos,_RI);//折射，计算反射系数
	if(random_unit_figure()<=refract_prob)scattered=ray(info._p,reflected,InRay.time());
	else scattered=ray(info._p,refracted,InRay.time());
	return 1;
}