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
#include<vector>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="demo.bmp";//图片名称
const int width=1600;//图像水平长度/像素
const int hight=900;//图像竖直高度/像素
const int times=200;//随机周围点的数量
const int tnum=20;//线程数
const int times_=5;//每次碰撞后反射次数
const int max_depth=50;//最多碰撞次数
BMPImage bmpImage(width,hight,32);
intersect* world;
camera cma(90,width/hight,vec_type(-1,1,1));

void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	bmp->at<BGRA>(hight-x-1,width-y-1)=BGRA{RGB.b(),RGB.g(),RGB.r(),255};
}

vec_type lerp(const ray &sight,const intersect *world,int depth)
{
	hitInfo rec;
	if(world->hit(sight,0.,INF,rec))
	{
		ray scattered;
		vec_type attenuation;
		int t=times_/depth+1;
		if(depth>max_depth)return vec_type(0,0,0);
		vec_type color(0,0,0);
		for(int i=0;i<t;i++)
		{
			rec.materialp->scatter(sight,rec,attenuation,scattered);
			color+=attenuation*lerp(scattered,world,depth+1);
		}
		return color/t;
	}
	else
	{
		vec_type dirUnit=sight.direction().ret_unitization();
		value_type t=0.5*(dirUnit.y()+1.);
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
			// vec2<value_type> para(value_type(j)/width,value_type(i)/hight);
			color+=lerp(cma.get_ray(para),world,1);
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
vector<intersect*>objects;
int main()
{
	// camera f(60,width/hight,vec_type(0,2,0));
	clock_t start=clock(),finish;

	objects.push_back(new sphere(vec_type(0,-100.5,-1),100,new lambertain(vec_type(0.8,0.8,0.))));
	objects.push_back(new sphere(vec_type(1,0,-1),0.5,new lambertain(vec_type(.5,.2,.9))));
	objects.push_back(new sphere(vec_type(0,0,-1),0.5,new dielectric(1.5)));
	objects.push_back(new sphere(vec_type(-1,0,-1),0.5,new metal(vec_type(.8,.6,.2))));
	objects.push_back(new sphere(vec_type(0,0,-5),0.45,new lambertain(vec_type(.9,.4,.8))));
	objects.push_back(new sphere(vec_type(0,0.7,-0.5),0.2,new lambertain(vec_type(.9,.2,.6))));
	intersect **list=new intersect*[objects.size()];
	for(int i=0;i<objects.size();i++)list[i]=objects[i];
	world=new intersections(list,objects.size());
	

	for(int i=0;i<tnum;i++)
	{
		thread t(render);
		t.detach();
	}
	unique_lock<mutex>lock(mx);
	while(tcnt)cv.wait(lock);

	for(int i=0;i<objects.size();i++)if(list[i])delete list[i];
	if (list)delete[] list;

	for(int i=0;i<hight;i++)
	for(int j=0;j<width;j++)setpoint(&bmpImage,i,j,btmap[i][j]*255);

	bmpImage.save(file_name.c_str());//保存图片

	finish=clock();
	cout<<"time="<<double(finish-start)/CLOCKS_PER_SEC<<"s"<<endl;

	char file_path[256];
	_getcwd(file_path,sizeof(file_path));
	cmd_+=file_path;
	cmd_+="\\"+file_name;
	cout<<cmd_<<endl;
	system(cmd_.c_str());
	//打开图片
}