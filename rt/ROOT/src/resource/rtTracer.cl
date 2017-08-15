RT_Vec3f Shade(const RT_Material *m, const RT_Light *l, const RT_Result *r)
{
	RT_Vec3f color;
	return color;
}

RT_Vec3f AreaLightShade(const RT_Material *m, const RT_Light *l, const RT_Result *r)
{
	RT_Vec3f color;
	return color;
}

RT_Vec3f TraceRay(__constant RT_Sphere *spheres, const int numObj, const RT_Ray *ray)
{
	RT_Result r = Hit(spheres, numObj, ray);
	return r.hit? r.color : (RT_Vec3f)(0.0f);
}