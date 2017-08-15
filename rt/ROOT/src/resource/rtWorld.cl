typedef struct
{
	int width;
	int height;
	RT_Vec2f sp;
	RT_SScoord coord;
} RT_ViewPlane;

RT_Result Hit(__constant RT_Sphere *spheres, const int numObj, const RT_Ray *ray)
{
	RT_Result r = CreateResult();

	float tmin = INFINITE;
	float t;

	for(int i = 0; i < numObj; i++)
	{
		RT_Sphere s = spheres[i];
		if(Sphere_Hit(&s, ray, &t, &r) && t < tmin)
		{
			tmin = t;
			r.hit = true;
		}	
	}
	if(r.hit)
	{ 
		float cosfactor = dot(r.normal, ray->d) * -1;
		r.color = (RT_Vec3f)(0.5f, 0.7f, 0.8f) * cosfactor;
	}

	return r;
}

bool ShadowHit(__constant RT_Sphere *spheres, const int numObj, RT_Ray *ray, float tmin)
{
	float t;
	for(int i = 0; i < numObj; i++)
	{
		RT_Sphere s = spheres[i];
		if(Sphere_ShadowHit(&s, ray, &t) && t < tmin)
		{
			return true;
		}	
	}

	return false;
}

RT_Vec3f TraceRay(__constant RT_Sphere *spheres, const int numObj, const RT_Ray *ray)
{
	RT_Result r = Hit(spheres, numObj, ray);
	return r.hit? r.color : (RT_Vec3f)(0.0f);
}

