#include<ray.h>
#include<all_objects.h>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<direct.h>
#include<BMPImage.h>
#include<my_random.h>
#include<ctime>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
using namespace std;
using namespace gm;
string cmd_="rundll32.exe C:\\Windows\\System32\\shimgvw.dll,ImageView_Fullscreen ";//打开图片的命令
string file_name="demo.bmp";//图片名称
const int width=400;//图像水平长度/像素
const int hight=200;//图像竖直高度/像素
const int times=200;//随机周围点的数量
const int tnum=20;//线程数
const int times_=0;//每次碰撞后反射次数
const int max_depth=50;//最多碰撞次数
const value_type theta=90.;//相机在垂直方向上从屏幕顶端扫描到底部所岔开的视角角度
int x,y,tcnt;//x,y为当前处理的像素坐标，tcnt为当前线程数
mutex mtx,mx;//线程锁
condition_variable cv;
void render()
{
	mtx.lock();
	int k=++tcnt;
	mtx.unlock();

	for(int i=1;i<=10;i++)cout<<k<<" "<<i<<" "<<random_unit_figure()<<endl;

	mtx.lock();
	tcnt--;
	unique_lock<mutex>lock(mx);
	if(!tcnt)cv.notify_all();//当所有线程结束时唤醒main
	mtx.unlock();
}
int main()
{
	for(int i=0;i<tnum;i++)
	{
		thread t(render);
		t.detach();
	}
	unique_lock<mutex>lock(mx);
	while(tcnt)cv.wait(lock);
}