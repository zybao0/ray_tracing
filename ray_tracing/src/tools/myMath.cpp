#include<cmath>
#include<algorithm>
#include<tools/myMath.h>
using namespace std;
const value_type pi=acos(-1);
const value_type eps=1e-9;
void get_sphere_uv(const vec_type &p,value_type &u,value_type &v,const vec_type &Vup)
{
	value_type phi,the;
	if(Vup.x()>0)phi=atan2(p.y(),p.z()),the=asin(p.x());
	else if(Vup.y()>0)phi=atan2(p.z(),p.x()),the=asin(p.y());
	else if(Vup.z()>0)phi=atan2(p.x(),p.y()),the=asin(p.z());
	u=1-(phi+pi)/(2*pi);v=(the+pi/2)/pi;
}
value_type get_triangle_uv(const vec_type &origin,const vec_type &direction,const vec_type &p,const vec_type &e1,const vec_type &e2,value_type &t,value_type &u,value_type &v)
{
	vec_type pvec=cross(direction,e2),tvec=origin-p,qvec=cross(tvec,e1);
	value_type d=dot(e1,pvec),f=1./d;
	if(d>-eps&&d<eps)return 0;//平行
	t=f*dot(e2,qvec);
	u=f*dot(tvec,pvec);
	v=f*dot(qvec,direction);
	return d;
}