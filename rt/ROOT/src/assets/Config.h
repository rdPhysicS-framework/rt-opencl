#ifndef	__CONFIG_H__
#define	__CONFIG_H__

#include <iostream>
#include <ostream>
#include <map>

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

inline RT_Vec3f operator+(const RT_Vec3f &v1, const RT_Vec3f &v2)
{
	return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

inline RT_Vec3f operator-(const RT_Vec3f &v1, const RT_Vec3f &v2)
{
	return{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline RT_Vec3f Cross(const RT_Vec3f &v1, const RT_Vec3f &v2)
{
	return {(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x)};
}
inline RT_Vec3f Normalize(const RT_Vec3f &v)
{
	float invSize = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return {v.x * invSize, v.y * invSize, v.z * invSize};
}

std::ostream &operator<<(std::ostream &out, const RT_Vec3f &v)
{
	out << "(" << v.x << " - " << v.y << " - " << v.z << ")" << std::endl;
	return out;
}

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
	RT_Vec3f point;
	RT_Vec3f normal;
	//float dist;

	RT_Material material;
}RT_Plane;

typedef struct
{
	RT_Vec3f position;
	RT_Vec3f size;

	RT_Material material;
} RT_Box;

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

typedef struct
{
	/*position*/
	RT_Vec3f eye;
	RT_Vec3f lookAt;
	RT_Vec3f up;

	float viewPlaneDistance;
	float zoom;

	float rollAngle;
	float exposureTime;

	RT_Vec3f u, v, w;


	void computeUVW()
	{
		w = Normalize(eye - lookAt);
		
		if (eye.x == lookAt.x && eye.z == lookAt.z && eye.y > lookAt.y)
		{
			u = {0, 0, 1};
			v = {1, 0, 0};
			w = {0, 1, 0};
			return;
		}

		if (eye.x == lookAt.x && eye.z == lookAt.z && eye.y < lookAt.y)
		{
			u = {1, 0, 0};
			v = {0, 0, 1};
			w = {0, -1, 0};
			return;
		}

		u = Normalize(Cross(up, w));
		v = Normalize(Cross(w, u));
	}

} RT_Camera;

/*struct RT_Program {
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
	/*}

	void BildProgram()
	{
		cl_int status = program.build({ device });
		if (status)
		{
			std::cerr << "Error during conpilation OpenCl code: "
				<< status << std::endl;
		}
	}
};*/

class Kernel 
{
private:
	cl_kernel kernel;
	std::map<char*, cl_mem> memObjects;

public:
	Kernel(const Program &p, const char *kernelName)
	{
		cl_int status;
		kernel = clCreateKernel(p.program, kernelName, &status);
		/*se houver tratar erro*/
	}

	Kernel setMemObject(const Program &p, cl_mem_flags flag, size_t size, char *nameObj)
	{
		cl_int status;
		if (memObjects.find(nameObj) == memObjects.end())
		{
			memObjects[nameObj] = clCreateBuffer(p.context, flag, size, nullptr, &status);
		}
		/*se houver tratar erro*/

		return *this;
	}

	cl_kernel getKernel() const
	{
		return kernel;
	}

	cl_mem getMemObject(char *name) 
	{
		if (memObjects.find(name) == memObjects.end())
		{
			std::cerr << "Objeto informado nao existe" << std::endl;
		}
		return memObjects[name];
	}

	Kernel setArgument(unsigned int id, size_t size, const void *value)
	{
		cl_int status;
		status = clSetKernelArg(kernel, id, size, value);
		/*se houver tratar erro*/
		return *this;
	}

	Kernel WriteBuffer(const Program &p, size_t size, char *name, const void *value)
	{
		cl_int status;
		status = clEnqueueWriteBuffer(p.queue, getMemObject(name), CL_TRUE, 0, size, value, 0, nullptr, nullptr);
		/*se houver tratar erro*/
		return *this;
	}

	Kernel ReadBuffer(const Program &p, size_t size, char *name, void *value)
	{
		cl_int status;
		status = clEnqueueReadBuffer(p.queue, getMemObject(name), CL_TRUE, 0, size, value, 0, nullptr, nullptr);
		/*se houver tratar erro*/
		return *this;
	}

	Kernel NDRangeKernel(const Program &p, const size_t *globalItemSize, const size_t *localItemSize)
	{
		cl_int status;
		status = clEnqueueNDRangeKernel(p.queue, kernel, 1, nullptr, globalItemSize, localItemSize, 0, nullptr, nullptr);
		/*se houver tratar erro*/
		return *this;
	}

	~Kernel()
	{
		cl_int status;
		status = clReleaseKernel(kernel);

		for (std::map<char*, cl_mem>::iterator i = memObjects.begin(); i != memObjects.end(); i++)
		{
			status |= clReleaseMemObject(i->second);
		}
		memObjects.clear();
		/*se houver tratar erro*/
	}
};

std::string readInputStream(const std::string &name)
{
	std::string code;
	std::ifstream kernelFile;

	kernelFile.exceptions(std::ifstream::failbit |
		std::ifstream::badbit);

	try
	{
		kernelFile.open(name.c_str());

		std::stringstream kernelStream;

		kernelStream << kernelFile.rdbuf();

		kernelFile.close();

		code = kernelStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		throw std::out_of_range(e.what());
	}

	return code;
}

std::string LoadSource(std::initializer_list<std::string> kernels)
{
	if (kernels.size() == 0)
	{
		throw std::exception("You must provide kernel names!");
	}

	std::string code;
	std::vector<std::string> k = kernels;
	for (std::string i : k)
	{
		code += readInputStream(i);
	}

	return code;
}

#endif//__CONFIG_H__
