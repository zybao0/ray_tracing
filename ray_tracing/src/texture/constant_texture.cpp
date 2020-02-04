#include<texture/constant_texture.h>
using namespace std;
constant_texture::constant_texture(const vec_type &color):_color(color){}
vec_type constant_texture::value(value_type u,value_type v,const vec_type &p)const{return _color;}