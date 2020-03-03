#include<material/lambertain.h>
lambertain::lambertain(texture *albedo,pdf* p)
{
	_pdf=p;
	_albedo=albedo;
	_emit=0;
	_intensity=0;
}
lambertain::lambertain(texture *albedo,texture *emit,value_type intensity,pdf* p)
{
	_pdf=p;
	_albedo=albedo;
	if(emit)_emit=emit;
	else _emit=albedo;
	_intensity=intensity;
}
bool lambertain::scatter(const ray &InRay,const hitInfo &info,vec_type &attenuation,vec_type &emit,ray &scattered,value_type &pdf_value)const
{
	bool fg=(dot(info._n,InRay.direction())>0);//一般来讲，光线不会再漫反射表面穿过物体，若光线与法向量同向，则翻转法向量
	vec_type target=_pdf->generate(InRay.origin()),w=fg?-info._n:info._n.ret_unitization(),u,v;
	pdf_value=_pdf->value(InRay.origin(),target)*(2*pi);
	 // mat_type(target).echo();
	if(w==vec_type(0,0,-1))target=-target;
	else if(!(w==vec_type(0,0,1)))
	{
		u=cross(vec_type(0,0,1),w);
		v=cross(w,u);
		// mat_type(vector<vector<value_type>>{vector<value_type>{u.x(),v.x(),w.x()},vector<value_type>{u.y(),v.y(),w.y()},vector<value_type>{u.z(),v.z(),w.z()}}).echo();
		target=(mat_type(vector<vector<value_type>>{vector<value_type>{u.x(),v.x(),w.x()},vector<value_type>{u.y(),v.y(),w.y()},vector<value_type>{u.z(),v.z(),w.z()}})*(mat_type(target).transpose())).transpose().to_vec();
		// mat_type(target).echo();
	}
	scattered=ray(info._p,target,InRay.time());
	
	// vec_type target=random_unit_sphere().ret_unitization();
	// while(dot(target,fg?-info._n:info._n)<0)target=random_unit_sphere().ret_unitization();
	// scattered=ray(info._p,target,InRay.time());
	// pdf_value=1;

	// vec_type target=info._p+(fg?-info._n:info._n)+random_unit_sphere();
	// scattered=ray(info._p,target-info._p,InRay.time());
	// pdf_value=1;
	
	attenuation=_albedo->value(info._u,info._v,info._p)*(1-_intensity)*dot(target,fg?-info._n:info._n);
	if(_emit)emit=_intensity*_emit->value(info._u,info._v,info._p);
	return 1;
}