#include"type_vec.h"
#pragma once
namespace gm
{
	template<typename T>class vec3
	{
	public:
		using value_type=T;
		using norm_t=precision;
	public:
			template<typename E>
			vec3(const vec3<E>& vec);	  //static_cast
			vec3(const T e1 = T(), const T e2 = T(), const T e3 = T())noexcept;
			explicit vec3(const vec2<T>& v2, const T e3 = T())noexcept;
			explicit vec3(const T e1, const vec2<T>& v)noexcept;
			explicit vec3(const vec3&);
			~vec3() {  }
		public:
			inline T& x(){ return _x; }
			inline T& y(){ return _y; }
			inline T& z(){ return _z; }
			inline T& r(){ return _x; }
			inline T& g(){ return _y; }
			inline T& b(){ return _z; }
			inline vec2<T> xy() { return vec2<T>{_x, _y}; }
			inline vec2<T> yx() { return vec2<T>{_y, _x}; }
			inline vec2<T> xz() { return vec2<T>{_x, _z}; }
			inline vec2<T> zx() { return vec2<T>{_z, _x}; }
			inline vec2<T> yz() { return vec2<T>{_y, _z}; }
			inline vec2<T> zy() { return vec2<T>{_z, _y}; }
			inline vec2<T> rg() { return vec2<T>{_x, _y}; }
			inline vec2<T> gr() { return vec2<T>{_y, _x}; }
			inline vec2<T> rb() { return vec2<T>{_x, _z}; }
			inline vec2<T> br() { return vec2<T>{_z, _x}; }
			inline vec2<T> gb() { return vec2<T>{_y, _z}; }
			inline vec2<T> bg() { return vec2<T>{_z, _y}; }
			inline vec3 rgb()	{ return vec3{_x, _y, _z}; }
			inline vec3 rbg()	{ return vec3{_x, _z, _y}; }	
			inline vec3 gbr()	{ return vec3{_y, _z, _x}; }
			inline vec3 grb()	{ return vec3{_y, _x, _z}; }
			inline vec3 bgr()	{ return vec3{_z, _y, _x}; }
			inline vec3 brg()	{ return vec3{_z, _x, _y}; }
			inline const T& x()const	{ return _x; }
			inline const T& y()const	{ return _y; }
			inline const T& z()const	{ return _z; }
			inline const T& r()const	{ return _x; }
			inline const T& g()const	{ return _y; }
			inline const T& b()const	{ return _z; }
			//inline oprator function
			inline const vec3& operator+() const;
			inline vec3 operator-()const;
			inline vec3& operator++();
			inline vec3& operator--();
			inline const vec3 operator++(int);
			inline const vec3 operator--(int);
			inline const T& operator[](const int index)const;
			inline T& operator[](const int index);
			inline vec3& operator=(const vec3& rhs);
			inline vec3& operator+=(const vec3& rhs);
			inline vec3& operator-=(const vec3& rhs);
			inline vec3& operator*=(const vec3& rhs);
			inline vec3& operator/=(const vec3& rhs);
			inline vec3& operator*=(const T factor);
			inline vec3& operator/=(const T factor);		  
		public:
			//return the Norm of vec3
			inline norm_t normal()const;
			inline norm_t squar()const;
			//let self become to the unit vector of vec_type
			inline void self_unitization();
			//return a non-integer three-dimensional unit vector [the type is norm_t]
			inline vec3<precision> ret_unitization()const;
			inline bool isnull()const;
		private:
			T _x, _y, _z;
		};

//constructor functions

	template<typename T>
	template<typename E>
	vec3<T>::vec3(const vec3<E>& vec):_x(static_cast<T>(vec.x())),_y(static_cast<T>(vec.y())),_z(static_cast<T>(vec.z())){}

	template<typename T>
	vec3<T>::vec3(const T e1, const T e2, const T e3)noexcept:_x{e1},_y{e2},_z{e3}{}

	template<typename T>
	vec3<T>::vec3(const vec2<T>& v, const T e3)noexcept:_x(v.x()),_y(v.y()),_z(e3){}

	template<typename T>
	vec3<T>::vec3(const T e, const vec2<T>& v)noexcept:_x(e),_y(v.x()),_z(v.y()){}

	template<typename T>
	vec3<T>::vec3(const vec3<T>& rhs):_x{rhs._x},_y{rhs._y},_z{rhs._z}{}

