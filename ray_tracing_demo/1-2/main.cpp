//linear interpolation 
#include<ray.h>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<direct.h>
#include<BMPImage.h>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="hello.bmp";//图片名称
const int width=400;//图像水平长度/像素
const int hight=200;//图像竖直高度/像素
/*for(int i=0;i<200;i++)
for(int j=0;j<400;j++)
{
	dvec3 col(double(i)/200,double(j)/400,0.0);
	bmpImage.at<BGRA>(i, j) = BGRA{(int)(255.99*col.b()),(int)(255.99*col.g()),(int)(255.99*col.r()),0};
}*///保存图片样例
void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	bmp->at<BGRA>(x,y)=BGRA{RGB.b(),RGB.g(),RGB.r(),255};
}
vec_type lerp(const ray &r)
{
	vec_type unit_dir=r.direction().ret_unitization();//将方向向量单位化
	value_type t=.5*(unit_dir.y()+1.0);//将y分量映射导[0,1](屏幕的长度为2,y分量原本的范围为[-1,1])
	return (1.-t)*vec_type(1.,1.,1.)+t*vec_type(0.,0.,1.);//用白色与蓝色线性插值
}
int main()
{
	BMPImage bmpImage(width,hight,32);
	//vec_type 一个三维向量 与光线（视线）的类型相同
	//value_type 上述向量每个分量的类型 默认double
	vec_type left_bottom(-2.,-1.,-1.);//左下角作为开始位置
	vec_type horizontal(4.,0,0);//屏幕水平宽度
	vec_type vertical(0,2.,0);//屏幕垂直高度
	vec_type eye(0,0,0);//见图一

	for(int i=0;i<hight;i++)
	for(int j=0;j<width;j++)
	{
		//cout<<i<<" "<<j<<endl;
		vec2<value_type> para(value_type(j)/width,value_type(i)/hight);
		ray r(eye,left_bottom+para.u()*horizontal+para.v()*vertical);
		//光的起始点为 eye ;方向为left_bottom+para.u*horizontal+para.v*vertical
		vec_type color=lerp(r);
		//cout<<i<<" "<<j<<" "<<color.r()*255<<" "<<color.g()*255<<" "<<color.b()*255<<endl;
		setpoint(&bmpImage,i,j,color*255);
	}

	bmpImage.save(file_name.c_str());//保存图片

	char file_path[256];
	_getcwd(file_path,sizeof(file_path));
	cmd_+=file_path;
	cmd_+="\\"+file_name;
	cout<<cmd_<<endl;
	system(cmd_.c_str());
	//打开图片
}