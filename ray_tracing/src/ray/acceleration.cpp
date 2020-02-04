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
	// std::cout<<"min_max:"<<tmin<<" "<<tmax<<std::endl;
	// std::cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<std::endl;
	// std::cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<std::endl;
	// std::cout<<_min.x()<<" "<<_min.y()<<" "<<_min.z()<<std::endl;
	// std::cout<<_max.x()<<" "<<_max.y()<<" "<<_max.z()<<std::endl;
	for(int i=0;i<3;i++)if(sight.direction()[i]!=0.)
	{
		value_type t1=(_min[i]-sight.origin()[i])/sight.direction()[i],t2=(_max[i]-sight.origin()[i])/sight.direction()[i];
		if(sight.direction()[i]<0)std::swap(t1,t2);
		// std::cout<<"t:"<<t1<<" "<<t2<<std::endl;
		if((tmax=std::min(t2,tmax))<(tmin=std::max(t1,tmin)))return 0;
	}
	else if(sight.origin()[i]>_max[i]||sight.origin()[i]<_min[i]||sight.origin()[i]>tmax||sight.origin()[i]<tmin)return 0;
	// std::cout<<"hit"<<std::endl<<std::endl;
	return 1;
}
const vec_type aabb::min()const{return _min;}
const vec_type aabb::max()const{return _max;}
const value_type aabb::area()const{return _area;}

bool cmpx(intersect *a,intersect *b){return a->get_box().min().x()<b->get_box().min().x();}
bool cmpy(intersect *a,intersect *b){return a->get_box().min().y()<b->get_box().min().y();}
bool cmpz(intersect *a,intersect *b){return a->get_box().min().z()<b->get_box().min().z();}
int get_max_asix(intersect **world,const int n)//获得极差最大的那根轴
{
	vec_type _min(INF,INF,INF),_max(-INF,-INF,-INF);
	for(int i=0;i<n;i++)
	{
		aabb tmp=world[i]->get_box();
		_min=vec_type(std::min(tmp.min().x(),_min.x()),std::min(tmp.min().y(),_min.y()),std::min(tmp.min().z(),_min.z()));
		_max=vec_type(std::max(tmp.max().x(),_max.x()),std::max(tmp.max().y(),_max.y()),std::max(tmp.max().z(),_max.z()));
	}
	if((_max.x()-_min.x()+esp>_max.y()-_min.y())&&(_max.x()-_min.x()+esp>_max.z()-_min.z()))return AXIS_X;
	else if((_max.y()-_min.y()+esp>_max.x()-_min.x())&&(_max.y()-_min.y()+esp>_max.z()-_min.z()))return AXIS_Y;
	else return AXIS_Z;
}
vec3<int> sort_asix(aabb box)//对每根轴按极差从小到大排序
{
	int tmp[3]={0,1,2};
	vec3<int>f;
	std::sort(tmp,tmp+3,[=](int u,int v)->bool{return box.max()[u]-box.min()[u]>box.max()[v]-box.min()[v];});
	for(int i=0;i<3;i++)f[i]=tmp[i];
	return f;
}
bvh_node::bvh_node(intersect** world,const int n)
{
	// for(int i=0;i<n;i++)std::cout<<world[i]<<" ";
	// std::cout<<std::endl;
	if(n==1)_left=world[0],_box=world[0]->get_box(),_right=0;
	else
	{
		int axis=get_max_asix(world,n);
		if(axis==0)std::sort(world,world+n,cmpx);
		else if(axis==1)std::sort(world,world+n,cmpy);
		else std::sort(world,world+n,cmpz);

		int div=0;
		value_type area=0,area_=0,min_cost=INF;
		for(int i=0;i<n;i++)area+=world[i]->get_box().area();
		for(int i=0;i<n-1;i++)
		{
			area_+=world[i]->get_box().area();
			if((i+1)*area_+(n-i-1)*(area-area_)<min_cost)
			{
				min_cost=(i+1)*area_+(n-i-1)*(area-area_);
				div=i;
			}
		}//我所理解的sah(surface area heoristic)
		
		_left=new bvh_node(world,div+1);
		_right=new bvh_node(world+div+1,n-div-1);
		_box=aabb(_left->get_box(),_right->get_box());
	}
}
bool bvh_node::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	if(!_box.hit(sight,t_min,t_max))return 0; 
	hitInfo linfo,rinfo;
	bool lhit=0,rhit=0;
	lhit=_left->hit(sight,t_min,t_max,linfo);//似乎左边一定有儿子，不用判断
	if(lhit)t_max=linfo._t,rec=linfo;//如果左边有交点，则将最远距离设置为交点
	if(_right)rhit=_right->hit(sight,t_min,t_max,rinfo);
	if(!lhit&&!rhit)return 0;
	else if(rhit)rec=rinfo;
	if(rhit&&lhit&&rinfo._t<linfo._t)rec=rinfo;
	return 1;
}

