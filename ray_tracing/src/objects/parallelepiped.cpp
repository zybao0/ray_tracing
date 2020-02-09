#include<objects/parallelepiped.h>
#include<all_tools.h>
using namespace std;
parallelepiped::parallelepiped(){}
parallelepiped::parallelepiped(const vec_type &p,const vec_type &edge1,const vec_type &edge2,const vec_type &edge3,material *ma)
{
	_list[0]=parallelogram(p,edge1,edge2,ma);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p+edge3,edge1,edge2,ma);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p,edge2,edge3,ma);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p+edge1,edge2,edge3,ma);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p,edge3,edge1,ma);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p+edge2,edge3,edge1,ma);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();

	_box=(aabb(aabb(p,p+edge1+edge2+edge3),aabb(p+edge3,p+edge1+edge2)),aabb(aabb(p+edge2,p+edge1+edge3),aabb(p+edge1,p+edge2+edge3)));
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
parallelepiped::parallelepiped(const vec_type &p,const vec_type &edge1,const vec_type &edge2,const vec_type &edge3,material *ma1,material *ma2,material *ma3)
{
	_list[0]=parallelogram(p,edge1,edge2,ma1);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p+edge3,edge1,edge2,ma1);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p,edge2,edge3,ma2);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p+edge1,edge2,edge3,ma2);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p,edge3,edge1,ma3);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p+edge2,edge3,edge1,ma3);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();

	_box=(aabb(aabb(p,p+edge1+edge2+edge3),aabb(p+edge3,p+edge1+edge2)),aabb(aabb(p+edge2,p+edge1+edge3),aabb(p+edge1,p+edge2+edge3)));
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
parallelepiped::parallelepiped(const vec_type &p,const vec_type &edge1,const vec_type &edge2,const vec_type &edge3,material *ma1,material *ma1_,material *ma2,material *ma2_,material *ma3,material *ma3_)
{
	_list[0]=parallelogram(p,edge1,edge2,ma1);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p+edge3,edge1,edge2,ma1_);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p,edge2,edge3,ma2);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p+edge1,edge2,edge3,ma2_);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p,edge3,edge1,ma3);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p+edge2,edge3,edge1,ma3_);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();

	_box=(aabb(aabb(p,p+edge1+edge2+edge3),aabb(p+edge3,p+edge1+edge2)),aabb(aabb(p+edge2,p+edge1+edge3),aabb(p+edge1,p+edge2+edge3)));
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
bool parallelepiped::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	hitInfo t_rec;
	bool hitSomething=0;
	value_type far=t_max;//刚开始可以看到无限远
	for(int i=0;i<6;i++)if(_list[i].hit(sight,t_min,far,t_rec))
	{
		hitSomething=1;
		far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
		rec=t_rec;
	}
	return hitSomething;
}