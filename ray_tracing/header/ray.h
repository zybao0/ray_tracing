#pragma once
#include"all_tools.h"
using namespace gm;

using value_type=precision;
using vec_type=vec3<value_type>;
using mat_type=matrix<value_type>;

const vec_type Vup=vec_type(0,1,0);//世界上方的方向

const value_type INF=0x3f3f3f3f;
const value_type eps=1e-9;
const value_type pi=acos(-1);

const int monte_carlo_rand_times=20;//有时我们需要使用蒙特卡洛求面积，该数为求面积时随机次数 

class ray
{
	public:
		ray();
		ray(const vec_type &a,const vec_type &b,const value_type c=0);
		ray(const ray& r);
		vec_type origin()const;
		vec_type direction()const;
		value_type time()const;
		vec_type go(const value_type t)const;
	private:
		vec_type _a,_b;//_a:origin(光的起点) _b:direction(光的方向)
		value_type _time;
};



class texture
{
	public:
		virtual vec_type value(value_type u,value_type v,const vec_type &p)const=0;//p为撞击点,返回衰减向量
};



class pdf
{
	public:
		virtual value_type value(const vec_type &origin,const vec_type &direction)const=0;//根据向量求出概率密度
		virtual vec_type generate(const vec_type &origin)const=0;//获得根据pdf生成的随机向量
};



class material;

struct hitInfo
{
	value_type _t,_u,_v;//ray 中的系数t(direction的t倍) 材质中的 x,y 
	vec_type _p,_n;//_p:相交点、撞击点 _n:_p点的表面法线
	material *materialp;//材料
};

/*
@brief: produce a scattered ray
@param: InRay -> Incident light
		info -> the information of intersect-point(hit-point)
		attenuation -> when scattered, how much the ray should be attenuated by tis reflectance R
		scattered -> as we talk, it is a new sight; or
					it is the scattered ray with the intersect-point
@retur: the function calculate a scattered ray or not

diffuse表面：1.视线与物体表面产生撞击点p，在p处相切单位圆内随机找一点s，散射光方向即p->s
　　　　　　 2.我们上一篇采用的光线强度衰减机制是取半。
这一篇中我们将
metal表面： 1.根据物理反射定律确定入射光对应的反射光的方向
　　　　　　2.强度衰减改为三元组，分别对应rgb三分量的衰减度，且用参数自由确定
*/
class material
{
	public:
		virtual bool scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered,value_type &pdf_value)const=0;
	//protected:
	public:
		vec_type reflect(const vec_type &in,const vec_type &n)const;//反射
		bool refract(const vec_type &InRay,const vec_type &n,value_type eta,vec_type &reflected)const;//折射
		value_type schlick(const value_type cosine,const value_type RI)const;
	protected:
		texture *_albedo,*_emit;//材质（衰弱三元组）发光
		value_type _intensity;//光照强度
		pdf *_pdf;//使用蒙特卡洛
};




#define AUTO 0
#define NONE 1
#define BVH  2
#define KDTREE 3//加速方式

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class aabb//平行轴包围盒
{
	public:
		aabb();
		aabb(const vec_type &a,const vec_type &b);//任意对角坐标
		aabb(const aabb &a,const aabb &b);//移动的物体
		bool hit(const ray &sight,value_type tmin,value_type tmax)const;
		const vec_type& min()const;
		const vec_type& max()const;
		const value_type& area()const;
	private:
		vec_type _min,_max;//_min(_max):长方体8个顶点中坐标的x,y,z均最小(大)的点
		value_type _area;//长方体表面积
};

/*
而我们这个类完成的就是前半部分：计算光线相交点，或者说是交叉点，或者说是撞击点。
所以讲基类命名为intersect
因为在实际操作中可能需要对根进行条件过滤，所以，我们在hit中增加了关于系数t的上限和下限，增加灵活度，强化用户体验。
*/
class intersect
{
	public:
		// constexpr static value_type inf();
		/*
		@brief: 撞击函数，求取撞击点相关记录信息
		@param: sight->视线
				系数t的上下界->筛选撞击点
				rec->返回撞击点信息
		@retur,: 是否存在合法撞击点
		*/
		intersect();
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const=0;
		virtual value_type size()const;
		virtual vec_type random_poin()const;
		const aabb& get_box()const;
		bool not_optimization()const;
		virtual ~intersect();
	protected:
		bool _not_optimization;//有些物体（如无穷大平面）返回aabb没有意义，说以选择不优化
		aabb _box;
};

class bvh_node:public intersect
{
	public:
		bvh_node(intersect**world,const int n);
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
	private:
		intersect *_left,*_right;
};

class KD_tree_node:public intersect
{
	public:
		KD_tree_node(intersect**world,const int n,const int depth,aabb bound);
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
	private:
		bool split;
		int _split_axis;
		value_type _split_pos;
		intersect *_left,*_right;
};

class KD_tree_leaf:public intersect
{
	public:
		KD_tree_leaf(intersect **world,const int n,aabb bound);
		virtual bool hit(const ray &sight,value_type t_min,value_type t_max,hitInfo &rec)const override;
	private:
		intersect **_list;
		size_t _size;//叶节点物体数量
};

/*
顾名思义，这个就是用于记录多个交叉点的一个表
它包含一个二维指针，高维指的是一个有关于基类指针的数组，低维度就是指向基类——intersect的一个多态指针。
而它的hit函数就是，遍历每一个sphere对象，求取得到视线穿过的离eye最近的交叉点。扫描屏幕的每一条视线均如此做，
可翻阅上一篇，我们的3条line的那个实线和虚线图，对于每一条视线，如果与多个对象存在交叉点，
那么最短的那一条是实线，我们求取的始终是实线部分，而实线的长，就是t
*/
class intersections:public intersect
{
	public:
		intersections(intersect** list,size_t n,int acceleration_type=AUTO);
		virtual bool hit(const ray& sight,value_type t_min,value_type t_max,hitInfo& rec)const override;
		// ~intersections();
	private:
		intersect** _list;//一个指针数组 指向每一个实体
		size_t _size,_num;//物体个数；可以优化的物体个数
		intersect *root;
		int _acceleration;
};




class camera
{
	public:
		camera(const vec_type &eye=vec_type(0.,0.,0.),const vec_type &start=vec_type(-2.,-1.,-1.),const vec_type &horizon=vec_type(4.,0.,0.),const vec_type &vertical=vec_type(0.,2.,0.));//默认参数
		camera(const value_type aspect,const value_type vfov=90.,const vec_type &lookfrom=vec_type(0.,0.,0.),const vec_type &lookat=vec_type(0.,0.,-1.),const value_type focus=1,const value_type aperture=0,const value_type t1=0,const value_type t2=0,const vec_type &vup=Vup);
		//vfov:相机在垂直方向上从屏幕顶端扫描到底部所岔开的视角角度;aspect:屏幕宽高比;lookfrom:眼睛位置;focus:焦距;aperture:光圈直径
		const ray get_ray(const value_type u,const value_type v)const;
		const ray get_ray(const vec2<value_type> &para)const;
		const vec_type& eye()const;
		const vec_type& start()const;
		const vec_type& horizon()const;
		const vec_type& vertical()const;
		const vec_type& u()const;
		const vec_type& v()const;
		const vec_type& w()const;
		const value_type& lens_r()const;
	private:
		vec_type _eye,_start,_horizon,_vertical,_u,_v,_w;//相机位置，开始位置（左下角），屏幕水平宽度，屏幕垂直高度，图像水平方向单位向量，图像竖直方向单位向量，世界竖直方向单位向量
		value_type _len_radius,_time1,_time2;//光圈半径
};

