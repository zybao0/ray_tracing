#include<pdf/light_pdf.h>
using namespace std;
// cosine_pdf::cosine_pdf(const vec_type &t):trans(3,3)
// {
// 	vec_type w=t.ret_unitization(),u=cross(vec_type(0,0,1),w),v=cross(vec_type(0,0,1),u);
// 	trans=mat_type(vector<vector<value_type>>{vector<value_type>{u.x(),v.x(),w.x()},vector<value_type>{u.y(),v.y(),w.y()},vector<value_type>{u.z(),v.z(),w.z()}});
// }
light_pdf::light_pdf(intersect *item,intersect *world):_item(item),_world(world){}
value_type light_pdf::value(const vec_type &origin,const vec_type &direction)const
{
	// vec_type u(trans[0][2],trans[1][2],trans[2][2]);
	// value_type cosine=dot(direction.ret_unitization(),u);
	int succeed_hit=0;
	for(int i=0;i<monte_carlo_rand_times;i++)
	{

	}
}
vec_type light_pdf::generate(const vec_type &origin)const
{
	vec_type direction;
	hitInfo rec1,rec2;
	for(int i=0;i<monte_carlo_rand_times;i++)
	{
		direction=(_item->random_poin()-origin).ret_unitization();
		ray sight(origin,direction);
		if(!_item->hit(sight,0.,INF,rec1))continue;
		if(!_world->hit(sight,0.,INF,rec2))continue;
		if(rec2._t<rec1._t+eps&&rec2._t>rec1._t-eps)break;
	}
	return direction;
}