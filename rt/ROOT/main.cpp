#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#ifdef APPLE
	#include <OpenCL\opencl.h>
#else
	#include <CL\cl.h>
	#include<CL\cl.hpp>
#endif // APPLE

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define PATH "src/resource/"
#define FILE_NAME "raytrace.png"

void displayPlatformInfo(cl_platform_id id, 
						 cl_platform_info param_name, 
						 const char *paranNameAsStr)
{
	cl_int error = 0;
	size_t paramSize = 0;
	error = clGetPlatformInfo(id, param_name, 0, nullptr, &paramSize);
	char *moreInfo = new char[paramSize];
	error = clGetPlatformInfo(id, param_name, paramSize, moreInfo, nullptr);

	if (error != CL_SUCCESS)
	{
		std::cerr << "Unable to find any OpenCL platform information" << std::endl;
		return;
	}

	std::cout << paranNameAsStr << ": " << moreInfo << std::endl;
}

void displayDeviceDetails(cl_device_id id,
						  cl_device_info param_name,
						  const char *paramNameAsStr)
{
	cl_int error = 0;
	size_t paramSize = 0;

	error = clGetDeviceInfo(id, param_name, 0, nullptr, &paramSize);

	if (error != CL_SUCCESS)
	{
		perror("Unable to obtain devices info for param\n");
		return;
	}

	switch (param_name)
	{
	case CL_DEVICE_TYPE:
	{
		cl_device_type *type = new cl_device_type[paramSize];
		error = clGetDeviceInfo(id, param_name, paramSize, type, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		switch (*type)
		{
		case CL_DEVICE_TYPE_CPU:
			std::cout << "CPU detected" << std::endl;
			break;
		case CL_DEVICE_TYPE_GPU:
			std::cout << "GPU detected" << std::endl;
			break;
		case CL_DEVICE_TYPE_DEFAULT:
			std::cout << "DEFAULT detected" << std::endl;
			break;

			delete type;
		}
	}
	break;

	case CL_DEVICE_VENDOR_ID:
	case CL_DEVICE_MAX_COMPUTE_UNITS:
	case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
	{
		cl_uint *ret = new cl_uint[paramSize];
		error = clGetDeviceInfo(id, param_name, paramSize, ret, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		switch (param_name)
		{
		case CL_DEVICE_VENDOR_ID:
			std::cout << "Vedor ID: "
				<< *ret << std::endl;
			break;
		case CL_DEVICE_MAX_COMPUTE_UNITS:
			std::cout << "Maximum number of parallel compute units: "
				<< *ret << std::endl;
			break;
		case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
			std::cout << "Maximum dimensions for global / local work - item IDs: "
				<< *ret << std::endl;
			break;
		}
	}
	break;

	case CL_DEVICE_MAX_WORK_ITEM_SIZES:
	{
		cl_uint maxWIDimensions;
		size_t *ret = new size_t[paramSize];

		error = clGetDeviceInfo(id, param_name, paramSize, ret, nullptr);
		error = clGetDeviceInfo(id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxWIDimensions, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		std::cout << "Maximum number of work-items in each dimension: (  ";
		for (int i = 0; i < maxWIDimensions; i++)
		{
			std::cout << ret[i] << " ";
		}

		std::cout << ")" << std::endl;
	}
	break;

	case CL_DEVICE_MAX_WORK_GROUP_SIZE:
	{
		size_t *ret = new size_t[paramSize];

		error = clGetDeviceInfo(id, param_name, paramSize, ret, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		std::cout << "Maximum number of work-items in a work-group: "
			<< *ret << std::endl;
	}
	break;

	case CL_DEVICE_NAME:
	case CL_DEVICE_VENDOR:
	{
		char data[48];
		error = clGetDeviceInfo(id, param_name, paramSize, data, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		switch (param_name)
		{
		case CL_DEVICE_NAME:
			std::cout << "Device name is "
				<< data << std::endl;
			break;
		case CL_DEVICE_VENDOR:
			std::cout << "Device vendor is "
				<< data << std::endl;
			break;
		}
	}
	break;


	case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
	{
		cl_uint *size = new cl_uint[paramSize];

		error = clGetDeviceInfo(id, param_name, paramSize, size, nullptr);

		if (error != CL_SUCCESS)
		{
			perror("Unable to obtain devices info for param\n");
			return;
		}

		std::cout << "Device global cacheline size: "
			<< *size << " bytes" << std::endl;
	}
	break;
	case CL_DEVICE_GLOBAL_MEM_SIZE:
	case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
	{
		cl_ulong *size = new cl_ulong[paramSize];
		error = clGetDeviceInfo(id, param_name, paramSize, size, NULL);

		if (error != CL_SUCCESS) {
			perror("Unable to obtain device name/vendor info for param\n");
			return;
		}

		switch (param_name) {
		case CL_DEVICE_GLOBAL_MEM_SIZE:
			std::cout << "Device global mem: "
				<< ((*size) >> 20) << " mega-bytes" << std::endl;
			break;
		case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
			std::cout << "Device max memory allocation: "
				<< ((*size) >> 20) << " mega-bytes" << std::endl;
			break;
		}
	}
	break;

	case CL_DEVICE_LOCAL_MEM_SIZE:
	{
		cl_ulong size = 0;
		error = clGetDeviceInfo(id, param_name, paramSize, &size, NULL);

		if (error != CL_SUCCESS) {
			perror("Unable to obtain device name/vendor info for param\n");
			return;
		}

		std::cout << "Local memory: " << size << " bits" << std::endl;
	}
	break;
	}
}

void displayDeviceInfo(cl_platform_id id,
					   cl_device_type dev_type)
{
	cl_int error = 0;
	cl_uint numDevices = 0;

	error = clGetDeviceIDs(id, dev_type, 0, nullptr, &numDevices);
	if (error != CL_SUCCESS)
	{
		std::cerr << "Unable to obtain any OpenCL compliant device info" << std::endl;
		system("pause");
		exit(1);
	}

	cl_device_id *devices = new cl_device_id[numDevices];

	error = clGetDeviceIDs(id, dev_type, numDevices, devices, nullptr);
	if (error != CL_SUCCESS)
	{
		std::cerr << "Unable to obtain any OpenCL compliant device info" << std::endl;
		system("pause");
		exit(1);
	}

	std::cout << "Number of detected OpenCL devices: " << numDevices << std::endl;

	for (int i = 0; i < numDevices; ++i) {
		displayDeviceDetails(devices[i], CL_DEVICE_TYPE, "CL_DEVICE_TYPE");
		displayDeviceDetails(devices[i], CL_DEVICE_NAME, "CL_DEVICE_NAME");
		displayDeviceDetails(devices[i], CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR");
		displayDeviceDetails(devices[i], CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
		displayDeviceDetails(devices[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE, "CL_DEVICE_MAX_MEM_ALLOC_SIZE");
		displayDeviceDetails(devices[i], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE");
		displayDeviceDetails(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, "CL_DEVICE_GLOBAL_MEM_SIZE");
		displayDeviceDetails(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
		displayDeviceDetails(devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS");
		displayDeviceDetails(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, "CL_DEVICE_MAX_WORK_ITEM_SIZES");
		displayDeviceDetails(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE");
	}
}

inline std::string readInputStream(const std::string &name)
{
	std::string code;
	std::ifstream kernelFile;

	kernelFile.exceptions(std::ifstream::failbit |
						  std::ifstream::badbit   );

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

inline std::string LoadSource(std::initializer_list<std::string> kernels)
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

#define NAME_NUM 4
#define DATA_NUM 21
#define WINDOW_SIZE 13
#define MAX_SOURCE_SIZE 0x100000

typedef cl_float3 RT_Vec3f;

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
				program(nullptr){}
};

typedef struct
{
	RT_Vec3f center;
	float radius;
} RT_Sphere;

int main(int argc, char **argv)
{
	cl_uint retNumPlatforms;
	cl_uint retNumDevices;
	cl_mem memObjIn = nullptr;
	cl_mem memObjOut = nullptr;
	cl_kernel kernel = nullptr;
	Program prog;

	size_t sourceSize;
	char *source = nullptr;
	cl_int ret;
	//RT_Vec3f out;

	const int size = 2;
	RT_Sphere in[size];
	in[0].center.s[0] = 162;
	in[0].center.s[1] = 54;
	in[0].center.s[2] = 432;
	in[0].radius = 216;

	in[1].center.s[0] = -540;
	in[1].center.s[1] = -86;
	in[1].center.s[2] = 432;
	in[1].radius = 270;

	const int qtd = 1920 * 1080;
	int *outI = new int[qtd];

	ret = clGetPlatformIDs(1, &prog.platform, &retNumPlatforms);

	ret = clGetDeviceIDs(prog.platform, CL_DEVICE_TYPE_GPU, 1, &prog.device, &retNumDevices);

	prog.context = clCreateContext(nullptr, 1, &prog.device, nullptr, nullptr, &ret);

	prog.queue = clCreateCommandQueue(prog.context, prog.device, 0, &ret);

	//só aqui que funciona, antes de criar o programa
	//no inicio antes de capturar as plataformas não funciona
	try
	{
		std::string s = LoadSource({ PATH"rtUtil.cl", 
									 PATH"rtMaterial.cl",
									 PATH"rtGeometricObject.cl", 
									 PATH"rtWorld.cl",
									 PATH"rtLights.cl",
									 PATH"rtKernel.cl" });
		source = const_cast<char*>(s.c_str());
		sourceSize = s.size();
		//std::cout << s << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << "\nFailed to load kernel." << std::endl;
		system("pause");
		exit(1);
	}
	
	
	prog.program = clCreateProgramWithSource(prog.context, 1, (const char**)&source, (const size_t*)&sourceSize, &ret);

	ret = clBuildProgram(prog.program, 1, &prog.device, nullptr, nullptr, nullptr);

	kernel = clCreateKernel(prog.program, "render", &ret);

	memObjIn = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(RT_Sphere)*size, nullptr, &ret);
	memObjOut = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(int)*qtd, nullptr, &ret);

	ret = clEnqueueWriteBuffer(prog.queue, memObjIn, CL_TRUE, 0, sizeof(RT_Sphere)*size, in, 0, nullptr, nullptr);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&memObjIn);
	ret = clSetKernelArg(kernel, 1, sizeof(int), (void*)&size);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&memObjOut);

	//ret = clEnqueueTask(prog.queue, kernel, 0, nullptr, nullptr);

	size_t globalItemSize = qtd;
	size_t localItemSize = 1;
	//for (int i = 0; i < qtd; i++) {
	//	//std::cout << localItemSize << std::endl;
	//	ret = clEnqueueNDRangeKernel(prog.queue, kernel, 1, nullptr, &globalItemSize, &localItemSize, 0, nullptr, nullptr);
	//	ret = clEnqueueReadBuffer(prog.queue, memObjOut, CL_TRUE, 0, sizeof(int)*qtd, outI, 0, nullptr, nullptr);
	//}
	
	ret = clEnqueueNDRangeKernel(prog.queue, kernel, 1, nullptr, &globalItemSize, &localItemSize, 0, nullptr, nullptr);
	ret = clEnqueueReadBuffer(prog.queue, memObjOut, CL_TRUE, 0, sizeof(int)*qtd, outI, 0, nullptr, nullptr);
	
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(prog.program);
	ret = clReleaseMemObject(memObjIn);
	ret = clReleaseMemObject(memObjOut);
	ret = clReleaseCommandQueue(prog.queue);
	ret = clReleaseContext(prog.context);
	
	SDL_Surface *img = SDL_CreateRGBSurface(SDL_SWSURFACE, 1920, 1080, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	img->pixels = (cl_uint*)outI;
	IMG_SavePNG(img, FILE_NAME);
	//std::cout << out.x << ", " << out.y << ", " << out.z << std::endl;
	delete outI;
	//SDL_FreeSurface(img);

	system("pause");
	return 0;
}