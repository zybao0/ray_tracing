//基本类 包含光线，照相机与物体的基类
#include<ray.h>
#include<algorithm>

ray::ray():_a{vec_type()},_b{vec_type().ret_unitization()}{}
ray::ray(const vec_type& a,const vec_type& b,const value_type c):_a(a),_b(b.ret_unitization()),_time(c){}
ray::ray(const ray& r):_a(r._a),_b(r._b){}
vec_type ray::origin()const{return _a;}
vec_type ray::direction()const{return _b;}
value_type ray::time()const{return _time;}
vec_type ray::go(const value_type t)const{return _a+t*_b;}

vec_type material::reflect(const vec_type &in,const vec_type &n)const{return in-2*dot(in,n)*n;}//反射
bool material::refract(const vec_type &InRay,const vec_type &n,value_type eta,vec_type &reflected)const//折射
{
	vec_type unitIn=InRay.ret_unitization();
	value_type cos1=dot(-unitIn,n),cos2=1.-eta*eta*(1-cos1*cos1);
	if(cos2>0)
	{
		reflected=eta*unitIn+n*(eta*cos1-sqrt(cos2));
		return 1;
	}
	return 0;//全反射
}
value_type material::schlick(const value_type cosine,const value_type RI)const//反射系数
{
	value_type r0=((1.-RI)/(1.+RI))*((1.-RI)/(1.+RI));
	return r0+(1-r0)*pow(1-cosine,5);
}

intersect::intersect(){}
const aabb intersect::get_box()const{return _box;}
const bool intersect::not_optimization()const{return _not_optimization;}
intersect::~intersect(){}

intersect_cmp(intersect *a,intersect *b){return a->not_optimization()<b->not_optimization();}
intersections::intersections(intersect** list,size_t n,int acceleration_type):_list(list),_size(n),_acceleration(acceleration_type)
{
	std::sort(list,list+n,intersect_cmp);
	for(_num=0;_num<n;_num++)if(list[_num]->not_optimization()==1)break;
	// std::cout<<_num<<std::endl;
	if(_acceleration==BVH||(_acceleration==AUTO&&_num>5&&_num<100))root=new bvh_node(list,_num);
	else if((_acceleration==KDTREE||(_acceleration==AUTO&&_num>=100))&&_num)
	{
		aabb bound=list[0]->get_box();
		for(int i=1;i<_num;i++)bound=aabb(bound,list[i]->get_box());
		root=new KD_tree_node(list,_num,(int)(8+1.3*log2(_num)),bound);
	}
}
bool intersections::hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const
{
	hitInfo t_rec;
	bool hitSomething=0;
	value_type far=t_max;//刚开始可以看到无限远
	if(_acceleration==NONE||(_acceleration==AUTO&&_num<=5))
	{
		for(int i=0;i<_size;i++)if(_list[i]->hit(sight,t_min,far,t_rec))
		{
			hitSomething=1;
			far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
			rec=t_rec;
		}
	}
	else if(_acceleration==BVH||(_acceleration==AUTO&&_num>5&&_num<100)||_acceleration==KDTREE||(_acceleration==AUTO&&_num>=100))
	{
		if(root->hit(sight,t_min,t_max,t_rec))
		{
			hitSomething=1;
			far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
			rec=t_rec;
		}//对可加速的进行判断
		for(int i=_num;i<_size;i++)if(_list[i]->hit(sight,t_min,far,t_rec))
		{
			hitSomething=1;
			far=t_rec._t;//将上一次的最近撞击点作为视线可达最远处
			rec=t_rec;
		}//对不可加速的进行暴力枚举
	}
	return hitSomething;
}

camera::camera(const vec_type &eye,const vec_type &start,const vec_type &horizon,const vec_type &vertical):_eye(eye),_start(start),_horizon(horizon),_vertical(vertical){};
camera::camera(const value_type aspect,const value_type vfov,const vec_type &lookfrom,const vec_type &lookat,const value_type focus,const value_type aperture,const value_type t1,const value_type t2,const vec_type &vup):_eye(lookfrom),_len_radius(aperture/2),_time1(t1),_time2(t2)
{
	value_type theta=vfov*pi/180,half_height=tan(theta/2)*focus,half_width=aspect*half_height;
	_w=(lookfrom-lookat).ret_unitization();
	_u=cross(vup,_w).ret_unitization();
	_v=cross(_w,_u);
	_start=_eye-half_width*_u-half_height*_v-focus*_w;
	_horizon=2.*half_width*_u;
	_vertical=2*half_height*_v;
}
const ray camera::get_ray(const value_type u,const value_type v)const
{
	vec_type rd=_len_radius*random_unit_dick();
	value_type time=((_time2-_time1)<eps)?0:_time1+random_unit_figure()*(_time2-_time1);
	return ray(_eye,_start+u*_horizon+v*_vertical-(_eye+_u*rd.x()+_v*rd.y()),time);
}
const ray camera::get_ray(const vec2<value_type> &para)const{return get_ray(para.u(),para.v());}
const vec_type& camera::eye()const{return _eye;}
const vec_type& camera::start()const{return _start;}
const vec_type& camera::horizon()const{return _horizon;}
const vec_type& camera::vertical()const{return _vertical;}
const vec_type& camera::u()const{return _u;}
const vec_type& camera::v()const{return _v;}
const vec_type& camera::w()const{return _w;}
const value_type& camera::lens_r()const{return _len_radius;}