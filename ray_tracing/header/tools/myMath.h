#pragma once
#include<vector>
#include"myVec2.h"
#include"myVec3.h"
using namespace gm;
using namespace std;
using value_type=precision;
using vec_type=vec3<value_type>;
void get_sphere_uv(const vec_type &p,value_type &u,value_type &v,const vec_type &Vup);
value_type get_triangle_uv(const vec_type &origin,const vec_type &direction,const vec_type &p,const vec_type &e1,const vec_type &e2,value_type &t,value_type &u,value_type &v);//当光线与e1,e2,p所在的平面相交时 求出p与交点成的向量用u*e1+v*e2表示，可用于求光线与三角形没有吗交点


template<typename T>class matrix
{
	public:
		matrix();
		matrix(const vector<vector<T>> &a);
		matrix(const vec3<T> &a);
		matrix(size_t n,size_t m,T **a);
		matrix(size_t n,size_t m);
		~matrix();
		inline T** mat();
		inline int n()const;
		inline int m()const;
		inline vec3<T> to_vec()const;
		inline const T& f(int x,int y)const;
		inline matrix<T> transpose()const;
		inline matrix<T> LU()const;
		inline matrix<T> LU(matrix<T> &p)const;
		inline matrix<T> gauss(matrix<T> &p)const;
		inline T* operator[](const int x)const;
		inline const matrix<T>& operator=(const matrix<T> &m1);
		inline const matrix<T>& operator+=(const matrix<T> &m1);
		inline const matrix<T>& operator-=(const matrix<T> &m1);
		inline const matrix<T>& operator*=(const matrix<T> &m1);
		inline const matrix<T>& operator*=(T factor);
		inline const matrix<T>& operator/=(T factor);
		inline void echo()const;
	private:
		size_t _n,_m;
		T **_mat;
};

template<typename T>
matrix<T>::matrix(){_n=0;_m=0;_mat=0;}

template<typename T>
matrix<T>::matrix(const vector<vector<T>> &a)
{
	_n=a.size(),_m=0;
	for(int i=0;i<_n;i++)_m=max(_m,a[i].size());
	_mat=new T*[_n];
	for(int i=0;i<_n;i++)_mat[i]=new T[_m];
	for(int i=0;i<_n;i++)
	for(int j=0;j<a[i].size();j++)_mat[i][j]=a[i][j]; 
}

template<typename T>
matrix<T>::matrix(const vec3<T> &a)
{
	_n=1;_m=3;
	_mat=new T*[_n];
	_mat[0]=new T[_m];
	for(int i=0;i<3;i++)_mat[0][i]=a[i];
}

template<typename T> 
matrix<T>::matrix(size_t n,size_t m,T **a):_n(n),_m(m),_mat(a){}

template<typename T> 
matrix<T>::matrix(size_t n,size_t m):_n(n),_m(m)
{
	_mat=new T*[_n];
	for(int i=0;i<_n;i++)_mat[i]=new T[_m];
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]=0; 
}

template<typename T>
matrix<T>::~matrix()
{
	for(int i=0;i<_n;i++)delete []_mat[i];
	delete []_mat;
}

template<typename T>
inline matrix<T> matrix<T>::transpose()const
{
	matrix<T>m(_m,_n);
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)m[j][i]=_mat[i][j]; 
	return m;
}

template<typename T>
inline matrix<T> matrix<T>::LU()const
{
	if(_n!=_m)throw "the size is error";
	// cout<<"1"<<endl;
	matrix<T>m(_n,_n);
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)m[i][j]=_mat[i][j]; 
	for(int i=0;i<_n;i++)
	{
		for(int j=i;j<_n;j++)
		for(int k=0;k<i;k++)m[i][j]-=m[i][k]*m[k][j];
		for(int j=i+1;j<_n;j++)
		for(int k=0;k<i;k++)m[j][i]-=m[j][k]*m[k][i];
		for(int j=i+1;j<_n;j++)m[j][i]/=m[i][i];
	}
	return m;
}

