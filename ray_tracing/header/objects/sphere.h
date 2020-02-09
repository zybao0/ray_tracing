#pragma once
#include"../ray.h"
#include"../all_tools.h"
using namespace gm;
class sphere:public intersect
{
	public:
		sphere(){};
		sphere(const vec_type &h,value_type r,material *ma);
		~sphere();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
		const value_type r()const;
		const vec_type& heart()const;
	private:
		vec_type _heart;
		value_type _radius;
		material* _materialp;//材质
};