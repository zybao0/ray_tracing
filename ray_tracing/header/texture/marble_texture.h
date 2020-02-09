#pragma once
#include"../ray.h"
using namespace gm;
class marble:public texture
{
	public:
		marble(const vec_type &color,const vec_type &omega=vec_type(0,0,5),const value_type scale=1,const value_type scale_=10);
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const override;
	private:
		Perlin _noise;
		vec_type _color,_omega;
		value_type _scale,_scale_;
};