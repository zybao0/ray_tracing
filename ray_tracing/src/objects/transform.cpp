#include<objects/transform.h>
#include<all_tools.h>
using namespace std;
transform::transform(const vec_type &o,const mat_type &m,const vec_type &shift,intersect *item):_m(m.n(),m.m()),_lu(m.n(),m.m()),_p(m.n(),m.m())
{
	_not_optimization=0;
	_m=m;m.gauss(_p);
	_lu=(_p*m).LU();
	// (m*(mat_type(o).transpose())).echo();
	_shift=shift-(m*(mat_type(o).transpose())).transpose().to_vec()+o;
	// mat_type(_shift).echo();
	_item=item;
	vec_type tmp_max(-INF,-INF,-INF),tmp_min(INF,INF,INF);
	for(int i=0;i<=1;i++)
	for(int j=0;j<=1;j++)
	for(int k=0;k<=1;k++)
	{
		// cout<<"begin"<<endl;
		vec_type tmp((i?item->get_box().max().x():item->get_box().min().x()),(j?item->get_box().max().y():item->get_box().min().y()),(k?item->get_box().max().z():item->get_box().min().z()));
		// cout<<tmp.x()<<" "<<tmp.y()<<" "<<tmp.z()<<endl;
		// cout<<_shift.x()<<" "<<_shift.y()<<" "<<_shift.z()<<endl;
		// m.echo();
		// (m*(mat_type(tmp).transpose())).transpose().echo();
		tmp=(m*(mat_type(tmp).transpose())).transpose().to_vec()+_shift;
		// mat_type(tmp).echo();
		tmp_min=vec_type(min(tmp_min.x(),tmp.x()),min(tmp_min.y(),tmp.y()),min(tmp_min.z(),tmp.z()));
		tmp_max=vec_type(max(tmp_min.x(),tmp.x()),max(tmp_max.y(),tmp.y()),max(tmp_max.z(),tmp.z()));
	}
	// cout<<tmp_max.x()<<" "<<tmp_max.y()<<" "<<tmp_max.z()<<endl;
	// cout<<tmp_min.x()<<" "<<tmp_min.y()<<" "<<tmp_min.z()<<endl;
	_box=aabb(tmp_min,tmp_max);
}
bool transform::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	// cout<<"hit_trans"<<endl;
	// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;

	// cout<<"OK1"<<endl;
	ray sight_(solve(_lu,_p*(mat_type(sight.origin()-_shift).transpose())).transpose().to_vec(),solve(_lu,_p*(mat_type(sight.direction()).transpose())).transpose().to_vec());
	// cout<<"OK2"<<endl;
	if(_item->hit(sight_,t_min,t_max,rec))
	{
		rec._p=(_m*(mat_type(rec._p).transpose())).transpose().to_vec()+_shift;
		rec._n=(_m*(mat_type(rec._n).transpose())).transpose().to_vec();
		rec._t=(rec._p-sight.origin()).normal()/sight.direction().normal();
		if(rec._t<t_min+eps||rec._t>t_max-eps)return 0;
		return 1;
	}
	return 0;
}
