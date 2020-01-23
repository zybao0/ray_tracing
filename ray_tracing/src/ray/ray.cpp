#include<ray.h>
#include<my_random.h>
using namespace std;

ray::ray():_a{vec_type()},_b{vec_type()}{}
ray::ray(const vec_type& a,const vec_type& b,const value_type c):_a(a),_b(b),_time(c){}
ray::ray(const ray& r):_a(r._a),_b(r._b){}
vec_type ray::origin()const{return _a;}
vec_type ray::direction()const{return _b;}
value_type ray::time()const{return _time;}
vec_type ray::go(const value_type t)const{return _a+t*_b;}

vec_type material::reflect(const vec_type &in,const vec_type &n)const{return in-2*dot(in,n)*n;}//反射
bool material::refract(const vec_type &InRay,const vec_type &n,value_type eta,vec_type &reflected)const//折射
{
	vec_type unitIn=InRay.ret_unitization();
	value_type cos1=dot(-unitIn,n),cos2=1.-eta*eta*(1-cos1*cos1);
	if(cos2>esp)
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
	if(dot(InRay.direction(),info._n)>esp)
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

// constexpr static value_type intersect::inf(){return 0x3f3f3f3f;}
intersect::~intersect(){}

intersections::intersections(intersect** list,size_t n):_list(list),_size(n){}
bool intersections::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	hitInfo t_rec;
	bool hitSomething=0;
	value_type far=t_max;//刚开始可以看到无限远
	for(int i=0;i<_size;i++)if(_list[i]->hit(sight,t_min,far,t_rec))
	{
		hitSomething=1;
		far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
		rec=t_rec;
	}
	return hitSomething;
}

camera::camera(const vec_type &eye,const vec_type &start,const vec_type &horizon,const vec_type &vertical):_eye(eye),_start(start),_horizon(horizon),_vertical(vertical){};
camera::camera(const value_type aspect,const value_type vfov,const vec_type &lookfrom,const vec_type &lookat,const value_type focus,const value_type aperture,const value_type t1,const value_type t2,const vec_type &vup):_eye(lookfrom),_len_radius(aperture/2),_time1(t1),_time2(t2)
{
	value_type theta=vfov*pi/180,half_height=tan(theta/2)*focus,half_width=aspect*half_height;
	_w=(lookfrom-lookat).ret_unitization();
	_u=cross(vup,_w).ret_unitization();
	_v=cross(_w,_u);
	_start=_eye-half_width*_u-half_height*_v-focus*_w;
	_horizon=2.*half_width*_u;
	_vertical=2*half_height*_v;
}
const ray camera::get_ray(const value_type u,const value_type v)const
{
	vec_type rd=_len_radius*random_unit_dick();
	value_type time=((_time2-_time1)<esp)?0:_time1+random_unit_figure()*(_time2-_time1);
	return ray(_eye,_start+u*_horizon+v*_vertical-(_eye+_u*rd.x()+_v*rd.y()),time);
}
const ray camera::get_ray(const vec2<value_type> &para)const{return get_ray(para.u(),para.v());}
const vec_type& camera::eye()const{return _eye;}
const vec_type& camera::start()const{return _start;}
const vec_type& camera::horizon()const{return _horizon;}
const vec_type& camera::vertical()const{return _vertical;}
const vec_type& camera::u()const{return _u;}
const vec_type& camera::v()const{return _v;}
const vec_type& camera::w()const{return _w;}
const value_type& camera::lens_r()const{return _len_radius;}