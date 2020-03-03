#include"../ray.h"
class metal:public material
{
	public:
		metal(texture *albedo,const value_type f=0.);
		metal(texture *albedo,texture *emit,value_type intensity,const value_type f=0.);
		virtual bool scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered,value_type &pdf_value)const override;
	protected:
		value_type _fuzz;
};