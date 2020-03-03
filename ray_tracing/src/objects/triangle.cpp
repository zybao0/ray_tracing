#include<objects/triangle.h>
#include<all_tools.h>
using namespace std;
triangle::triangle(){}
triangle::triangle(const vec_type &p,const vec_type &edge1,const vec_type &edge2,material *ma):_heart(p),_edge1(edge1),_edge2(edge2),_materialp(ma)
{
	_not_optimization=0;
	_n=cross(edge1,edge2).ret_unitization();
	_box=aabb(aabb(_heart,_heart+_edge1),aabb(_heart,_heart+_edge2));
}
// triangle::~triangle(){if(_materialp)delete _materialp;}
const vec_type& triangle::n()const{return _n;}
bool triangle::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	value_type d=get_triangle_uv(sight.origin(),sight.direction(),_heart,_edge1,_edge2,rec._t,rec._u,rec._v);
	if((d>-eps&&d<eps)||(rec._t<t_min+eps)||(rec._t>t_max-eps)||(rec._u<0)||(rec._v<0)||(rec._u+rec._v>1))return 0;
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
vec_type triangle::random_poin()const
{
	int u=random_unit_figure(),v=random_unit_figure();
	while(u+v>1)u=random_unit_figure(),v=random_unit_figure();
	return _heart+u*_edge1+v*_edge2;
}
value_type triangle::size()const{0.5*cross(_edge1,_edge2).normal();}