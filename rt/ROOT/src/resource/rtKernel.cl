inline void DisplayPixel(const int x, 
						 const int y, 
						 const int w,
						 const RT_Color *pc, 
						 __global int *bufferImage)
{ 
	int invR = w - x - 1;
	int i = (y * w) + invR;
	*(bufferImage + i) = pc->rgba;
}

__kernel void render(__constant RT_ViewPlane *viewPlane,
					 __constant RT_Camera *camera,
					 __global const RT_Light *lights,
					 __global const RT_Plane *planes,
				     __global const RT_Sphere *spheres,
					 __global const RT_Box *box, 
					 const int numLights,
					 const int numPlanes, 
					 const int numSpheres, 
					 const int numBox,
					 __global int *bufferImage )
{
	RT_Vec2f s = viewPlane->sp / camera->zoom;
	unsigned int id = get_global_id(0);
	unsigned int x = id % viewPlane->width;
	unsigned int y = id / viewPlane->width;
	
	RT_Vec2f pp = (RT_Vec2f)( s.x * (x - 0.5f * viewPlane->width),
							 -s.y * (y - 0.5f * viewPlane->height));

	RT_Ray ray = CreateRay(camera->eye, GetDirectionRayCam(&pp, camera));

	RT_Vec3f cf = TraceRay(lights, planes, spheres, box, &ray, 
						   numLights, numPlanes, numSpheres, numBox);
	Saturate(&cf);
	RT_Color pc = CreatePixelColorv3(cf);
	DisplayPixel(x, y, viewPlane->width, &pc, bufferImage);
}

/*__kernel void render2(__constant RT_ViewPlane *viewPlane,
					 __global const RT_Light *lights,
					 __global const RT_Plane *planes,
				     __global const RT_Sphere *spheres, 
					 const int numLights,
					 const int numPlanes, 
					 const int numSpheres, 
					 __global int *bufferImage )
{
	unsigned int id = get_global_id(0);
	unsigned int x = id % viewPlane->width;
	unsigned int y = id / viewPlane->width;

	RT_Vec3f p = (RT_Vec3f)( s.x * (x - 0.5f * viewPlane->width),
							-s.y * (y - 0.5f * viewPlane->height),
							-500.0f);*/

	/*RT_Ray ray = CreateRay(p, (RT_Vec3f)(0, 0, 1));

	RT_Vec3f cf = TraceRay(lights, planes, spheres, &ray, 
						   numLights, numPlanes, numSpheres);
	Saturate(&cf);
	RT_Color pc = CreatePixelColorv3(cf);
	DisplayPixel(x, y, viewPlane->width, viewPlane->height, &pc, bufferImage);
}*/
