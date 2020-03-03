#include<ctime>
#include<random>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<tools/myRandom.h>
using namespace std;
const value_type eps=1e-9;
const value_type sqrt2=sqrt(2);
const value_type sqrt3=sqrt(3);
const value_type sq3_=2./sqrt(3);
mt19937 seed(time(0));
uniform_real_distribution<value_type>rnd;
const value_type random_unit_figure()
{
	value_type t;
	t=rnd(seed);
	return t;
}
const vec_type random_unit_dick()
{
	vec_type p=2.*vec_type(random_unit_figure(),random_unit_figure(),0.)-vec_type(1.,1.,0.);
	while(dot(p,p)+eps>=1.)p=2.*vec_type(random_unit_figure(),random_unit_figure(),0.)-vec_type(1.,1.,0.);
	return p;
}
const vec_type random_unit_sphere()
{
	vec_type p=2.*vec_type(random_unit_figure(),random_unit_figure(),random_unit_figure())-vec_type(1.,1.,1.);
	while(dot(p,p)+eps>=1.)p=2.*vec_type(random_unit_figure(),random_unit_figure(),random_unit_figure())-vec_type(1.,1.,1.);
	return p;
}
const int random_int(int l,int r)
{
	int len=r-l+1;
	value_type tmp=((value_type)len-eps)*random_unit_figure();
	return floor(tmp);
}


vec_type* perlin_generate()
{
	vec_type *p=new vec_type[256];
	for(int i=0;i<256;i++)p[i]=vec_type(random_unit_figure()*2-1,random_unit_figure()*2-1,random_unit_figure()*2-1).ret_unitization();
	return p;
}
int* perlin_generate_perm()
{
	int *p=new int[256];
	for(int i=0;i<256;i++)p[i]=i;
	//random_shuffle(p,p+256,seed);
	shuffle(p,p+256,seed);
	return p;
}

value_type integer_radical_inverse(int base,int num)//把这个base进制的数镜像到小数点右边去
{
	long long p=1,inv=0;
	for(;num>0;num/=base,p*=(long long)base)inv=inv*base+(long long)(num%base);
	return (value_type)inv/p;//除以p将这个数镜像到小数点右边

}
value_type halton(int dimension, int index){return integer_radical_inverse(special_numbers::prime[dimension-1],index);}
value_type hammersley(int dimension,int index,int n){return dimension==1?(value_type)index/n:integer_radical_inverse(special_numbers::prime[dimension-2],index);}
value_type halton_scrambling(int index){return (special_numbers::faure_permutation[index%125u]*1953125u+special_numbers::faure_permutation[(index/125u)%125u]*15625u+special_numbers::faure_permutation[(index/15625u)%125u]*125u+special_numbers::faure_permutation[(index/1953125u)%125u])*(0x1.fffffep-1/244140625u);}


value_type return_self(value_type u){return u;}
value_type trans_function(value_type u){return u*u*u*(u*(6*u-15)+10);}
value_type trans_function2(value_type u){return trans_function(sqrt(u));}//一个我乱搞的玄学调差
value_type trans_function3(value_type u){return sqrt(trans_function(u));}//一个我乱搞的玄学调差

Perlin::Perlin()
{
	_randomvalue=perlin_generate();
	_perm_x=perlin_generate_perm();
	_perm_y=perlin_generate_perm();
	_perm_z=perlin_generate_perm();
}
const vec_type* Perlin::randomvalue()const{return _randomvalue;}
const int* Perlin::perm_x()const{return _perm_x;}
const int* Perlin::perm_y()const{return _perm_y;}
const int* Perlin::perm_z()const{return _perm_z;}
const value_type Perlin::interp(vec_type list[2][2][2],value_type u,value_type v,value_type w)const
{
	// u=6*u*u*u*u*u-15*u*u*u*u+10*u*u*u;
	// v=6*v*v*v*v*v-15*v*v*v*v+10*v*v*v;
	// w=6*w*w*w*w*w-15*w*w*w*w+10*w*w*w;
	u=trans_function(u);
	v=trans_function(v);
	w=trans_function(w);
	value_type accumulate=0;
	for(int i=0;i<=1;i++)
	for(int j=0;j<=1;j++)
	for(int k=0;k<=1;k++)accumulate+=(i*u+(1-i)*(1-u))*(j*v+(1-j)*(1-v))*(k*w+(1-k)*(1-w))*dot(list[i][j][k],vec_type(u-i,v-j,w-k));//这样算出来最大值只有二分之根号三 sqrt((u-i)*(u-i)+(v-j)*(v-j)+(w-k)*(w-k))
	//return ((sq3_*accumulate)+1)*0.5;
	return abs(sq3_*accumulate);
}
const value_type Perlin::noise(const vec_type &p,trans_func f)const
{
	int x=floor(p.x()),y=floor(p.y()),z=floor(p.z());
	value_type u=p.x()-x,v=p.y()-y,w=p.z()-z;
	vec_type list[2][2][2];
	for(int i=0;i<=1;i++)
	for(int j=0;j<=1;j++)
	for(int k=0;k<=1;k++)list[i][j][k]=_randomvalue[_perm_x[(x+i)&255]^_perm_y[(y+j)&255]^_perm_z[(z+k)&255]];
	return f(interp(list,u,v,w));
	// int i=int(4*p.x())&255,j=int(4*p.y())&255,k=int(4*p.z())&255;
	// return _randomvalue[_perm_x[i]^_perm_y[j]^_perm_z[k]];
}
const value_type Perlin::turb(vec_type p,trans_func f,int depth)const
{
	value_type accumulate=0,weight=0.5;
	for(int i=0;i<depth;i++,weight*=0.5,p*=2)accumulate+=weight*noise(p,f);
	accumulate+=2*weight*noise(p);
	return accumulate;
}
