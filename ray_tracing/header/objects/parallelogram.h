#pragma once
#include"../ray.h"
using namespace gm;
class parallelogram:public intersect
{
	public:
		parallelogram();
		parallelogram(const vec_type &p,const vec_type &edge1,const vec_type &edge2,material *ma);
		// ~parallelogram();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
		virtual vec_type random_poin()const override;
		virtual value_type size()const override;
		const vec_type& n()const;
		const void flit_n();
	protected:
		vec_type _p,_edge1,_edge2,_n;//平面上的一点(贴图的左下点) 沿着两条边的向量(贴图中沿宽的边，贴图中沿高的边) 法向量
		material* _materialp;//材质
};