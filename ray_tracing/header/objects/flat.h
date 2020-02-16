#pragma once
#include"../ray.h"
using namespace gm;
class flat:public intersect
{
	public:
		flat();
		flat(const vec_type &p,const vec_type &n,material *ma);
		// ~flat();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
		const vec_type& n()const;
	private:
		vec_type _heart,_n;//平面上的一点 法向量
		material* _materialp;//材质
};