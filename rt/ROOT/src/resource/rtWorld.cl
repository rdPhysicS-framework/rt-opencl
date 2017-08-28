/*void ComputeUVW(const RT_Camera *camera)
{
	if(camera->eye.x == camera->lookAt.x &&
	   camera->eye.z == camera->lookAt.z &&
	   camera->eye.y > camera->lookAt.y    )
	{ 
		camera->u = (RT_Vec3f)(0, 0, 1);
		camera->v = (RT_Vec3f)(1, 0, 0);
		camera->w = (RT_Vec3f)(0, 1, 0);
	}
	else if(camera->eye.x == camera->lookAt.x &&
	        camera->eye.z == camera->lookAt.z &&
	        camera->eye.y < camera->lookAt.y    )
	{ 
		camera->u = (RT_Vec3f)(1, 0, 0);
		camera->v = (RT_Vec3f)(0, 0, 1);
		camera->w = (RT_Vec3f)(0, -1, 0);
	}
	else
	{
		camera->w = normalize(camera->eye - camera->lookAt);
		camera->u = normalize(cross(camera->up, camera->w));
		camera->v = normalize(cross(camera->w, camera->u));
	}
}*/


RT_Result Hit(__global const RT_Plane *planes,
			  __global const RT_Sphere *spheres,
			  __global const RT_Box *box, 
			  const RT_Ray *ray,
			  const int numPlanes,
			  const int numSpheres,
			  const int numBox)
{
	RT_Result r = CreateResult();
	RT_Vec3f normal;
	RT_Vec3f hp;

	float tmin = INFINITE;
	float t = 0;
	
	for(int i = 0; i < numPlanes; i++)
	{
		RT_Plane p = planes[i];
		if(Plane_Hit(&p, ray, &t, &r) && t < tmin)
		{
			tmin = t;
			r.hit = true;
			r.material = p.material;
			normal = r.normal;
			hp = r.lhitPoint;
		}
	}

	for(int i = 0; i < numSpheres; i++)
	{
		RT_Sphere s = spheres[i];
		if(Sphere_Hit(&s, ray, &t, &r) && t < tmin)
		{
			r.hit = true;
			tmin = t;
			r.material = s.material;
			normal = r.normal;
			hp = r.lhitPoint;
		}	
	}

	for(int i = 0; i < numBox; i++)
	{
		RT_Box b = box[i];
		if(Box_Hit(&b, ray, &t, &r) && t < tmin)
		{
			r.hit = true;
			tmin = t;
			r.material = b.material;
			normal = r.normal;
			hp = r.lhitPoint;
		}	
	}

	if(r.hit)
	{ 
		r.t = tmin;
		r.normal = normal;
		r.lhitPoint = hp;
	}

	return r;
}

bool ShadowHit(__global const RT_Plane *planes,
			   __global const RT_Sphere *spheres,
			   __global const RT_Box *box, 
			   const RT_Ray *ray,
			   const int numPlanes,
			   const int numSpheres,
			   const int numBox,
			   float tmin)
{
	float t = 0;
	
	for(int i = 0; i < numPlanes; i++)
	{
		RT_Plane p = planes[i];
		if(Plane_ShadowHit(&p, ray, &t) && t < tmin)
		{ 
			return true;
		}
	}

	for(int i = 0; i < numSpheres; i++)
	{
		RT_Sphere s = spheres[i];
		if(Sphere_ShadowHit(&s, ray, &t) && t < tmin)
		{ 
			return true;
		}
	}

	for(int i = 0; i < numBox; i++)
	{
		RT_Box b = box[i];
		if(Box_ShadowHit(&b, ray, &t) && t < tmin)
		{ 
			return true;
		}
	}

	return false;
}

/*----------------------------------------------------------------------------------------------
 *
 * Camera
 *
 *----------------------------------------------------------------------------------------------*/

inline RT_Vec3f GetDirectionRayCam(const RT_Vec2f *point, __constant RT_Camera *camera)
{ 
	return normalize(camera->u * point->x +
					 camera->v * point->y +
					 camera->w * -camera->viewPlaneDistance);
}
