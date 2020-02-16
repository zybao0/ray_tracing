#include<objects/moving_sphere.h>
using namespace std;
moving_sphere::moving_sphere(const vec_type &h1,const vec_type &h2,value_type t1,value_type t2,value_type r,material *ma):_heart1(h1),_heart2(h2),_time1(t1),_time2(t2),_radius(r),_materialp(ma){}
// moving_sphere::~moving_sphere(){if(_materialp)delete _materialp;}
const value_type moving_sphere::r()const{return _radius;}
const vec_type moving_sphere::heart(const value_type t)const{return _heart1+((t-_time1)/(_time2-_time1))*(_heart2-_heart1);}
bool moving_sphere::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	vec_type trace=sight.origin()-heart(sight.time());
	value_type a=dot(sight.direction(),sight.direction()),b=2.*dot(trace,sight.direction()),c=dot(trace,trace)-_radius*_radius,delt=b*b-4.*a*c;
	if(delt>0)
	{
		rec.materialp=_materialp;
		value_type x=(-b-sqrt(delt))/(2.*a);
		if(x+eps<t_max&&x>t_min+eps)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-heart(sight.time()))/_radius;
			get_sphere_uv((rec._p-heart(sight.time()))/_radius,rec._u,rec._v,Vup);
			return 1;
		}
		x=(-b+sqrt(delt))/(2.*a);
		if(x+eps<t_max&&x>t_min+eps)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-heart(sight.time()))/_radius;
			get_sphere_uv((rec._p-heart(sight.time()))/_radius,rec._u,rec._v,Vup);
			return 1;
		}
	}
	return 0;
}