template<typename T>
inline matrix<T> matrix<T>::LU(matrix<T> &p)const
{
	if(_n!=_m)throw "the size is error";
	// cout<<"2"<<endl;
	matrix<T>m(_n,_n);
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)m[i][j]=_mat[i][j];
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)p[i][j]=i==j?1:0;
	for(int i=0;i<_n;i++)
	for(int j=i+1;j<_m;j++)if(m[i][i]<m[j][i])
	{
		T **u=m.mat()+i,**v=m.mat()+j;
		T *tmp1=m[i],*tmp2=m[j];
		*u=tmp2;*v=tmp1;
		u=p.mat()+i,v=p.mat()+j;
		tmp1=p[i],tmp2=p[j];
		*u=tmp2;*v=tmp1;
	}
	for(int i=0;i<_n;i++)
	{
		for(int j=i;j<_n;j++)
		for(int k=0;k<i;k++)m[i][j]-=m[i][k]*m[k][j];
		for(int j=i+1;j<_n;j++)
		for(int k=0;k<i;k++)m[j][i]-=m[j][k]*m[k][i];
		for(int j=i+1;j<_n;j++)m[j][i]/=m[i][i];
	}
	return m;
}
// cout<<i<<" "<<j<<endl;
// cout<<m[i]<<" "<<m[j]<<endl;
// cout<<*m[i]<<" "<<*m[j]<<endl;
// cout<<&tmp1<<" "<<&tmp2<<endl;
// cout<<m.mat()+i<<" "<<m.mat()+j<<endl;
// cout<<endl;

// cout<<m[i]<<" "<<m[j]<<endl;
// cout<<*u<<" "<<*v<<endl;
// cout<<u<<" "<<v<<endl;
// cout<<m.mat()+i<<" "<<m.mat()+j<<endl;
//swap(m[i],m[j]);
//swap(p[i],p[j]);

template<typename T>
inline matrix<T> matrix<T>::gauss(matrix<T> &p)const
{
	if(_n!=_m)throw "the size is error";
	matrix<T>m(_n,_n);
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)m[i][j]=_mat[i][j];
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)p[i][j]=i==j?1:0;
	for(int i=0;i<_n;i++)
	{
		for(int j=i+1;j<_n;j++)if(abs(m[i][i])<abs(m[j][i]))
		{
			T **u=m.mat()+i,**v=m.mat()+j;
			T *tmp1=m[i],*tmp2=m[j];
			*u=tmp2;*v=tmp1;
			u=p.mat()+i,v=p.mat()+j;
			tmp1=p[i],tmp2=p[j];
			*u=tmp2;*v=tmp1;
		}
		for(int j=i+1;j<_n;j++)
		{
			T tmp=m[j][i]/m[i][i];
			// cout<<tmp<<endl;
			for(int k=i;k<_n;k++)m[j][k]-=tmp*m[i][k];
		}
	}
	return m;
}

