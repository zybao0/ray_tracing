#pragma once
#include"../ray.h"
#include"my_random.h"
using namespace gm;
class noise_texture:public texture
{
	public:
		noise_texture(const vec_type &color,const value_type scale=1);
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const override;
	private:
		Perlin _noise;
		vec_type _color;
		value_type _scale;
};