#pragma once
#include"myVec2.h"
#include"myVec3.h"
using namespace gm;
using value_type=precision;
using vec_type=vec3<value_type>;
void get_sphere_uv(const vec_type &p,value_type &u,value_type &v,const vec_type &Vup);
value_type get_triangle_uv(const vec_type &origin,const vec_type &direction,const vec_type &p,const vec_type &e1,const vec_type &e2,value_type &t,value_type &u,value_type &v);//当光线与e1,e2,p所在的平面相交时 求出p与交点成的向量用u*e1+v*e2表示，可用于求光线与三角形没有吗交点