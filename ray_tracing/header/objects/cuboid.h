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
	// 	virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
	// private:
	// 	parallelogram _list[6];
	// 	//list[0]下面(edge1与edge2围成的面)
	// 	//list[1]上面
	// 	//list[2]后面(edge2与edge3围城的面)
	// 	//list[3]前面
	// 	//list[2]左面(edge3与edge1围城的面)
	// 	//list[2]右面
};