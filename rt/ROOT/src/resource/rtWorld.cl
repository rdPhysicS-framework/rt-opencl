RT_Result Hit(__constant RT_Sphere *spheres, const int numObj, const RT_Ray *ray)
{
	RT_Result r = CreateResult();
	RT_Vec3f normal;
	RT_Vec3f hp;

	float tmin = INFINITE;
	float t;
	for(int i = 0; i < numObj; i++)
	{
		RT_Sphere s = spheres[i];
		if(Sphere_Hit(&s, ray, &t, &r) && t < tmin)
		{
			tmin = t;
			r.hit = true;
			r.material = s.material;
			normal = r.normal;
			hp = r.lhitPoint;
		}	
	}
	if(r.hit)
	{ 
		r.normal = normal;
		r.lhitPoint = hp;
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

RT_Vec3f TraceRay(__constant RT_Sphere *spheres, 
				  const int numObj, 
				  const RT_Ray *ray)
{
	RT_Result r = Hit(spheres, numObj, ray);
	return r.hit? r.material.color : (RT_Vec3f)(0.0f);
}

