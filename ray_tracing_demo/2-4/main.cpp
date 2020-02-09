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
#include<BMPImage.h>
#include<all_tools.h>
#include<all_texture.h>
#include<all_objects.h>
#include<all_material.h>
#include<condition_variable>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="demo.bmp";//图片名称
const int width=800;//图像水平长度/像素
const int height=400;//图像竖直高度/像素
const int times=200;//随机周围点的数量
const int tnum=20;//线程数
const int times_=0;//每次碰撞后反射次数
const int max_depth=50;//最多碰撞次数
const value_type theta=90.;//相机在垂直方向上从屏幕顶端扫描到底部所岔开的视角角度
BMPImage bmpImage(width,height,32);
intersect* world;
camera cma(width/height,theta,vec_type(1,1,1));//眼睛（相机）

void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	bmp->at<BGRA>(height-x-1,width-y-1)=BGRA{(uchar)RGB.b(),(uchar)RGB.g(),(uchar)RGB.r(),255};
}//保存图片

vec_type lerp(const ray &sight,const intersect *world,int depth)
{
	if(depth>max_depth)return vec_type(0,0,0);//超过的最大的递归次数
	hitInfo rec;//撞击信息
	if(world->hit(sight,0.,INF,rec))//如果光线打到了某个物体
	{
		// cout<<"hhhhhhhhhhhhhhhhhhhhhhhhh"<<endl;
		// cout<<rec._t<<endl;
		// cout<<sight.origin().x()<<" "<<sight.origin().y()<<" "<<sight.origin().z()<<endl;
		// cout<<sight.direction().x()<<" "<<sight.direction().y()<<" "<<sight.direction().z()<<endl;
		// cout<<rec._p.x()<<" "<<rec._p.y()<<" "<<rec._p.y()<<endl;
		ray scattered;//反射（折射）光线
		vec_type attenuation(0,0,0),emit(0,0,0);//颜色衰减
		int t=times_/depth+1;//前面撞击时，对图片的影响比较大，应该多次取样
		vec_type color(0,0,0);
		for(int i=0;i<t;i++)
		{
			rec.materialp->scatter(sight,rec,attenuation,emit,scattered);//得到反射（折射）光线与颜色衰减
			if(attenuation.r()<eps&&attenuation.g()<eps&&attenuation.g()<eps)color+=emit;
			else color+=emit+attenuation*lerp(scattered,world,depth+1);//递归
		}
		return color/t;
	}
	else//否则返回天空颜色
	{
		return vec_type(0,0,0);
		vec_type dirUnit=sight.direction().ret_unitization();
		value_type t=0.5*(dirUnit.y()+1.);
		return (1.-t)*vec_type(1.,1.,1.)+t*vec_type(.5,.7,1.);
	}
}
int x,y,tcnt;//x,y为当前处理的像素坐标，tcnt为当前线程数
mutex mtx,mx;//线程锁
condition_variable cv;
vec_type btmap[height][width];//位图

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
		if(y==width)y=0,x++;//对x,y拷贝，并处理下一个像素
		mtx.unlock();

		// if(i%10==0)cout<<i<<" "<<j<<endl;
		if(i==height)break;//所有像素处理完毕，推出线程
		vec_type color(0,0,0);
		for(int k=0;k<times;k++)//多次处理
		{
			//cout<<i<<" "<<j<<" "<<k<<endl;
			vec2<value_type> para((j+random_unit_figure())/width,((double)i+random_unit_figure())/height);
			// vec2<value_type> para(value_type(j)/width,value_type(i)/height);
			color+=lerp(cma.get_ray(para),world,1);
		}
		color=vec_type(sqrt(color.r()/times),sqrt(color.g()/times),sqrt(color.b()/times));
		btmap[i][j]=color;
	}

	mtx.lock();
	tcnt--;
	unique_lock<mutex>lock(mx);
	if(!tcnt)cv.notify_all();//当所有线程结束时唤醒main
	mtx.unlock();
}
vector<intersect*>objects;
int main()
{
	// cout<<hello()<<endl;
	BMPImage texture_ball("./final.bmp");
	vec3<int> *texture_=new vec3<int>[texture_ball.height()*texture_ball.width()];
	// cout<<texture.height()<<" "<<texture.width()<<endl;
	for(int i=0;i<texture_ball.height();i++)
	for(int j=0;j<texture_ball.width();j++)
	{
		BGRA value=texture_ball.at<BGRA>(i,j);
		texture_[i*texture_ball.width()+j]=vec3<int>(value.r,value.g,value.b);
	}
	// camera f(60,width/height,vec_type(0,2,0));
	clock_t start=clock(),finish;

	// objects.push_back(new sphere(vec_type(0,-1000,0),1000,new lambertain(new noise_texture(vec_type(1,1,1),0.5))));
	// objects.push_back(new sphere(vec_type(0,4,0),4,new lambertain(new marble(vec_type(1,1,1),vec_type(2,0,5),1,10))));
	texture *pp;
	objects.push_back(new flat(vec_type(0,-1,0),vec_type(0,1,0),new lambertain(pp=new checker_texture(new constant_texture(vec_type(0.8,0.8,0.)),new constant_texture(vec_type(1,1,1))),pp,1)));
	// objects.push_back(new sphere(vec_type(0,-100.5,0),100,new lambertain(pp=new checker_texture(new constant_texture(vec_type(0.8,0.8,0.)),new constant_texture(vec_type(1,1,1))),pp,1)));
	objects.push_back(new sphere(vec_type(1,0,-1),0.5,new lambertain(new marble(vec_type(1,1,1),vec_type(5,0,10),1,10))));
	objects.push_back(new sphere(vec_type(0,0,-1),0.5,new dielectric(1.5,new constant_texture(vec_type(1,1,1)),new constant_texture(vec_type(0.5,0,1)),0.2)));
	objects.push_back(new sphere(vec_type(-1,0,-1),0.5,new metal(new constant_texture(vec_type(.8,.6,.2)))));
	objects.push_back(new sphere(vec_type(0,0,-5),0.45,new lambertain(new constant_texture(vec_type(.9,.4,.8)))));
	objects.push_back(new sphere(vec_type(2,0,-1),0.5,new lambertain(new picture(texture_,texture_ball.width(),texture_ball.height()),new constant_texture(vec_type(0,1,1)),0.3)));
	objects.push_back(new sphere(vec_type(0,0.7,-0.5),0.2,new lambertain(new constant_texture(vec_type(.9,.2,.6)))));
	// objects.push_back(new triangle(vec_type(0,-0.5,-1),vec_type(1,0,0),vec_type(0,1,0),new lambertain(new constant_texture(vec_type(.8,.5,.6)))));
	objects.push_back(new cuboid(vec_type(-2,-0.5,-1),vec_type(-3,-0.5,-2),vec_type(0,0,-1),2,new lambertain(new constant_texture(vec_type(.8,.5,.6))),new lambertain(new constant_texture(vec_type(.2,.5,.6))),new lambertain(new constant_texture(vec_type(.4,.0,1)))));
	intersect **list=new intersect*[objects.size()];
	// for(int i=0;i<objects.size();i++)cout<<objects[i]<<" ";cout<<endl;
	for(int i=0;i<objects.size();i++)list[i]=objects[i];
	world=new intersections(list,objects.size());
	// cout<<"OK"<<endl;
// cout<<endl;
	// world=new intersections(list,objects.size());

	for(int i=0;i<tnum;i++)
	{
		thread t(render);
		t.detach();
	}
	unique_lock<mutex>lock(mx);
	while(tcnt)cv.wait(lock);
	// vec2<value_type> para(584./width,275./height);
	// vec_type debug=lerp(cma.get_ray(para),world,1);
	// cout<<debug.x()<<" "<<debug.y()<<" "<<debug.z()<<endl;


	for(int i=0;i<objects.size();i++)if(list[i])delete list[i];
	if (list)delete[] list;

	for(int i=0;i<height;i++)
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