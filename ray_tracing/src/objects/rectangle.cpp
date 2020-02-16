#include<objects/rectangle.h>
#include<all_tools.h>
using namespace std;
rectangle::rectangle(){}
rectangle::rectangle(const vec_type &p1,const vec_type &p2,const vec_type &dirict,material *ma)
{
	_not_optimization=0;
	_p=p1;_materialp=ma;
	value_type t=dot(p2-p1,dirict)/dirict.squar();
	_edge1=t*dirict;
	_edge2=p2-(p1+_edge1);
	_n=cross(_edge1,_edge2).ret_unitization();
	_box=aabb(aabb(p1,p1+_edge1),aabb(p2,p1+_edge2));
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
// rectangle::~rectangle(){if(_materialp)delete _materialp;}