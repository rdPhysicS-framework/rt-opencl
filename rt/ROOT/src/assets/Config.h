#ifndef	__CONFIG_H__
#define	__CONFIG_H__

#include <iostream>

#ifdef APPLE
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#include<CL/cl.hpp>
#endif // APPLE

#define PATH "src/resource/"
#define FILE_NAME "raytrace.png"

#define NAME_NUM 4
#define DATA_NUM 21
#define WINDOW_SIZE 13
#define MAX_SOURCE_SIZE 0x100000

#define WIDTH_CANVAS 1920
#define HEIGHT_CANVAS 1080

typedef cl_float3 RT_Vec3f;
typedef cl_float2 RT_Vec2f;

typedef struct
{
	float f[3];
} Package;

typedef struct
{
	float rf, gf, bf, af;
	int ri, gi, bi, ai;
	int rgba;

} RT_Color;

struct Program {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;

	Program() : platform(nullptr),
		        device(nullptr),
		        queue(nullptr),
		        program(nullptr) {}
	~Program() 
	{
		cl_int status;
		status = clReleaseProgram(program);
		//ret = clReleaseDevice(device);
		status = clReleaseCommandQueue(queue);
		status = clReleaseContext(context);
	}

	void BildProgram()
	{
		cl_int status;
		status = clBuildProgram(program, 1, &device, 
							    nullptr, nullptr, nullptr);
		/*caso houver tratar erro*/
	}
};

typedef struct
{
	RT_Vec3f color;
	float ambient;//RT_Lambertian
	float diffuse;//RT_Lambertian
	float specular;//RT_GlossySpecular
	float intensity;
} RT_Material;

typedef struct
{
	RT_Vec3f center;
	float radius;

	RT_Material material;
} RT_Sphere;

typedef struct
{
	RT_Vec3f position;
	RT_Vec3f color;
	float ls;
} RT_Light;

typedef struct
{
	int width;
	int height;
	RT_Vec2f sp;
	//RT_SScoord coord;
} RT_ViewPlane;

struct RT_Program {
	cl::Device device;
	cl::Context context;
	cl::CommandQueue queue;
	cl::Program program;

	~RT_Program()
	{
		/*cl_int status;
		status = clReleaseProgram(program);
		//ret = clReleaseDevice(device);
		status = clReleaseCommandQueue(queue);
		status = clReleaseContext(context);*/
	}

	void BildProgram()
	{
		cl_int status = program.build({ device });
		if (status)
		{
			std::cerr << "Error during conpilation OpenCl code: "
				<< status << std::endl;
		}
	}
};
#endif//__CONFIG_H__
