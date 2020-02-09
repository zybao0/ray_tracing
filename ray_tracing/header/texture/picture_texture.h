#pragma once
#include"../ray.h"
using namespace gm;
class picture:public texture
{
	public:
		picture(vec3<int> *pic,int x,int y);
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const override;
	private:
		int _width,_height;
		vec3<int> *_picture;
};