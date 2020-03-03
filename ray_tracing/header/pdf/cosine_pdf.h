#pragma once
#include"../ray.h"
class cosine_pdf:public pdf 
{
	public:
		// cosine_pdf(const vec_type &t);//默认生成的方向是绕z的但实际上我们要生成绕法向，所以要旋转
		virtual value_type value(const vec_type &origin,const vec_type &direction)const override;
		virtual vec_type generate(const vec_type &origin)const override;
	// private:
	// 	mat_type trans;
};