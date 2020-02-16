#include<objects/parallelogram.h>
#include<objects/cuboid.h>
#include<all_tools.h>
using namespace std;
cuboid::cuboid(){}
cuboid::cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma)
{
	// cout<<"1"<<endl;
	value_type t=dot(p2-p1,dirict)/dirict.squar();
	vec_type edge1=t*dirict,edge2=p2-(p1+edge1),_n=cross(edge1,edge2).ret_unitization(),edge3=_n*height;

	_list[0]=parallelogram(p1,edge1,edge2,ma);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p1+edge3,edge1,edge2,ma);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p1,edge2,edge3,ma);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p1+edge1,edge2,edge3,ma);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p1,edge3,edge1,ma);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p1+edge2,edge3,edge1,ma);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();

	_box=(aabb(aabb(p1,p1+edge1+edge2+edge3),aabb(p1+edge3,p1+edge1+edge2)),aabb(aabb(p1+edge2,p1+edge1+edge3),aabb(p1+edge1,p1+edge2+edge3)));
	// cout<<_box.min().x()<<" "<<_box.min().y()<<" "<<_box.min().z()<<endl;
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
cuboid::cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma1,material *ma2,material *ma3)
{
	// cout<<"2"<<endl;
	value_type t=dot(p2-p1,dirict)/dirict.squar();
	vec_type edge1=t*dirict,edge2=p2-(p1+edge1),_n=cross(edge1,edge2).ret_unitization(),edge3=_n*height;

	// cout<<edge1.x()<<" "<<edge1.y()<<" "<<edge1.z()<<endl;
	// cout<<edge2.x()<<" "<<edge2.y()<<" "<<edge2.z()<<endl;

	_list[0]=parallelogram(p1,edge1,edge2,ma1);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p1+edge3,edge1,edge2,ma1);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p1,edge2,edge3,ma2);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p1+edge1,edge2,edge3,ma2);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p1,edge3,edge1,ma3);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p1+edge2,edge3,edge1,ma3);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();

	_box=(aabb(aabb(p1,p1+edge1+edge2+edge3),aabb(p1+edge3,p1+edge1+edge2)),aabb(aabb(p1+edge2,p1+edge1+edge3),aabb(p1+edge1,p1+edge2+edge3)));
	// cout<<_box.min().x()<<" "<<_box.min().y()<<" "<<_box.min().z()<<endl;
	// cout<<_box.max().x()<<" "<<_box.max().y()<<" "<<_box.max().z()<<endl;
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;
}
cuboid::cuboid(const vec_type &p1,const vec_type &p2,const vec_type &dirict,const value_type height,material *ma1,material *ma1_,material *ma2,material *ma2_,material *ma3,material *ma3_)
{
	// cout<<"3"<<endl;
	value_type t=dot(p2-p1,dirict)/dirict.squar();
	vec_type edge1=t*dirict,edge2=p2-(p1+edge1),_n=cross(edge1,edge2).ret_unitization(),edge3=_n*height;

	_list[0]=parallelogram(p1,edge1,edge2,ma1);
	if(dot(_list[0].n(),edge3)>0)_list[0].flit_n();

	_list[1]=parallelogram(p1+edge3,edge1,edge2,ma1_);
	if(dot(_list[1].n(),edge3)<0)_list[1].flit_n();

	_list[2]=parallelogram(p1,edge2,edge3,ma2);
	if(dot(_list[2].n(),edge1)>0)_list[2].flit_n();

	_list[3]=parallelogram(p1+edge1,edge2,edge3,ma2_);
	if(dot(_list[3].n(),edge1)<0)_list[3].flit_n();

	_list[4]=parallelogram(p1,edge3,edge1,ma3);
	if(dot(_list[4].n(),edge2)>0)_list[4].flit_n();

	_list[5]=parallelogram(p1+edge2,edge3,edge1,ma3_);
	if(dot(_list[5].n(),edge2)<0)_list[5].flit_n();
	// cout<<_edge1.x()<<" "<<_edge1.y()<<" "<<_edge1.z()<<endl;
	// cout<<_edge2.x()<<" "<<_edge2.y()<<" "<<_edge2.z()<<endl;

	_box=(aabb(aabb(p1,p1+edge1+edge2+edge3),aabb(p1+edge3,p1+edge1+edge2)),aabb(aabb(p1+edge2,p1+edge1+edge3),aabb(p1+edge1,p1+edge2+edge3)));
}