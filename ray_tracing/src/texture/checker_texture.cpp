#include<texture/checker_texture.h>
using namespace std;
checker_texture::checker_texture(texture *t1,texture *t2,value_type omega_x,value_type phi_x,value_type omega_y,value_type phi_y,value_type omega_z,value_type phi_z):_texture1(t1),_texture2(t2),_omega_x(omega_x),_phi_x(phi_x),_omega_y(omega_y),_phi_y(phi_y),_omega_z(omega_z),_phi_z(phi_z){}
vec_type checker_texture::value(value_type u,value_type v,const vec_type &p)const
{
	value_type sines=sin(_omega_x*p.x()+_phi_x)*sin(_omega_y*p.y()+_phi_y)*sin(_omega_z*p.z()+_phi_z);
	return sines<0?_texture1->value(u,v,p):_texture2->value(u,v,p);
}