	// Binary operator functions [non-mem]
	template<typename T>
	vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2)
	{
	//the operator of + ,example  (5,4,3) + (2,-2,1) -> (7,2,4)  
		return vec3<T>(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
	}

	template<typename T>
	inline vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2)
	{
	//the operator of - ,example  (5,4,3) - (2,2,1) -> (3,2,2)  
		return v1 + (-v2);
	}

	template<typename A, typename B>
	inline auto operator*(const vec3<A>& v1, const vec3<B>& v2)
	{
	//the operator of * ,example  (1.1, 2.1, 3.1) * (2, 3, 4) -> (2.2, 6.3, 12.4)
		using type = decltype(v1[0] * v2[0]);
		return vec3<type>((type)v1[0] * v2[0], (type)v1[1] * v2[1], (type)v1[2] * v2[2]);
	}

	template<typename T>
	inline vec3<T> operator*(const vec3<T>& v1, const vec3<T>& v2)
	{
	//the operator of * ,example 3 * 5 -> 15 , (1,2,3) * (2,3,4) -> (2,6,12)
		return vec3<T>(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
	}

	template<typename T, typename E>
	inline vec3<T> operator*(const vec3<T>& v, const E factor)
	{
		return vec3<T>(v.x() * factor, v.y() * factor, v.z() * factor);
	}

	template<typename T, typename E>
	inline vec3<T> operator*(const E factor, const vec3<T>& v)
	{
		return vec3<T>(v.x() * factor, v.y() * factor, v.z() * factor);
	}

	template<typename A, typename B>
	inline auto operator/(const vec3<A>& v1, const vec3<B>& v2)
	{
	//the operator of / ,example  (1.1, 2.1, 3.2) * (2, 3, 4) -> (0.55, 0.7, 0.8)
		assert(v2.isnull());
		using type = decltype(v1[0] / v2[0]);
		return vec3<type>((type)v1[0] / v2[0], (type)v1[1] / v2[1], (type)v1[2] / v2[2]);
	}

	template<typename T>
	inline vec3<T> operator/(const vec3<T>& v1, const vec3<T>& v2)
	{
	//the operator of / ,example 3 * 5 -> 15 , (1,2,3) * (2,3,4) -> (1/2,2/3,3/4)
		assert(v2.isnull());
		return operator/<T, T> (v1, v2);
	}

	template<typename T, typename E>
	inline vec3<T> operator/(const vec3<T>& v, const E factor)
	{
		assert(factor != 0 && factor != 0.);
		return vec3<T>(v.x() / factor, v.y() / factor, v.z() / factor);
	}

	template<typename T>
	inline bool operator==(const vec3<T>& v1, const vec3<T>& v2)
	{
		return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
	}

	template<typename T>
	inline bool operator!=(const vec3<T>& v1, vec3<T>& v2)
	{
		return !(v1 == v2);
	}

	// Unary operator functions [mem]
	template<typename T>
	inline const vec3<T>& vec3<T>::operator+() const 
	{
	//the operator of + ,example 5 -> +5,  +(1,-2,3) -> (1,-2,3)
		return *this; 
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator-() const
	{
	//the operator of - ,example 5 -> -5,  -(1,-2,3) -> (-1,2,-3)
		return vec3<T>(-_x, -_y, -_z);
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator++()
	{
		++this->_x;
		++this->_y;
		++this->_z;
		return *this;
	}

	template<typename T>
	inline const vec3<T> vec3<T>::operator++(int)
	{
		vec3<T>ori(*this);
		++*this;
		return ori;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator--()
	{
		--this->_x;
		--this->_y;
		--this->_z;
		return *this;
	}

	template<typename T>
	inline const vec3<T> vec3<T>::operator--(int)
	{
		vec3<T>ori(*this);
		--*this;
		return ori;
	}

	template<typename T>
	inline const T& vec3<T>::operator[](const int index)const
	{
		if (index == 0)return _x;
		else if (index == 1)return _y;
		else if (index == 2)return _z;
		else throw "the index is error";
	}

	template<typename T>
	inline T& vec3<T>::operator[](const int index)
	{
		if (index == 0)return _x;
		else if (index == 1)return _y;
		else if (index == 2)return _z;
		else throw "the index is error";
	}

	// member functions
	template<typename T>
	inline vec3<T>& vec3<T>::operator=(const vec3<T>& rhs)
	{
		if (this != &rhs)
		{
			_x = rhs._x;
			_y = rhs._y;
			_z = rhs._z;
		}
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator+=(const vec3& rhs)
	{
		this->_x += rhs._x;
		this->_y += rhs._y;
		this->_z += rhs._z;
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator-=(const vec3& rhs)
	{
		this->_x -= rhs._x;
		this->_y -= rhs._y;
		this->_z -= rhs._z;
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator/=(const vec3<T>& rhs)
	{
		assert(!rhs.isnull());
		this->_x /= rhs._x;
		this->_y /= rhs._y;
		this->_z /= rhs._z;
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator*=(const vec3<T>& rhs)
	{
		this->_x *= rhs._x;
		this->_y *= rhs._y;
		this->_z *= rhs._z;
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator*=(const T factor)
	{
		this->_x *= factor;
		this->_y *= factor;
		this->_z *= factor;
		return *this;
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator/=(const T factor)
	{
		assert(factor != 0);
		this->_x /= factor;
		this->_y /= factor;
		this->_z /= factor;
		return *this;
	}

	template<typename T>
	inline typename vec3<T>::norm_t vec3<T>::normal()const
	{
		return sqrt(squar());
	}

	template<typename T>
	inline typename vec3<T>::norm_t vec3<T>::squar()const
	{
		return _x*_x + _y*_y + _z*_z;
	}

	template<typename T>
	inline void vec3<T>::self_unitization()
	{
		*this /= normal();
	}

	template<typename T>
	inline vec3<precision> vec3<T>::ret_unitization()const
	{
		norm_t div = normal();
		return vec3<norm_t>{ (norm_t)this->_x / div,(norm_t)this->_y / div,(norm_t)this->_z / div };
	}

	template<typename T, typename E>
	inline auto dot(const vec3<T>& v1, const vec3<E>& v2) //-> decltype(v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z())
	{// x1 * x2 + y1 * y2 + z1 * z2
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
	}

	template<typename T, typename E>
	inline auto cross(const vec3<T> v1, const vec3<E>& v2)
	{// v1 × v2
		return vec3<decltype(v1[1] * v2[2] - v1[2] * v2[1])>(v1[1] * v2[2] - v1[2] * v2[1],v1[2] * v2[0] - v1[0] * v2[2],v1[0] * v2[1] - v1[1] * v2[0]);
	}

	template<typename T>
	inline bool vec3<T>::isnull()const
	{
		return *this == vec3<T>();
	}

}	//namespace lvgm

