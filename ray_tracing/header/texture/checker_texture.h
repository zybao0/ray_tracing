#pragma once
#include"../ray.h"
using namespace gm;
class checker_texture:public texture//两个材质相间 
{
	public:
		checker_texture(texture *t1,texture *t2,value_type omega_x=1,value_type phi_x=0,value_type omega_y=1,value_type phi_y=0,value_type omega_z=1,value_type phi_z=0);//f(x)=sin(omega*x+phi)
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const override;
	private:
		texture *_texture1,*_texture2;
		value_type _omega_x,_phi_x,_omega_y,_phi_y,_omega_z,_phi_z;
};