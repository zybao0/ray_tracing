#include<ray.h>
#include<all_objects.h>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<direct.h>
#include<BMPImage.h>
#include<random>
#include<ctime>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="hello.bmp";//图片名称
const int width=400;//图像水平长度/像素
const int hight=200;//图像竖直高度/像素
const int times=100;//随机周围点的数量
const int tnum=20;//线程数
BMPImage bmpImage(width,hight,32);
intersect* world;
camera cma;
// const value_type INF=0x3f3f3f3f;
/*for(int i=0;i<200;i++)
for(int j=0;j<400;j++)
{
	dvec3 col(double(i)/200,double(j)/400,0.0);
	bmpImage.at<BGRA>(i, j) = BGRA{(int)(255.99*col.b()),(int)(255.99*col.g()),(int)(255.99*col.r()),0};
}*///保存图片样例
void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	bmp->at<BGRA>(hight-x-1,width-y-1)=BGRA{RGB.b(),RGB.g(),RGB.r(),255};
}
const int times_=5;
const int max_depth=20;
const value_type esp=1e-6;
vec_type lerp(const ray &sight,const intersect *world,int depth,vec_type attenua)
{
	hitInfo rec;
	if(world->hit(sight,0.,INF,rec))
	{
		ray scattered;
		vec_type attenuation,attenua_;
		int t=times_/depth+1;
		if(depth>max_depth)return vec_type(0,0,0);
		vec_type color(0,0,0);
		for(int i=0;i<t;i++)
		{
			rec.materialp->scatter(sight,rec,attenuation,scattered);
			attenua_=attenua*attenuation;
			if(attenua_.r()>esp||attenua_.g()>esp||attenua_.b()>esp)color+=attenuation*lerp(scattered,world,depth+1,attenua_);
		}
		return color/t;
	}
	else
	{
		vec_type dirUnit=sight.direction().ret_unitization();
		value_type t=0.5*(dirUnit.z()+1.);
		return (1.-t)*vec_type(1.,1.,1.)+t*vec_type(.5,.7,1.);
	}
}
int x,y,tcnt;
mutex mtx,mx;
condition_variable cv;
vec_type btmap[hight][width];
void render()
{
	mtx.lock();
	tcnt++;
	mtx.unlock();

	while(1)
	{
		mtx.lock();
		int i=x,j=y;
		y++;
		if(y==width)y=0,x++;
		mtx.unlock();

		if(i==hight)break;
		vec_type color(0,0,0);
		for(int k=0;k<times;k++)
		{
			vec2<value_type> para(value_type((double)j+rnd(seed))/width,value_type((double)i+rnd(seed))/hight);
			color+=lerp(cma.get_ray(para),world,1,vec_type(1.,1.,1.));
		}
		color=vec_type(sqrt(color.r()/times),sqrt(color.g()/times),sqrt(color.b()/times));
		btmap[i][j]=color;
	}

	mtx.lock();
	tcnt--;
	unique_lock<mutex>lock(mx);
	if(!tcnt)cv.notify_all();
	mtx.unlock();
}
int main()
{
	int object_num=4;
	intersect **list=new intersect*[object_num];
	list[0]=new sphere(vec_type(0,0,-1),0.5,new lambertain(vec_type(0.8,0.3,0.3)));
	list[1]=new sphere(vec_type(0,-100.5,-1),100,new lambertain(vec_type(0.8,0.8,0.)));
	list[2]=new sphere(vec_type(1,0,-1),0.5,new metal(vec_type(0.8,0.8,0.8)));
	//list[0]=new sphere(vec_type(0,0,-1),0.5,new dielectric(1));
	list[3]=new sphere(vec_type(-1,0,-1),0.5,new metal(vec_type(.8,.6,.2)));
	// list[4]=new sphere(vec_type(1,0,-1),-0.45,new dielectric(1.5));

	world=new intersections(list,object_num);
	

	for(int i=0;i<tnum;i++)
	{
		thread t(render);
		t.detach();
	}
	unique_lock<mutex>lock(mx);
	while(tcnt)cv.wait(lock);


	for(int i=0;i<object_num;i++)if(list[i])delete list[i];
	if (list)delete[] list;

	for(int i=0;i<hight;i++)
	for(int j=0;j<width;j++)setpoint(&bmpImage,i,j,btmap[i][j]*255);
	bmpImage.save(file_name.c_str());//保存图片

	char file_path[256];
	_getcwd(file_path,sizeof(file_path));
	cmd_+=file_path;
	cmd_+="\\"+file_name;
	cout<<cmd_<<endl;
	system(cmd_.c_str());
	//打开图片
}