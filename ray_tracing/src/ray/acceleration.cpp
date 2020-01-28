//加速与优化
#include<cmath>
#include<ray.h>
#include<algorithm>

aabb::aabb()
{
	_min=vec_type(0,0,0);
	_max=vec_type(0,0,0);
}
aabb::aabb(const vec_type &a,const vec_type &b)
{
	_min=vec_type(std::min(a.x(),b.x())-esp,std::min(a.y(),b.y())-esp,std::min(a.z(),b.z())-esp);
	_max=vec_type(std::max(a.x(),b.x())+esp,std::max(a.y(),b.y())+esp,std::max(a.z(),b.z())+esp);
	vec_type len=_max-_min;
	_area=(len.x()*len.y()+len.y()*len.z()+len.x()*len.z())*2;
	// std::cout<<"max:"<<_min.x()<<" "<<_min.y()<<" "<<_min.z()<<std::endl;
	// std::cout<<"min:"<<_max.x()<<" "<<_max.y()<<" "<<_max.z()<<std::endl;
}
aabb::aabb(const aabb &a,const aabb &b)
{
	_min=vec_type(std::min(a.min().x(),b.min().x())-esp,std::min(a.min().y(),b.min().y())-esp,std::min(a.min().z(),b.min().z())-esp);
	_max=vec_type(std::max(a.max().x(),b.max().x())+esp,std::max(a.max().y(),b.max().y())+esp,std::max(a.max().z(),b.max().z())+esp);
	vec_type len=_max-_min;
	_area=(len.x()*len.y()+len.y()*len.z()+len.x()*len.z())*2;
	// std::cout<<"max:"<<_min.x()<<" "<<_min.y()<<" "<<_min.z()<<std::endl;
	// std::cout<<"min:"<<_max.x()<<" "<<_max.y()<<" "<<_max.z()<<std::endl;
}
bool aabb::hit(const ray &sight,value_type tmin,value_type tmax)const
{
	// std::cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<std::endl;
	// std::cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<std::endl;
	// std::cout<<_min.x()<<" "<<_min.y()<<" "<<_min.z()<<std::endl;
	// std::cout<<_max.x()<<" "<<_max.y()<<" "<<_max.z()<<std::endl;
	for(int i=0;i<3;i++)if(sight.direction()[i]!=0.)
	{
		value_type t1=(_min[i]-sight.origin()[i])/sight.direction()[i],t2=(_max[i]-sight.origin()[i])/sight.direction()[i];
		if(sight.direction()[i]<0)std::swap(t1,t2);
		if((tmax=std::min(t2,tmax))<(tmin=std::max(t1,tmin)))return 0;
	}
	else if(sight.origin()[i]>_max[i]||sight.origin()[i]<_min[i]||sight.origin()[i]>tmax||sight.origin()[i]<tmin)return 0;
	return 1;
}
const vec_type aabb::min()const{return _min;}
const vec_type aabb::max()const{return _max;}
const value_type aabb::area()const{return _area;}

bool cmpx(intersect *a,intersect *b){return a->get_box().min().x()<b->get_box().min().x();}
bool cmpy(intersect *a,intersect *b){return a->get_box().min().y()<b->get_box().min().y();}
bool cmpz(intersect *a,intersect *b){return a->get_box().min().z()<b->get_box().min().z();}
bvh_node::bvh_node(intersect** world,const int n)
{
	// for(int i=0;i<n;i++)std::cout<<world[i]<<" ";
	// std::cout<<std::endl;
	if(n==1)_left=world[0],_box=world[0]->get_box();
	else
	{
		vec_type _min(INF,INF,INF),_max(-INF,-INF,-INF);
		value_type area=0,area_=0,min_cost=INF;
		int div=0;
		for(int i=0;i<n;i++)
		{
			aabb tmp=world[i]->get_box();
			area+=tmp.area();
			_min=vec_type(std::min(tmp.min().x(),_min.x()),std::min(tmp.min().y(),_min.y()),std::min(tmp.min().z(),_min.z()));
			_max=vec_type(std::max(tmp.max().x(),_max.x()),std::max(tmp.max().y(),_max.y()),std::max(tmp.max().z(),_max.z()));
		}
		if((_max.x()-_min.x()+esp>_max.y()-_min.y())&&(_max.x()-_min.x()+esp>_max.z()-_min.z()))std::sort(world,world+n,cmpx);
		else if((_max.y()-_min.y()+esp>_max.x()-_min.x())&&(_max.y()-_min.y()+esp>_max.z()-_min.z()))std::sort(world,world+n,cmpy);
		else std::sort(world,world+n,cmpz);
		for(int i=0;i<n-1;i++)
		{
			area_+=world[i]->get_box().area();
			if((i+1)*area_+(n-i-1)*(area-area_)<min_cost)
			{
				min_cost=(i+1)*area_+(n-i-1)*(area-area_);
				div=i;
			}
		}//surface area heoristic
		_left=new bvh_node(world,div+1);
		_right=new bvh_node(world+div+1,n-div-1);
		_box=aabb(_left->get_box(),_right->get_box());
	}
}
bool bvh_node::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	if(_box.hit(sight,t_min,t_max))
	{
		hitInfo linfo,rinfo;
		bool lhit=0,rhit=0;
		if(_left)lhit=_left->hit(sight,t_min,t_max,linfo);
		if(_right)rhit=_right->hit(sight,t_min,t_max,rinfo);
		if(!lhit&&!rhit)return 0;
		if(lhit)rec=linfo;
		else if(rhit)rec=rinfo;
		if(rhit&&lhit&&rinfo._t<linfo._t)rec=rinfo;
		return 1;
	}
	else return 0;
}