#include<pdf/cosine_pdf.h>
using namespace std;
// cosine_pdf::cosine_pdf(const vec_type &t):trans(3,3)
// {
// 	vec_type w=t.ret_unitization(),u=cross(vec_type(0,0,1),w),v=cross(vec_type(0,0,1),u);
// 	trans=mat_type(vector<vector<value_type>>{vector<value_type>{u.x(),v.x(),w.x()},vector<value_type>{u.y(),v.y(),w.y()},vector<value_type>{u.z(),v.z(),w.z()}});
// }
value_type cosine_pdf::value(const vec_type &origin,const vec_type &direction)const
{
	// vec_type u(trans[0][2],trans[1][2],trans[2][2]);
	// value_type cosine=dot(direction.ret_unitization(),u);
	value_type cosine=dot(direction.ret_unitization(),vec_type(0,0,1));
	return cosine>0?cosine/pi:0;
}
vec_type cosine_pdf::generate(const vec_type &origin)const
{
	value_type r1=random_unit_figure(),r2=random_unit_figure(),ph=2*pi*r1;
	// return (trans*(mat_type(vec_type(2.*cos(ph)*sqrt(r2),2*sin(ph)*sqrt(r2),sqrt(1-r2))).transpose())).transpose().to_vec();
	return vec_type(cos(ph)*sqrt(r2),sin(ph)*sqrt(r2),sqrt(1-r2));
}