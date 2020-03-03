#include"../ray.h"
class dielectric:public material
{
	public:
		dielectric(value_type RI,texture *albedo);
		dielectric(value_type RI,texture *albedo,texture *emit,value_type intensity);
		virtual bool scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered,value_type &pdf_value)const override;
	private:
		value_type _RI;
};