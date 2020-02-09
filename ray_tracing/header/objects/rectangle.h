#pragma once
#include"../ray.h"
using namespace gm;
class rectangle:public intersect
{
	public:
		rectangle();
		rectangle(const vec_type &p1,const vec_type &p2,const vec_type &dirict,material *ma);//矩形上一点与其对角线上令一点 与p1相邻的一条边的方向向量
		~rectangle();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
		const vec_type& n()const;
	private:
		vec_type _p,_n,_edge1,_edge2;//_n:法向量
		material* _materialp;//材质
};