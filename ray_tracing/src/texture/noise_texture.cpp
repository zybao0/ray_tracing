#include<texture/noise_texture.h>
using namespace std;
noise_texture::noise_texture(const vec_type &color,const value_type scale):_color(color),_scale(scale){}
vec_type noise_texture::value(value_type u,value_type v,const vec_type &p)const{return _color*_noise.turb(_scale*p,trans_function2);}