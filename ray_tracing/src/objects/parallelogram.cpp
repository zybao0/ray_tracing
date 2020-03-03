#include<objects/parallelogram.h>
#include<all_tools.h>
using namespace std;
parallelogram::parallelogram(){}
parallelogram::parallelogram(const vec_type &p,const vec_type &edge1,const vec_type &edge2,material *ma):_p(p),_edge1(edge1),_edge2(edge2),_materialp(ma)
{
	_not_optimization=0;
	_n=cross(edge1,edge2).ret_unitization();
	_box=aabb(aabb(_p,_p+_edge1),aabb(_p+_edge2,_p+_edge1+_edge2));
}
// parallelogram::~parallelogram(){if(_materialp)delete _materialp;}
const vec_type& parallelogram::n()const{return _n;}
bool parallelogram::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	value_type d=get_triangle_uv(sight.origin(),sight.direction(),_p,_edge1,_edge2,rec._t,rec._u,rec._v);
	if((d>-eps&&d<eps)||(rec._t<t_min+eps)||(rec._t>t_max-eps)||(rec._u<0)||(rec._v<0)||(rec._u>1)||(rec._v>1))return 0;
	rec.materialp=_materialp;
	rec._p=sight.go(rec._t);rec._n=_n;
	// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;
	// cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<endl;
	// cout<<_n.x()<<" "<<_n.y()<<" "<<_n.z()<<endl;
	// cout<<_p.x()<<" "<<_p.y()<<" "<<_p.z()<<endl;
	// cout<<rec._p.x()<<" "<<rec._p.y()<<" "<<rec._p.z()<<endl;
	// cout<<endl;
	return 1;
}
vec_type parallelogram::random_poin()const{return _p+random_unit_figure()*_edge1+random_unit_figure()*_edge2;}
value_type parallelogram::size()const{cross(_edge1,_edge2).normal();}
const void parallelogram::flit_n(){_n=-_n;}