#include<pdf/constant_pdf.h>
using namespace std;
// cosine_pdf::cosine_pdf(const vec_type &t):trans(3,3)
// {
// 	vec_type w=t.ret_unitization(),u=cross(vec_type(0,0,1),w),v=cross(vec_type(0,0,1),u);
// 	trans=mat_type(vector<vector<value_type>>{vector<value_type>{u.x(),v.x(),w.x()},vector<value_type>{u.y(),v.y(),w.y()},vector<value_type>{u.z(),v.z(),w.z()}});
// }
value_type constant_pdf::value(const vec_type &origin,const vec_type &direction)const{return 0.5*(1/pi);}
vec_type constant_pdf::generate(const vec_type &origin)const
{
	// value_type r1=random_unit_figure(),r2=random_unit_figure(),ph=2*pi*r1;
	// // return (trans*(mat_type(vec_type(2.*cos(ph)*sqrt(r2),2*sin(ph)*sqrt(r2),sqrt(1-r2))).transpose())).transpose().to_vec();
	// return vec_type(cos(ph)*sqrt(2*r2-r2*r2),sin(ph)*sqrt(2*r2-r2*r2),1-r2);
	value_type u=2*random_unit_figure()-1.0,v=2*random_unit_figure()-1.0,r=u*u+v*v;
	while(r>=1||1-2*r<0)u=2*random_unit_figure()-1.0,v=2*random_unit_figure()-1.0,r=u*u+v*v;
	return vec_type(2*u*sqrt(1-r),2*v*sqrt(1-r),1-2*r);
}