#include"type_vec.h"
#pragma once
namespace gm
{
	template<typename T>class vec2
	{
		public:
			using value_type=T;
			using norm_t=precision;
		public:
			template<typename E>
			vec2(const vec2<E>&vec);		//static_cast
			vec2(const T x=T(),const T y=T())noexcept;
			vec2(const vec2&);
			~vec2(){}
		public:
			//inline get function
			inline T& x(){return _x;}
			inline T& y(){return _y;}
			inline T& u(){return _x;}
			inline T& v(){return _y;}
			inline T& r(){return _x;}
			inline T& g(){return _y;}
			inline T& s(){return _x;}
			inline T& t(){return _y;}
			inline vec2 xy(){return vec2<T>{_x, _y};}
			inline vec2 yx(){return vec2<T>{_y, _x};}
			inline vec2 rg(){return vec2<T>{_x, _y};}
			inline vec2 gr(){return vec2<T>{_y, _x};}
			inline vec2 uv(){return vec2<T>{_x, _y};}
			inline vec2 vu(){return vec2<T>{_y, _x};}
			inline vec2 st(){return vec2<T>{_x, _y};}
			inline vec2 ts(){return vec2<T>{_y, _x};}
			inline const T& x()const{return _x;}
			inline const T& y()const{return _y;}
			inline const T& u()const{return _x;}
			inline const T& v()const{return _y;}
			inline const T& r()const{return _x;}
			inline const T& g()const{return _y;}
			inline const T& s()const{return _x;}
			inline const T& t()const{return _y;}
			//inline operator function
			inline const vec2& operator+()const;
			inline vec2 operator-()const;
			inline vec2& operator++();
			inline vec2& operator--();
			inline const vec2 operator++(int);
			inline const vec2 operator--(int);
			inline const T& operator[](const int index)const;
			inline T& operator[](const int index);
			inline vec2& operator=(const vec2& rhs);
			inline vec2& operator+=(const vec2& rhs);
			inline vec2& operator-=(const vec2& rhs);
			inline vec2& operator*=(const vec2& rhs);
			inline vec2& operator/=(const vec2& rhs);
			inline vec2& operator*=(const T factor);
			inline vec2& operator/=(const T factor);

		public:
			//return the Norm of vec2
			inline norm_t normal()const;
			inline norm_t squar()const;
			//let self become to the unit vector of vec_type
			inline void self_unitization();
			//return a non-integer three-dimensional unit vector [the type is norm_t]
			inline vec2<precision> ret_unitization()const;
			inline bool isnull()const;
		private:
			T _x, _y;
	};


//constructor functions

	template<typename T>
	vec2<T>::vec2(const T x,const T y)noexcept:_x(x),_y(y){}

	template<typename T>
	template<typename E>
	vec2<T>::vec2(const vec2<E>& rhs):_x(static_cast<T>(rhs.x())),_y(static_cast<T>(rhs.y())){}

	template<typename T>
	vec2<T>::vec2(const vec2<T>& rhs):_x(rhs._x),_y(rhs._y){}

	// Binary operator functions [non-mem]

	template<typename T>
	inline vec2<T> operator+(const vec2<T>& v1, const vec2<T>& v2)
	{
		return vec2<T>(v1[0] + v2[0], v1[1] + v2[1]);
	}

	template<typename T>
	inline vec2<T> operator-(const vec2<T>& v1, const vec2<T>& v2)
	{
	//the operator of - ,example  (5,4) - (2,2) -> (3,2)  
		return v1 + (-v2);
	}

	template<typename A, typename B>
	inline auto operator*(const vec2<A>& v1, const vec2<B>& v2)
	{
	//the operator of * ,example  (1.1, 2.1) * (2, 3) -> (2.2, 6.3)
		using type = decltype(v1[0] * v2[0]);
		return vec2<type>((type)v1[0] * v2[0], (type)v1[1] * v2[1]);
	}

	template<typename T>
	inline vec2<T> operator*(const vec2<T>& v1, const vec2<T>& v2)
	{
	//the operator of * ,example (1,2) * (2,3) -> (2,6)
		return vec2<T>(v1[0] * v2[0], v1[1] * v2[1]);
	}

	template<typename T, typename E>
	inline vec2<T> operator*(const vec2<T>& v, const E factor)
	{
		return vec2<T>(v.x() * factor, v.y() * factor);
	}

	template<typename T, typename E>
	inline vec2<T> operator*(const E factor, const vec2<T>& v)
	{
		return vec2<T>(v.x() * factor, v.y() * factor);
	}

	template<typename A, typename B>
	inline auto operator/(const vec2<A>& v1, const vec2<B>& v2)
	{
		//the operator of / ,example  (1.1, 2.1) * (2, 3) -> (0.55, 0.7)
		assert(v2.isnull());
		using type = decltype(v1[0] / v2[0]);
		return vec2<type>((type)v1[0] / v2[0], (type)v1[1] / v2[1]);
	}

