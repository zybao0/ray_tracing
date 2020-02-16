#pragma once
#include"../ray.h"
#include"parallelepiped.h"
using namespace gm;
class cuboid:public parallelepiped//public intersect
{
	public:
		cuboid();
		cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma);//x y z 轴
		cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma1,material *ma2,material *ma3);//(上下),(前后),(左右)
		cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma1,material *ma1_,material *ma2,material *ma2_,material *ma3,material *ma3_);
};