#include<texture/picture_texture.h>
using namespace std;
picture::picture(vec3<int> *pic,int x,int y):_width(x),_height(y),_picture(pic){}
vec_type picture::value(value_type u,value_type v,const vec_type &p)const
{
	// cout<<u<<" "<<v<<endl;
	int x=(_width-1)*u,y=(_height-1)*v;
	// cout<<x<<" "<<y<<" "<<y*_width+x<<endl;
	// cout<<_picture[y*_width+x].r()<<" "<<_picture[y*_width+x].g()<<" "<<_picture[y*_width+x].b()<<endl;
	return vec_type((value_type)_picture[y*_width+x].r()/255.,(value_type)_picture[y*_width+x].g()/255.,(value_type)_picture[y*_width+x].b()/255.);
}