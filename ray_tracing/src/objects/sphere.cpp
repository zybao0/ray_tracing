#include<objects/sphere.h>
#include<all_tools.h>
using namespace std;
sphere::sphere(const vec_type &h,value_type r,material *ma):_heart(h),_radius(r),_materialp(ma)
{
	_not_optimization=0;
	_box=aabb(_heart-vec_type(_radius,_radius,_radius),_heart+vec_type(_radius,_radius,_radius));
}
sphere::~sphere(){if(_materialp)delete _materialp;}
const value_type sphere::r()const{return _radius;}
const vec_type& sphere::heart()const{return _heart;}
bool sphere::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	// cout<<_heart.x()<<" "<<_heart.y()<<" "<<_heart.z()<<endl;
	// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;
	// cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<endl;
	vec_type trace=sight.origin()-_heart;
	value_type a=dot(sight.direction().ret_unitization(),sight.direction().ret_unitization()),b=2.*dot(trace,sight.direction().ret_unitization()),c=dot(trace,trace)-_radius*_radius,delt=b*b-4.*a*c;
	if(delt>0)
	{
		rec.materialp=_materialp;
		value_type x=(-b-sqrt(delt))/(2.*a);
		// cout<<_heart.x()<<" "<<_heart.y()<<" "<<_heart.z()<<endl;
		// cout<<"x:"<<x<<" "<<t_min<<" "<<t_max<<endl;
		if(x+eps<t_max&&x>t_min+eps)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-_heart)/_radius;
			get_sphere_uv((rec._p-_heart)/_radius,rec._u,rec._v,Vup);
			return 1;
		}
		x=(-b+sqrt(delt))/(2.*a);
		// cout<<"x:"<<x<<" "<<t_min<<" "<<t_max<<endl;
		if(x+eps<t_max&&x>t_min+eps)
		{
			rec._t=x;rec._p=sight.go(rec._t);rec._n=(rec._p-_heart)/_radius;
			get_sphere_uv((rec._p-_heart)/_radius,rec._u,rec._v,Vup);
			return 1;
		}
	}
	return 0;
}
