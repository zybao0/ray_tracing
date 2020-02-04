#pragma once
#include"../ray.h"
using namespace gm;
class constant_texture:public texture
{
	public:
		constant_texture(const vec_type &color);
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const override;
	private:
		vec_type _color;
};