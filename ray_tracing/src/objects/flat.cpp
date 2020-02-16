#include<objects/flat.h>
#include<all_tools.h>
using namespace std;
flat::flat(){}
flat::flat(const vec_type &p,const vec_type &n,material *ma):_heart(p),_materialp(ma)
{
	_not_optimization=1;
	_n=n.ret_unitization();
	// _box=aabb(vec_type(INF,INF,INF),vec_type(INF,INF,INF));
}
// flat::~flat(){if(_materialp)delete _materialp;}
const vec_type& flat::n()const{return _n;}
bool flat::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	// cout<<"hit_flat"<<endl;
	value_type d=dot(sight.direction(),_n);
	if(d>-eps&&d<eps)return 0;//光线的方向向量与平面的法向量垂直，无焦点
	value_type x=dot(_heart-sight.origin(),_n)/dot(sight.direction(),_n);
	if(x<t_min+eps||x>t_max-eps)return 0;
	rec.materialp=_materialp;
	rec._t=x;rec._p=sight.go(rec._t);rec._n=_n;
	rec._u=0;rec._v=0;
	// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;
	// cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<endl;
	// cout<<_n.x()<<" "<<_n.y()<<" "<<_n.z()<<endl;
	// cout<<_heart.x()<<" "<<_heart.y()<<" "<<_heart.z()<<endl;
	// cout<<rec._p.x()<<" "<<rec._p.y()<<" "<<rec._p.z()<<endl;
	// cout<<endl;
	return 1;
}