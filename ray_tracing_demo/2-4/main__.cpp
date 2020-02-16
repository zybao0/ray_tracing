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
const int width=400;//图像水平长度/像素
const int height=200;//图像竖直高度/像素
const int times=50;//随机周围点的数量
const int tnum=20;//线程数
const int times_=0;//每次碰撞后反射次数
const int max_depth=5;//最多碰撞次数
const int save_time=10;//每次保存图片时的间隔/分钟
const value_type theta=90.;//相机在垂直方向上从屏幕顶端扫描到底部所岔开的视角角度

BMPImage bmpImage(width,height,32);
intersect* world;
camera cma(width/height,theta,vec_type(500,278,278),vec_type(278,278,278),100);//眼睛（相机）
// camera cma(width/height,theta,vec_type(1,1,1));//眼睛（相机）

void setpoint(BMPImage *bmp,int x,int y,vec_type RGB)//设置某个像素的RGB
{
	// mat_type(RGB).echo();
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
			// mat_type(emit).echo();
			// mat_type(attenuation).echo();
			if(attenuation.r()<eps&&attenuation.g()<eps&&attenuation.g()<eps)color+=emit;
			else color+=emit+attenuation*lerp(scattered,world,depth+1);//递归
		}
		return color/t;
	}
	else//否则返回天空颜色
	{
		// return vec_type(0,0,0);
		vec_type dirUnit=sight.direction().ret_unitization();
		value_type t=0.5*(dirUnit.y()+1.);
		return (1.-t)*vec_type(1.,1.,1.)+t*vec_type(.5,.7,1.);
	}
}
int x,y,tcnt,completion;//x,y为当前处理的像素坐标，tcnt为当前线程数 completion为当前完成度
mutex mtx,mx;//线程锁
condition_variable cv;
vec_type btmap[height][width];//位图
bool vis[height][width];//用于标记哪些像素已经渲染

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

		if(i==height)break;//所有像素处理完毕，推出线程
		if(vis[i][j])continue;//该像素已经被渲染
		// if(i%10==0)cout<<i<<" "<<j<<endl;
		vec_type color(0,0,0);
		for(int k=0;k<times;k++)//多次处理
		{
			//cout<<i<<" "<<j<<" "<<k<<endl;
			vec2<value_type> para(((value_type)j+random_unit_figure())/width,((value_type)i+random_unit_figure())/height);
			// vec2<value_type> para(value_type(j)/width,value_type(i)/height);
			color+=lerp(cma.get_ray(para),world,1);
			// cout<<(double)color.r()*255<<endl;
		}
		color=vec_type(sqrt(color.r()/times),sqrt(color.g()/times),sqrt(color.b()/times));
		// mat_type(color).echo();
		btmap[i][j]=color;
		vis[i][j]=1;
	}

	mtx.lock();
	tcnt--;
	unique_lock<mutex>lock(mx);
	if(!tcnt)cv.notify_all();//当所有线程结束时唤醒main
	mtx.unlock();
}

void save()
{
	while(1)
	{
		this_thread::sleep_for(std::chrono::minutes(save_time));
		int finish_cnt=0;
		ofstream out("ray_tracing.dat");
		for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)if(vis[i][j])
		{
			out<<i<<" "<<j<<" "<<btmap[i][j].r()<<" "<<btmap[i][j].g()<<" "<<btmap[i][j].b()<<" "<<endl;
			finish_cnt++;
		}
		cout<<"finished: "<<(value_type)finish_cnt/(height*width)*100.<<"%"<<endl;
		out.close();
		out.clear();
	}
}

