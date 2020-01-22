#include<objects/sphere.h>
using namespace std;
sphere::sphere(const vec_type &h,value_type r,material *ma):_heart(h),_radius(r),_materialp(ma){}
sphere::~sphere(){if(_materialp)delete _materialp;}
const value_type sphere::r()const{return _radius;}
const vec_type& sphere::heart()const{return _heart;}
bool sphere::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	vec_type trace=sight.origin()-_heart;
	value_type a=dot(sight.direction(),sight.direction()),b=2.*dot(trace,sight.direction()),c=dot(trace,trace)-_radius*_radius,delt=b*b-4.*a*c;
	if(delt>0)
	{
		rec.materialp=_materialp;
		value_type x=(-b-sqrt(delt))/(2.*a);
		if(x+esp<t_max&&x>t_min+esp)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-_heart)/_radius;
			return 1;
		}
		x=(-b+sqrt(delt))/(2.*a);
		if(x+esp<t_max&&x>t_min+esp)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-_heart)/_radius;
			return 1;
		}
	}
	return 0;
}
