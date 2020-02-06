#include<ray.h>
#include<ctime>
#include<mutex>
#include<thread>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<iostream>
#include<direct.h>
#include<algorithm>
#include<BMPImage.h>
#include<my_random.h>
#include<all_texture.h>
#include<all_objects.h>
#include<condition_variable>
using namespace std;
using namespace gm;
mt19937 sd(time(0));
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="demo.bmp";//图片名称
const int width=2000;//图像水平长度/像素
const int hight=2000;//图像竖直高度/像素
value_type w=100,h=100;
BMPImage bmpImage(width,hight,32);

void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	bmp->at<BGRA>(hight-x-1,width-y-1)=BGRA{(uchar)RGB.b(),(uchar)RGB.g(),(uchar)RGB.r(),255};
}//保存图片
double* generate()
{
	double *p=new double[256];
	for(int i=0;i<256;i++)p[i]=random_unit_figure()*2-1;
	return p;
}
double* _randomvalue;
int *_perm_x,*_perm_y,*_perm_z;
int* generate_perm()
{
	int *p=new int[256];
	for(int i=0;i<256;i++)p[i]=i;
	//random_shuffle(p,p+256,seed);
	shuffle(p,p+256,sd);
	return p;
}
const value_type interp(double list[2][2],value_type u,value_type v)
{
	u=6*u*u*u*u*u-15*u*u*u*u+10*u*u*u;
	v=6*v*v*v*v*v-15*v*v*v*v+10*v*v*v;
	w=6*w*w*w*w*w-15*w*w*w*w+10*w*w*w;
	value_type accumulate=0;
	for(int i=0;i<=1;i++)
	for(int j=0;j<=1;j++)accumulate+=(i*u+(1-i)*(1-u))*(j*v+(1-j)*(1-v))*list[i][j];//这样算出来最大值只有二分之根号三 sqrt((u-i)*(u-i)+(v-j)*(v-j)+(w-k)*(w-k))
	return accumulate;
}
const value_type noise(vec2<double> p)
{
	int x=floor(p.x()),y=floor(p.y());
	value_type u=p.x()-x,v=p.y()-y;
	double list[2][2];
	for(int i=0;i<=1;i++)
	for(int j=0;j<=1;j++)list[i][j]=_randomvalue[_perm_x[(x+i)&255]^_perm_y[(y+j)&255]];
	return interp(list,u,v);
	// int i=int(4*p.x())&255,j=int(4*p.y())&255,k=int(4*p.z())&255;
	// return _randomvalue[_perm_x[i]^_perm_y[j]^_perm_z[k]];
}
const value_type turb(vec2<double>p,int depth=7)
{
	value_type accumulate=0,weight=1;
	for(int i=0;i<depth;i++,weight*=0.5,p*=2)accumulate+=weight*noise(p);
	return accumulate;
}

int main()
{
	_randomvalue=generate();
	_perm_x=generate_perm();
	_perm_y=generate_perm();
	_perm_z=generate_perm();
	for(int i=0;i<hight;i++)
	for(int j=0;j<width;j++)
	{
		vec2<double>p((double)i/hight*h,(double)j/width*h);
		value_type t=turb(p);
		if((i+1)%200==0||(j+1)%200==0)setpoint(&bmpImage,i,j,vec_type(255,0,0));
		else if(t>0)setpoint(&bmpImage,i,j,vec_type(0,255,0)*t);
		else setpoint(&bmpImage,i,j,-vec_type(0,0,255)*t);
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