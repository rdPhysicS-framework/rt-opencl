inline void DisplayPixel(const int x, 
						 const int y, 
						 const int w,
						 const RT_Color *pc, 
						 __global int *bufferImage)
{ 
	//int invR = w - x - 1;
	int i = (y * w) + x;
	bufferImage[i] = pc->rgba;
}

__kernel void render(__constant RT_Sphere *spheres, int size, __global int *bufferImage)
{
	unsigned int id = get_global_id(0);
	unsigned int x = id % 1920;
	unsigned int y = id / 1920;

	RT_Vec3f p = (RT_Vec3f)( x - 0.5f * (1920 - 1.0f),
							 y - 0.5f * (1080 - 1.0f),
							-300.0f);

	RT_Ray ray = CreateRay(p, (RT_Vec3f)(0, 0, 1));

	RT_Color pc = CreatePixelColorv3(TraceRay(spheres, size, &ray));
	DisplayPixel(x, y, 1920, &pc, bufferImage);
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
