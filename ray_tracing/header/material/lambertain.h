#include"../ray.h"
class lambertain:public material
{
	public:
		lambertain(texture *albedo,pdf* p);
		lambertain(texture *albedo,texture *emit,value_type intensity,pdf* p);
		virtual bool scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered,value_type &pdf_value)const override;
};