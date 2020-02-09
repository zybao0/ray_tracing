#include<objects/rectangle.h>
#include<all_tools.h>
using namespace std;
rectangle::rectangle(){}
rectangle::rectangle(const vec_type &p1,const vec_type &p2,const vec_type &dirict,material *ma):_p(p1),_materialp(ma)
{
	_not_optimization=0;
	value_type t=dot(p2-p1,dirict)/dirict.squar();
	_edge1=t*dirict;
	_edge2=p2-(p1+_edge1);
	_n=cross(_edge1,_edge2).ret_unitization();
	_box=aabb(aabb(p1,p1+_edge1),aabb(p2,p1+_edge2));
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
rectangle::~rectangle(){if(_materialp)delete _materialp;}
const vec_type& rectangle::n()const{return _n;}
bool rectangle::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	value_type d=get_triangle_uv(sight.origin(),sight.direction(),_p,_edge1,_edge2,rec._t,rec._u,rec._v);
	// cout<<d<<endl;
	if((d>-eps&&d<eps)||(rec._t<t_min+eps)||(rec._t>t_max-eps)||(rec._u<0)||(rec._v<0)||(rec._u>1)||(rec._v>1))return 0;
	// cout<<rec._u<<" "<<rec._v<<endl;
	// if((d>-eps&&d<eps)||(rec._t<t_min+eps)||(rec._t>t_max-eps)||(rec._u<0)||(rec._v<0)||(rec._u+rec._v>1))return 0;
	rec.materialp=_materialp;
	rec._p=sight.go(rec._t);rec._n=_n;
	// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;
	// cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<endl;
	// cout<<_n.x()<<" "<<_n.y()<<" "<<_n.z()<<endl;
	// cout<<_heart.x()<<" "<<_heart.y()<<" "<<_heart.z()<<endl;
	// cout<<rec._p.x()<<" "<<rec._p.y()<<" "<<rec._p.z()<<endl;
	// cout<<endl;
	return 1;
}