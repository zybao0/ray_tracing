//材料类 实现了折射，反射与漫反射
#include<ray.h>
#include<my_random.h>

vec_type material::reflect(const vec_type &in,const vec_type &n)const{return in-2*dot(in,n)*n;}//反射
bool material::refract(const vec_type &InRay,const vec_type &n,value_type eta,vec_type &reflected)const//折射
{
	vec_type unitIn=InRay.ret_unitization();
	value_type cos1=dot(-unitIn,n),cos2=1.-eta*eta*(1-cos1*cos1);
	if(cos2>0)
	{
		reflected=eta*unitIn+n*(eta*cos1-sqrt(cos2));
		return 1;
	}
	return 0;//全反射
}
value_type material::schlick(const value_type cosine,const value_type RI)const//反射系数
{
	value_type r0=((1.-RI)/(1.+RI))*((1.-RI)/(1.+RI));
	return r0+(1-r0)*pow(1-cosine,5);
}

lambertain::lambertain(const vec_type &a):_albedo(a){}
bool lambertain::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,ray &scattered)const
{
	vec_type target=info._p+info._n+random_unit_sphere();
	scattered=ray(info._p,target-info._p,InRay.time());
	attenuation=_albedo;
	return 1;
}

metal::metal(const vec_type &a,const value_type f):_albedo(a){_fuzz=(f>=0&&f<1?f:1);}
bool metal::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,ray &scattered)const
{
	vec_type target=reflect(InRay.direction().ret_unitization(),info._n);
	scattered=ray(info._p,target+_fuzz*random_unit_sphere(),InRay.time());
	attenuation=_albedo;
	return 1;
	//return dot(scattered.direction(),info._n);
}

dielectric::dielectric(value_type RI,const vec_type &a):_RI(RI),_albedo(a){}

bool dielectric::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,ray &scattered)const
{
	vec_type outward_normal,refracted,reflected=reflect(InRay.direction(),info._n);
	value_type eta,cos,refract_prob=1.;
	attenuation=_albedo;
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