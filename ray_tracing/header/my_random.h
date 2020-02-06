#pragma once
#include"myVec2.h"
#include"myVec3.h"
using namespace gm;
using value_type=precision;
using vec_type=vec3<value_type>;
const value_type random_unit_figure();
const vec_type random_unit_dick();
const vec_type random_unit_sphere();

value_type return_self(value_type u);
value_type trans_function2(value_type u);
value_type trans_function3(value_type u);//一个定义与值域都在[0,1]的单调函数，用来改变噪声的分布

class Perlin
{
	using trans_func=value_type(*)(value_type u);//函数指针
	public:
		Perlin();
		const value_type noise(const vec_type &p,trans_func=return_self)const;
		const value_type turb(vec_type p,trans_func f=return_self,int depth=7)const;
		const value_type interp(vec_type list[2][2][2],value_type u,value_type v,value_type w)const;
		const vec_type* randomvalue()const;
		const int* perm_x()const;
		const int* perm_y()const;
		const int* perm_z()const;
	private:
		vec_type *_randomvalue;
		int *_perm_x,*_perm_y,*_perm_z;
};