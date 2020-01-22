#pragma once

#include <cmath>
#include <cassert>
#include <iostream>
namespace gm
{
    #ifdef HIGHPRECISION            //set the high precision 
    using precision=long double;
    #elif(defined LOWPRECISION)    //set the low preciion
    using precision=float;
    #else
    using precision=double;        //default precision
    #endif                        //set precision

    template<typename T>class vec2;
    template<typename T>class vec3;
    template<typename T>class vec4;

    typedef vec2<bool>bvec2;
    typedef vec2<char>cvec2;
    typedef vec2<short>svec2;
    typedef vec2<int>ivec2;
    typedef vec2<float>fvec2;
    typedef vec2<double>dvec2;
    typedef vec2<long double>ldvec2;

    typedef vec3<bool>bvec3;
    typedef vec3<char>cvec3;
    typedef vec3<short>svec3;
    typedef vec3<int>ivec3;
    typedef vec3<float>fvec3;
    typedef vec3<double>dvec3;
    typedef vec3<long double>ldvec3;

    typedef vec4<bool>bvec4;
    typedef vec4<char>cvec4;
    typedef vec4<short>svec4;
    typedef vec4<int>ivec4;
    typedef vec4<float>fvec4;
    typedef vec4<double>dvec4;
    typedef vec4<long double>ldvec4;
}