	template<typename T>
	inline vec2<T> operator/(const vec2<T>& v1, const vec2<T>& v2)
	{
	//the operator of / ,example 3 * 5 -> 15 , (1,2) * (2,3) -> (1/2,2/3)
		assert(v2.isnull());
		return operator/<T, T> (v1, v2);
	}

	template<typename T, typename E>
	inline vec2<T> operator/(const vec2<T>& v, const E factor)
	{
		assert(factor != 0 && factor != 0.);
		return vec2<T>(v.x() / factor, v.y() / factor);
	}

	template<typename T>
	inline bool operator==(const vec2<T>& v1, const vec2<T>& v2)
	{
		return v1.x() == v2.x() && v1.y() == v2.y();
	}

	template<typename T>
	inline bool operator!=(const vec2<T>& v1, const vec2<T>& v2)
	{
		return !(v1 == v2);
	}

	// Unary operator functions [mem]

	template<typename T>
	inline const vec2<T>& vec2<T>::operator+() const
	{
	//the operator of + ,example 5 -> +5,  +(1,-2) -> (1,-2)
		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator-() const
	{
	//the operator of - ,example 5 -> -5,  -(1,-2) -> (-1,2)
		return vec2<T>(-_x, -_y);
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator++()
	{
		++this->_x;
		++this->_y;
		return *this;
	}

	template<typename T>
	inline const vec2<T> vec2<T>::operator++(int)
	{
		vec2<T>ori(*this);
		++*this;
		return ori;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator--()
	{
		--this->_x;
		--this->_y;
		return *this;
	}

	template<typename T>
	inline const vec2<T> vec2<T>::operator--(int)
	{
		vec2<T>ori(*this);
		--*this;
		return ori;
	}

	template<typename T>
	inline const T& vec2<T>::operator[](const int index)const
	{
		if (index == 0)return _x;
		else if (index == 1)return _y;
		else throw "the index is error";
	}

	template<typename T>
	inline T& vec2<T>::operator[](const int index)
	{
		if (index == 0)return _x;
		else if (index == 1)return _y;
		else throw "the index is error";
	}

	// member functions

	template<typename T>
	inline vec2<T>& vec2<T>::operator=(const vec2<T>& rhs)
	{
		if (this != &rhs)
		{
			_x = rhs._x;
			_y = rhs._y;
		}
		return *this;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator+=(const vec2& rhs)
	{
		this->_x += rhs._x;
		this->_y += rhs._y;
		return *this;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator-=(const vec2& rhs)
	{
		return *this += (-rhs);
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator/=(const vec2<T>& rhs)
	{
		assert(!rhs.isnull());
		this->_x /= rhs._x;
		this->_y /= rhs._y;
		return *this;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator*=(const vec2<T>& rhs)
	{
		this->_x *= rhs._x;
		this->_y *= rhs._y;
		return *this;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator*=(const T factor)
	{
		this->_x *= factor;
		this->_y *= factor;
		return *this;
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator/=(const T factor)
	{
		assert(factor != 0);
		this->_x /= factor;
		this->_y /= factor;
		return *this;
	}


	template<typename T>
	inline typename vec2<T>::norm_t vec2<T>::normal()const
	{
		return sqrt(squar());
	}

	template<typename T>
	inline typename vec2<T>::norm_t vec2<T>::squar()const
	{
		return _x*_x + _y*_y;
	}

	template<typename T>
	inline void vec2<T>::self_unitization()
	{
		*this /= normal();
	}

	template<typename T>
	inline vec2<precision> vec2<T>::ret_unitization()const
	{
		norm_t div = normal();
		//return vec2<norm_t>{ (norm_t)this->_x / div, (norm_t)this->_y / div, (norm_t)this->_z / div };
		return vec2<norm_t>((norm_t)this->_x / div, (norm_t)this->_y / div);//change by Eabo
	}

	template<typename T, typename E>
	inline auto dot(const vec2<T>& v1, const vec2<E>& v2) //-> decltype(v1.x() * v2.x() + v1.y() * v2.y()
	{// x1 * x2 + y1 * y2
		return v1.x() * v2.x() + v1.y() * v2.y();
	}

	template<typename T, typename E>
	inline auto cross(const vec2<T> v1, const vec2<E>& v2)
	{// v1 × v2
		return vec2<decltype(v1[1] * v2[2] - v1[2] * v2[1])>(v1[0] * v2[1] - v1[1] * v2[0]);
	}

	template<typename T>
	inline bool vec2<T>::isnull()const
	{
		return *this == vec2<T>();
	}
}	//namespace lvgm
//^{[\s\S]+?}$