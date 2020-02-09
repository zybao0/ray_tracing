#include"../ray.h"
class lambertain:public material
{
	public:
		lambertain(texture *albedo);
		lambertain(texture *albedo,texture *emit,value_type intensity);
		virtual bool scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered)const override;
};