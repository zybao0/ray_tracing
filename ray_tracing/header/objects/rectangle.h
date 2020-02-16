#pragma once
#include"../ray.h"
#include"parallelogram.h"
using namespace gm;
class rectangle:public parallelogram
{
	public:
		rectangle();
		rectangle(const vec_type &p1,const vec_type &p2,const vec_type &dirict,material *ma);//矩形上一点与其对角线上令一点 与p1相邻的一条边的方向向量
		// ~rectangle();
};