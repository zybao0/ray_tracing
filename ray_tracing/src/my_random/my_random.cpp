#include<ctime>
#include<random>
#include<my_random.h>
using namespace std;
const value_type esp=1e-9;
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
	while(dot(p,p)+esp>=1.)p=2.*vec_type(random_unit_figure(),random_unit_figure(),0.)-vec_type(1.,1.,0.);
	return p;
}
const vec_type random_unit_sphere()
{
	vec_type p=2.*vec_type(random_unit_figure(),random_unit_figure(),random_unit_figure())-vec_type(1.,1.,1.);
	while(dot(p,p)+esp>=1.)p=2.*vec_type(random_unit_figure(),random_unit_figure(),random_unit_figure())-vec_type(1.,1.,1.);
	return p;
}