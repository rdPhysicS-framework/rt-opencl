inline void DisplayPixel(const int x, 
						 const int y, 
						 const int w,
						 const RT_Color *pc, 
						 __global int *bufferImage)
{ 
	int invR = w - x - 1;
	int i = (y * w) + x;
	/*int rgba = (((pc->x * 255) << RSHIFT) |
				((pc->y * 255) << GSHIFT) |
				((pc->z * 255) << BSHIFT) |
				(0xFF << ASHIFT));*/
	*(bufferImage + i) = pc->rgba;
}

__kernel void render(__constant RT_ViewPlane *viewPlane,
				     __constant RT_Sphere *spheres, 
					 int sizeSpheres, 
					 __global int *bufferImage)
{
	RT_ViewPlane vp = *viewPlane;
	unsigned int id = get_global_id(0);
	unsigned int x = id % vp.width;
	unsigned int y = id / vp.width;

	RT_Vec3f p = (RT_Vec3f)( x - 0.5f * (vp.width - 1.0f),
							 y - 0.5f * (vp.height - 1.0f),
							-300.0f);

	RT_Ray ray = CreateRay(p, (RT_Vec3f)(0, 0, 1));

	RT_Color pc = CreatePixelColorv3(TraceRay(spheres, sizeSpheres, &ray));
	//Saturate(&pc);
	DisplayPixel(x, y, vp.width, &pc, bufferImage);
}

__kernel void render2(__constant RT_ViewPlane *viewPlane,
					  __constant RT_Light *lights,
				      __constant RT_Sphere *spheres, 
					  const int sizeLights, const int sizeSpheres, 
					  __global int *bufferImage)
{
	RT_ViewPlane vp = *viewPlane;
	unsigned int id = get_global_id(0);
	unsigned int x = id % vp.width;
	unsigned int y = id / vp.width;

	RT_Vec3f p = (RT_Vec3f)( x - 0.5f * (vp.width - 1.0f),
							 y - 0.5f * (vp.height - 1.0f),
							-300.0f);

	RT_Ray ray = CreateRay(p, (RT_Vec3f)(0, 0, 1));

	RT_Vec3f cf = _TraceRay(spheres, lights, &ray, sizeLights, sizeSpheres);
	Saturate(&cf);
	RT_Color pc = CreatePixelColorv3(cf);
	DisplayPixel(x, y, vp.width, &pc, bufferImage);
}

__kernel void _render(__constant RT_Sphere *spheres, int size, __global int *bufferImage)
{
	RT_Ray ray = CreateRay((RT_Vec3f)(0), (RT_Vec3f)(0, 0, 1));
	RT_Color pc;

	for(int y = 0; y < 1080; y++)
	{
		for(int x = 0; x < 1920; x++)
		{  
			RT_Vec3f p = (RT_Vec3f)(x - 0.5f * (1920 - 1.0f),
								    y - 0.5f * (1080 - 1.0f),
								   -100.0f);
			ray.o = p;
			RT_Vec3f c = TraceRay(spheres, size, &ray);
			pc = CreatePixelColorv3(c);
			DisplayPixel(x, y, 1920, &pc, bufferImage);
		}
	}
}
