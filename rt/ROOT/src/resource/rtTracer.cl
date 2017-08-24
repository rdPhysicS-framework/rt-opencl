RT_Vec3f Shade(__constant const RT_Light *lights,
			   const RT_Ray *ray,
			   const RT_Result *r,
			   const int numLights)
{
	RT_Vec3f color = (r->material.color * r->material.ambient);// * (RT_Vec3f)(1.5f);

	for(int i = 0; i < 2; i++)
	{
		const RT_Light l = lights[i];
		RT_Vec3f wi = /*(RT_Vec3f)(0.0f, -1.0f, 0.0f);*/normalize(l.position - r->lhitPoint);//Direction(&l, r);
		
		float nDotWi = dot(r->normal, wi);
		if (nDotWi > 0.0f)
		{
			/*inShadow*/

			RT_Vec3f diffuse = r->material.color * (r->material.diffuse * INVPI);

			RT_Vec3f refl = reflect(&wi, &r->normal);
			float nDotWo = dot(refl, ray->d);
			RT_Vec3f specular = (RT_Vec3f)(0.0f);

			if(nDotWo > 0.0f)
			{ 
				specular = r->material.color * (r->material.specular * pow(nDotWo, r->material.intensity));
			}
			
			color += (diffuse + specular) * nDotWi * (RT_Vec3f)(0.6f, 0.6f, 0.7f);
		}
	}

	return color;
}

/*RT_Vec3f AreaLightShade(const RT_Material *m, const RT_Light *l, const RT_Result *r)
{
	RT_Vec3f color;
	return color;
}*/

RT_Vec3f _TraceRay(__constant const RT_Sphere *spheres, 
				   __constant const RT_Light *lights,
				   const RT_Ray *ray,
				   const int numLights, 
				   const int numObj)
{
	RT_Result r = Hit(spheres, numObj, ray);

	if(r.hit)
	{
		r.ray = *ray;
		return Shade(lights, ray, &r, numLights);
	}

	return (RT_Vec3f)(0.0f);
}