RT_Vec3f Lambertian_F(const RT_Result *r)
{ 
	return r->material.color * (r->material.diffuse * INVPI);
}

RT_Vec3f GlossySpecular_F(const RT_Result *r, 
						  const RT_Vec3f *wi,
						  const RT_Vec3f *wo)
{ 
	RT_Vec3f l = (RT_Vec3f)(0.0f);

	RT_Vec3f refl = reflect(wi, &r->normal);
	float nDotWo = dot(refl, *wo);

	if(nDotWo > 0.0f)
		l = r->material.color * (r->material.specular * pow(nDotWo, r->material.intensity));
			
	return l;
}

RT_Vec3f Shade(__global const RT_Light *lights,
			   __global const RT_Plane *planes,
			   __global const RT_Sphere *spheres, 
			   __global const RT_Box *box,
			   const RT_Ray *ray,
			   const RT_Result *r,
			   const int numLights,
			   const int numPlanes, 
			   const int numSpheres,
			   const int numBox)
{
	RT_Vec3f wo = ray->d;
	RT_Vec3f color = (r->material.color * r->material.ambient);// * (RT_Vec3f)(1.5f);

	for(int i = 0; i < numLights; i++)
	{
		RT_Light l = lights[i];
		RT_Vec3f wi = Direction(&l, r);
		
		float nDotWi = dot(r->normal, wi);
		if (nDotWi > 0.0f)
		{
			/*inShadow*/
			if(!InShadow(&l, planes, spheres, box, 
				CreateRay(r->lhitPoint, wi), 
			    numPlanes, numSpheres, numBox)     )
			{ 
				color += (Lambertian_F(r) + 
						  GlossySpecular_F(r, &wi, &wo)) *
						  Color(&l) * nDotWi;
			}
		}
	}

	return color;
}

/*RT_Vec3f AreaLightShade(const RT_Material *m, const RT_Light *l, const RT_Result *r)
{
	RT_Vec3f color;
	return color;
}*/

RT_Vec3f TraceRay(__global const RT_Light *lights,
				  __global const RT_Plane *planes,
				  __global const RT_Sphere *spheres,
				  __global const RT_Box *box,
				  const RT_Ray *ray,
				  const int numLights, 
				  const int numPlanes,
				  const int numSpheres,
				  const int numBox)
{
	RT_Result r = Hit(planes, spheres, box, ray,
					  numPlanes, numSpheres, numBox);

	if(r.hit)
	{
		r.ray = *ray;
		return Shade(lights, planes, spheres, box, ray, &r, 
					 numLights, numPlanes, numSpheres, numBox);
	}

	return (RT_Vec3f)(0.0f);
}