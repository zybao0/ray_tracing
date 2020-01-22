#include<ray.h>
#include<all_objects.h>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<direct.h>
#include<BMPImage.h>
#include<random>
#include<ctime>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="hello.bmp";//图片名称
const int width=400;//图像水平长度/像素
const int hight=200;//图像竖直高度/像素
const int times=1000;//随机周围点的数量
mt19937 seed(time(0));
uniform_real_distribution<value_type>rnd;
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
vec_type lerp(const ray &sight,const intersect *world)
{
	hitInfo rec;
	if(world->hit(sight,0.,INF,rec))return 0.5*vec_type(rec._n.x()+1.,rec._n.y()+1,rec._n.z()+1.);
	else
	{
		vec_type dirUnit=sight.direction().ret_unitization();
		value_type t=0.5*(dirUnit.y()+1.);
		return (1.-t)*vec_type(1.,1.,1.)+t*vec_type(.5,.7,1.);
	}
}
int main()
{
	BMPImage bmpImage(width,hight,32);

	intersect **list=new intersect*[2];
	list[0]=new sphere(vec_type(0,0,-1),.5);
	list[1]=new sphere(vec_type(0,-100.5,-1),100);
	intersect* world=new intersections(list,2);
	camera cma;

	for(int i=0;i<hight;i++)
	for(int j=0;j<width;j++)
	{
		vec_type color(0,0,0);
		for(int k=0;k<times;k++)
		{
			vec2<value_type> para(value_type((double)j+rnd(seed))/width,value_type((double)i+rnd(seed))/hight);
			color+=lerp(cma.get_ray(para),world);
		}
		setpoint(&bmpImage,i,j,color/times*255);
	}

	if (list[0])delete list[0];
	if (list[1])delete list[1];
	if (list)delete[] list;

	bmpImage.save(file_name.c_str());//保存图片

	char file_path[256];
	_getcwd(file_path,sizeof(file_path));
	cmd_+=file_path;
	cmd_+="\\"+file_name;
	cout<<cmd_<<endl;
	system(cmd_.c_str());
	//打开图片
}