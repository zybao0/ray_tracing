#pragma once
#include"../ray.h"
using namespace gm;
class moving_sphere:public intersect
{
	public:
		moving_sphere(){};
		moving_sphere(const vec_type &h1,const vec_type &h2,value_type t1,value_type t2,value_type r,material *ma);
		// ~moving_sphere();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
		virtual value_type size()const override;
		const value_type r()const;
		const vec_type heart(const value_type t)const;
	private:
		vec_type _heart1,_heart2;
		value_type _radius,_time1,_time2;
		material* _materialp;//材质
};