template<typename T>
inline void matrix<T>::echo()const
{
	cout<<"n,m:"<<_n<<" "<<_m<<endl;
	for(int i=0;i<_n;i++)
	{
		for(int j=0;j<_m;j++)cout<<_mat[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}

template<typename T>
inline T** matrix<T>::mat(){return _mat;}

template<typename T>
inline int matrix<T>::n()const{return _n;}

template<typename T>
inline int matrix<T>::m()const{return _m;}

template<typename T>
inline vec3<T> matrix<T>::to_vec()const
{
	if(_n!=1||_m!=3)throw "the size is error";
	return vec3<T>(_mat[0][0],_mat[0][1],_mat[0][2]);
}

template<typename T>
inline const T& matrix<T>::f(int x,int y)const{return _mat[x][y];}

template<typename T>
inline T* matrix<T>::operator[](const int x)const{return _mat[x];}

template<typename T>
inline const matrix<T> operator-(const matrix<T> &m1)
{
	matrix<T> m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=-m1[i][j];
	return m;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator=(const matrix<T> &m1)
{
	// if(m1.n()!=_n||m1.m()!=_m)throw "the size is error";
	for(int i=0;i<_n;i++)if(_mat[i])delete []_mat[i];
	if(_mat)delete []_mat;
	
	_n=m1.n();_m=m1.m();
	_mat=new T*[_n];
	for(int i=0;i<_n;i++)_mat[i]=new T[_m];
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]=m1[i][j];

	// for(int i=0;i<_n;i++)
	// for(int j=0;i<_m;j++)cout<<<<endl;
	return *this;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator+=(const matrix<T> &m1)
{
	if(m1.n()!=_n||m1.m()!=_m)throw "the size is error";
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]+=m1[i][j];
	return *this;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator-=(const matrix<T> &m1)
{
	if(m1.n()!=_n||m1.m()!=_m)throw "the size is error";
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]-=m1[i][j];
	return *this;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator*=(T factor)
{
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]*=factor;
	return *this;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator/=(T factor)
{
	for(int i=0;i<_n;i++)
	for(int j=0;j<_m;j++)_mat[i][j]/=factor;
	return *this;
}

template<typename T>
inline const matrix<T>& matrix<T>::operator*=(const matrix<T> &m1)
{
	if(_m!=m1.n())throw "the size is error";
	for(int k=0;k<_m;k++)
	for(int i=0;i<_n;i++)
	for(int j=0;j<m1.m();j++)_mat[i][j]+=_mat[i][k]*m1[k][j];
	return *this;
}

template<typename T>
inline bool operator==(const matrix<T> &m1,matrix<T> &m2)
{
	if(m1.n()!=m2.n()||m1.m()!=m2.m())return 0;
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)if(m1[i][j]!=m2[i][j])return 0;
	return 1;
}

template<typename T>
inline matrix<T> operator+(const matrix<T> &m1,const matrix<T> &m2)
{
	if(m1.n()!=m2.n()||m1.m()!=m2.m())throw "the size is error";
	matrix<T> m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=m1[i][j]+m2[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator-(const matrix<T> &m1,const matrix<T> &m2)
{
	if(m1.n()!=m2.n()||m1.m()!=m2.m())throw "the size is error";
	matrix<T> m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=m1[i][j]-m2[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator*(T factor,const matrix<T> &m1)
{
	matrix<T>m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=factor*m1[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator*(const matrix<T> &m1,T factor)
{
	matrix<T>m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=factor*m1[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator/(T factor,const matrix<T> &m1)
{
	matrix<T>m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=factor/m1[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator/(const matrix<T> &m1,T factor)
{
	matrix<T>m(m1.n(),m1.m());
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m1.m();j++)m[i][j]=factor/m1[i][j];
	return m;
}

template<typename T>
inline matrix<T> operator*(const matrix<T> &m1,const matrix<T> &m2)
{
	if(m1.m()!=m2.n())throw "the size is error";
	matrix<T>m(m1.n(),m2.m());
	// m.echo();
	// m1.echo();
	// m2.echo();
	for(int k=0;k<m1.m();k++)
	for(int i=0;i<m1.n();i++)
	for(int j=0;j<m2.m();j++)m[i][j]+=m1[i][k]*m2[k][j];
	// m.echo();
	return m;
}

template<typename T>
inline matrix<T> solve(const matrix<T> &LU,const matrix<T> &b)
{
	if(LU.n()!=LU.m()||LU.n()!=b.n()||b.m()!=1)throw "the size is error";
	matrix<T> tmp(b.n(),1);
	tmp=b;
	for(int i=0;i<b.n();i++)
	for(int j=0;j<i;j++)tmp[i][0]-=tmp[j][0]*LU[i][j];
	for(int i=b.n()-1;~i;i--)
	{
		for(int j=i+1;j<b.n();j++)tmp[i][0]-=tmp[j][0]*LU[i][j];
		tmp[i][0]/=LU[i][i];
	}
	return tmp;
}