vector<intersect*>objects;
int main()
{
	memset(vis,0,sizeof(vis));
	bool load_save=0;
	if(load_save)
	{
		ifstream in("ray_tracing.dat");
		while(!in.eof())
		{
			int u,v;
			value_type r,g,b;
			in>>u>>v>>r>>g>>b;
			btmap[u][v]=vec_type(r,g,b);
			vis[u][v]=1;
		}
	}
	BMPImage texture_ball("./final.bmp");
	vec3<int> *texture_=new vec3<int>[texture_ball.height()*texture_ball.width()];
	// cout<<texture.height()<<" "<<texture.width()<<endl;
	for(int i=0;i<texture_ball.height();i++)
	for(int j=0;j<texture_ball.width();j++)
	{
		BGRA value=texture_ball.at<BGRA>(i,j);
		texture_[i*texture_ball.width()+j]=vec3<int>(value.r,value.g,value.b);
	}
	clock_t start=clock(),finish;

	// // objects.push_back(new sphere(vec_type(0,-1000,0),1000,new lambertain(new noise_texture(vec_type(1,1,1),0.5))));
	// // objects.push_back(new sphere(vec_type(0,4,0),4,new lambertain(new marble(vec_type(1,1,1),vec_type(2,0,5),1,10))));
	// texture *pp;
	// intersect* cube=new cuboid(vec_type(-2,-0.5,-1),vec_type(-3,-0.5,-2),vec_type(0,0,-1),2,new lambertain(new constant_texture(vec_type(.8,.5,.6))),new lambertain(new constant_texture(vec_type(.2,.5,.6))),new lambertain(new constant_texture(vec_type(.4,.0,1))));
	// objects.push_back(new flat(vec_type(0,-1,0),vec_type(0,1,0),new lambertain(pp=new checker_texture(new constant_texture(vec_type(0.8,0.8,0.)),new constant_texture(vec_type(1,1,1))),pp,1)));
	// // objects.push_back(new sphere(vec_type(0,-100.5,0),100,new lambertain(pp=new checker_texture(new constant_texture(vec_type(0.8,0.8,0.)),new constant_texture(vec_type(1,1,1))),pp,1)));
	// objects.push_back(new sphere(vec_type(1,0,-1),0.5,new lambertain(new marble(vec_type(1,1,1),vec_type(5,0,10),1,10))));
	// objects.push_back(new sphere(vec_type(0,0,-1),0.5,new dielectric(1.5,new constant_texture(vec_type(1,1,1)),new constant_texture(vec_type(0.5,0,1)),0.2)));
	// objects.push_back(new sphere(vec_type(-1,0,-1),0.5,new metal(new constant_texture(vec_type(.8,.6,.2)))));
	// objects.push_back(new sphere(vec_type(0,0,-5),0.45,new lambertain(new constant_texture(vec_type(.9,.4,.8)))));
	// objects.push_back(new sphere(vec_type(2,0,-1),0.5,new lambertain(new picture(texture_,texture_ball.width(),texture_ball.height()),new constant_texture(vec_type(0,1,1)),0.3)));
	// // objects.push_back(new triangle(vec_type(0,-0.5,-1),vec_type(1,0,0),vec_type(0,1,0),new lambertain(new constant_texture(vec_type(.8,.5,.6)))));
	// objects.push_back(new transform(vec_type(-2,-0.5,-2),mat_type(vector<vector<double>>{vector<double>{sqrt(3)/2.,0,0.5},vector<double>{0,1,0},vector<double>{-0.5,0,sqrt(3)/2.}}),vec_type(0,0,0),cube));
	

	intersect* cube1=new cuboid(vec_type(0,0,0),vec_type(165,0,165),vec_type(0,0,165),165,new lambertain(new constant_texture(vec_type(.9,.0,.8))),new lambertain(new constant_texture(vec_type(.9,.9,.4))),new lambertain(new constant_texture(vec_type(.0,.5,.8))));
	intersect* cube2=new cuboid(vec_type(0,0,0),vec_type(165,0,165),vec_type(0,0,165),300,new lambertain(new constant_texture(vec_type(.0,.9,.8))),new lambertain(new constant_texture(vec_type(.9,.7,.3))),new lambertain(new constant_texture(vec_type(.3,.7,.8))));
	
	
	objects.push_back(new rectangle(vec_type(200,555-eps,200),vec_type(355,555-eps,355),vec_type(0,0,200),new lambertain(new constant_texture(vec_type(1,1,1)),new constant_texture(vec_type(5,5,5)),1)));
	objects.push_back(new cuboid(vec_type(0,0,0),vec_type(555,0,555),vec_type(0,0,555),555,new lambertain(new constant_texture(vec_type(1,.5,.5))),new lambertain(new constant_texture(vec_type(.5,.1,.5))),new lambertain(new constant_texture(vec_type(.5,.5,1)))));
	objects.push_back(new transform(vec_type(0,0,0),mat_type(vector<vector<double>>{vector<double>{0.7071,-0.3535,0.6123},vector<double>{0,0.8660,0.5},vector<double>{-0.7071,-0.3535,0.6123}}),vec_type(130,200,65),cube1));
	objects.push_back(new transform(vec_type(0,0,0),mat_type(vector<vector<double>>{vector<double>{0.8137,-0.2961,-0.5},vector<double>{0.3420,0.9396,0},vector<double>{0.4698,-0.1710,0.8660}}),vec_type(256,60,295),cube2));
	// objects.push_back(new transform(vec_type(0,0,0),mat_type(vector<vector<double>>{vector<double>{1,0,0},vector<double>{0,1,0},vector<double>{0,0,1}}),vec_type(265,200,465),cube1));
	// objects.push_back(new transform(vec_type(0,0,0),mat_type(vector<vector<double>>{vector<double>{1,0,0},vector<double>{0,1,0},vector<double>{0,0,1}}),vec_type(30,60,365),cube2));
	// objects.push_back(cube1);
	// objects.push_back(cube2);

	intersect **list=new intersect*[objects.size()];
	// for(int i=0;i<objects.size();i++)cout<<objects[i]<<" ";cout<<endl;
	for(int i=0;i<objects.size();i++)list[i]=objects[i];
	world=new intersections(list,objects.size());
	// world=new intersections(list,objects.size());

	for(int i=0;i<tnum;i++)
	{
		thread t(render);
		t.detach();
	}
	thread sv(save);
	sv.detach();
	unique_lock<mutex>lock(mx);
	while(tcnt)cv.wait(lock);

	// vec2<value_type> para(290./width,400./height);
	// vec_type debug=lerp(cma.get_ray(para),world,1);
	// cout<<debug.x()<<" "<<debug.y()<<" "<<debug.z()<<endl;

	// for(int i=0;i<height;i++)
	// for(int j=0;j<width;j++)
	// {
	// 	// cout<<i<<" "<<j<<endl;
	// 	vec2<value_type> para(((double)j)/width,((double)i)/height);
	// 		// vec2<value_type> para(value_type(j)/width,value_type(i)/height);
	// 		lerp(cma.get_ray(para),world,1);
	// }


	for(int i=0;i<objects.size();i++)if(list[i])delete list[i];
	if (list)delete[] list;

	for(int i=0;i<height;i++)
	for(int j=0;j<width;j++)
	{
		// cout<<i<<" "<<j<<endl;
		// mat_type( btmap[i][j]).echo();
		aabb col(vec_type(255,255,255),btmap[i][j]*255);//若rgb中有大于255的，取255
		setpoint(&bmpImage,i,j,col.min());
		// setpoint(&bmpImage,i,j,btmap[i][j]*255);
	}

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