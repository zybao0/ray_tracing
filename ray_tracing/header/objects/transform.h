#pragma once
#include"../ray.h"
#include"../all_tools.h"
using namespace gm;
class transform:public intersect
{
	public:
		transform(){};
		transform(const vec_type &o,const mat_type &m,const vec_type &shift,intersect *item);//o:变换(旋转)中心 m:变换矩阵 shift:旋转后再平移
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
	private:
		mat_type _m,_lu,_p;
		vec_type _shift;
		intersect *_item;
};