KD_tree_node::KD_tree_node(intersect**world,const int n,const int depth,aabb bound)
{
	// std::cout<<bound.min().x()<<" "<<bound.min().y()<<" "<<bound.min().z()<<" "<<std::endl;
	// std::cout<<bound.max().x()<<" "<<bound.max().y()<<" "<<bound.max().z()<<" "<<std::endl;
	_box=bound;
	intersect **objects=new intersect*[n];//objects in the bound
	value_type **bound_list=new value_type*[6];
	int num=0;
	for(int i=0;i<n;i++)
	{
		bool flag=1;
		for(int j=0;flag&&j<3;j++)if(bound.min()[j]>world[i]->get_box().max()[j]+esp||bound.max()[j]<world[i]->get_box().min()[j]-esp)flag=0;
		if(!flag)continue;
		objects[num++]=world[i];
	}
	if(num==1)
	{
		split=0;
		_left=new KD_tree_leaf(objects,num,bound);
		_right=0;
	}
	else
	{
		for(int i=0;i<6;i++)bound_list[i]=new value_type[num];
		for(int i=0;i<3;i++)
		for(int j=0;j<num;j++)
		{
			bound_list[i*2][j]=std::max(objects[j]->get_box().min()[i],bound.min()[i]);//每个物体的左端点
			bound_list[i*2+1][j]=std::min(objects[j]->get_box().max()[i],bound.max()[i]);//每个物体的右端点
		}
		vec3<int>f=sort_asix(bound);
		value_type min_cost=INF;
		split=0;//
		for(int i=0;i<3;i++)//f[i]为当前的轴
		{
			std::sort(bound_list[f[i]],bound_list[f[i]]+num);
			std::sort(bound_list[f[i]+1],bound_list[f[i]+1]+num);
			int p=0,q=0,size_l=0,size_r=num;
			while(p<num)//类似于归并排序遍历 由于物体左端点最大值一定小于物体右端点 p一定先遍历完
			{
				bool flag=bound_list[f[i]*2][p]<bound_list[f[i]*2+1][q];
				if(flag&&p&&bound_list[f[i]*2][p]==bound_list[f[i]*2][p-1])
				{
					size_l++;
					p++;
					break;
				}
				if(!flag&&q&&bound_list[f[i]*2+1][q]==bound_list[f[i]*2+1][q-1])
				{
					size_r--;
					q++;
					break;
				}
				vec_type tmp_min=bound.min(),tmp_max=bound.max();
				tmp_min[f[i]]=tmp_max[f[i]]=flag?bound_list[f[i]*2][p]:bound_list[f[i]*2+1][q];
				aabb l_box(bound.min(),tmp_max),r_box(tmp_min,bound.max());
				//仅当两个待划分节点中的任意一个为空节点时增加一个奖励项 
				value_type now_cost=(p==0&&(bound_list[f[i]*2][p]>bound.min()[f[i]]+esp)?(bound.max()[f[i]]-bound_list[f[i]*2][p])/(bound.max()[f[i]]-bound.min()[f[i]]):1.)*(size_l*l_box.area()+size_r*r_box.area());
				if(now_cost<min_cost)
				{
					min_cost=now_cost;
					_split_pos=flag?bound_list[f[i]*2][p]:bound_list[f[i]*2+1][q];
					_split_axis=f[i];
				}
				flag?size_l++:size_r--;
				flag?p++:q++;
			}
			while(q<num)
			{
				vec_type tmp_min=bound.min(),tmp_max=bound.max();
				tmp_min[f[i]]=tmp_max[f[i]]=bound_list[f[i]*2+1][q];
				aabb l_box(bound.min(),tmp_max),r_box(tmp_min,bound.max());
				value_type now_cost=(q==num-1&&(bound_list[f[i]*2+1][1]<bound.min()[f[i]]-esp)?(bound_list[f[i]*2+1][q]-bound.min()[f[i]])/(bound.max()[f[i]]-bound.min()[f[i]]):1.)*(size_l*l_box.area()+size_r*r_box.area());
				if(now_cost<min_cost)
				{
					min_cost=now_cost;
					_split_pos=bound_list[f[i]*2+1][q];
					_split_axis=f[i];
				}
				size_r--;
				q++;
			}
			//if(_split_pos>bound_list[f[i]*2][0]+esp&&_split_pos<bound_list[f[i]*2+1][num-1]-esp)
			if(_split_pos>bound.min()[_split_axis]+esp&&_split_pos<bound.max()[_split_axis]-esp)
			{

				split=1;
				break;
			}
		}
		// std::cout<<bound.min().x()<<" "<<bound.min().y()<<" "<<bound.min().z()<<" "<<std::endl;
		// std::cout<<bound.max().x()<<" "<<bound.max().y()<<" "<<bound.max().z()<<" "<<std::endl;
		// std::cout<<"split: "<<depth<<" "<<split<<" "<<_split_axis<<" "<<_split_pos<<std::endl<<std::endl;

		if(!split)
		{
			_left=new KD_tree_leaf(objects,num,bound);
			_right=0;
		}
		else if(!depth)
		{
			int left_num=0,right_num=0;
			intersect **left_objects=new intersect*[num],**right_objects=new intersect*[num];
			for(int i=0;i<num;i++)
			{
				if(objects[i]->get_box().min()[_split_axis]<_split_pos)left_objects[left_num++]=objects[i];
				if(objects[i]->get_box().max()[_split_axis]>_split_pos)right_objects[right_num++]=objects[i];
			}
			vec_type tmp_min=bound.min(),tmp_max=bound.max();
			tmp_min[_split_axis]=tmp_max[_split_axis]=_split_pos;
			aabb l_box(bound.min(),tmp_max),r_box(tmp_min,bound.max());
			_left=new KD_tree_leaf(left_objects,left_num,l_box);
			_right=new KD_tree_leaf(right_objects,right_num,r_box);
			delete []left_objects;
			delete []right_objects;
		}
		else 
		{
			vec_type tmp_min=bound.min(),tmp_max=bound.max();
			tmp_min[_split_axis]=tmp_max[_split_axis]=_split_pos;
			aabb l_box(bound.min(),tmp_max),r_box(tmp_min,bound.max());
			_left=new KD_tree_node(world,n,depth-1,l_box);
			_right=new KD_tree_node(world,n,depth-1,r_box);
		}
		for(int i=0;i<6;i++)delete []bound_list[i];
		delete []bound_list;
	}
	delete []objects;
}
bool KD_tree_node::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	// std::cout<<"split:"<<_split_axis<<" "<<_split_pos<<" "<<split<<std::endl;
	// std::cout<<_box.min().x()<<" "<<_box.min().y()<<" "<<_box.min().z()<<" "<<std::endl;
	// std::cout<<_box.max().x()<<" "<<_box.max().y()<<" "<<_box.max().z()<<"\n"<<std::endl;
	if(!_box.hit(sight,t_min,t_max))return 0;
	if(!split)return _left->hit(sight,t_min,t_max,rec);
	if(sight.direction()[_split_axis]>=0&&sight.origin()[_split_axis]>_split_pos)return _right->hit(sight,t_min,t_max,rec);
	if(sight.direction()[_split_axis]<=0&&sight.origin()[_split_axis]<_split_pos)return _left->hit(sight,t_min,t_max,rec);
	value_type t_hit_split=(_split_pos-sight.origin()[_split_axis])/sight.direction().ret_unitization()[_split_axis];
	// std::cout<<"t_hit:"<<t_hit_split<<std::endl;
	if(sight.direction()[_split_axis]>0&&sight.origin()[_split_axis]<_split_pos)
	{
		if(_left->hit(sight,t_min,t_hit_split,rec))return 1;
		// std::cout<<"change1"<<std::endl;
		return _right->hit(sight,t_hit_split,t_max,rec);
	}
	if(sight.direction()[_split_axis]<0&&sight.origin()[_split_axis]>_split_pos)
	{
		if(_right->hit(sight,t_min,t_hit_split,rec))return 1;
		// std::cout<<"change2"<<std::endl;
		return _left->hit(sight,t_hit_split,t_max,rec);
	}
}

KD_tree_leaf::KD_tree_leaf(intersect **world,const int n,aabb bound):_size(n)
{
	
	_box=bound;
	_list=new intersect*[n];
	for(int i=0;i<n;i++)_list[i]=world[i];
	// for(int i=0;i<n;i++)std::cout<<world[i]<<" ";
	// std::cout<<std::endl;
	// std::cout<<_box.min().x()<<" "<<_box.min().y()<<" "<<_box.min().z()<<" "<<std::endl;
	// std::cout<<_box.max().x()<<" "<<_box.max().y()<<" "<<_box.max().z()<<"\n"<<std::endl;
}
bool KD_tree_leaf::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	// for(int i=0;i<_size;i++)std::cout<<_list[i]<<" ";
	// std::cout<<std::endl;
	if(!_box.hit(sight,t_min,t_max))return 0;
	hitInfo t_rec;
	bool hitSomething=0;
	value_type far=t_max;//刚开始可以看到无限远
	for(int i=0;i<_size;i++)if(_list[i]->hit(sight,t_min,far,t_rec))
	{
		hitSomething=1;
		far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
		rec=t_rec;
	}
	return hitSomething;
}