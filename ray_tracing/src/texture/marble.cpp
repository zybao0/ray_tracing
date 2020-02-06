#include<my_random.h>
#include<texture/marble.h>
using namespace std;
marble::marble(const vec_type &color,const vec_type &omega,const value_type scale,const value_type scale_):_color(color),_scale(scale),_scale_(scale_),_omega(omega){}
vec_type marble::value(value_type u,value_type v,const vec_type &p)const{return 0.5*_color*(sin(dot(_omega,p)+_scale_*_noise.turb(_scale*p,trans_function2))+1);}