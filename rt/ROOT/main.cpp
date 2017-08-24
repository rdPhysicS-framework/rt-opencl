#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "src\assets\Config.h"
#include "src\assets\Util.h"

void setSphere(RT_Sphere *spheres)
{
	spheres[0].center.s[0] = 162;
	spheres[0].center.s[1] = 54;
	spheres[0].center.s[2] = 432;
	spheres[0].radius = 216;
	spheres[0].material.color = { 0.7f, 0.7f, 1.0f };
	spheres[0].material.ambient = 0.2f;
	spheres[0].material.diffuse = 0.7f;
	spheres[0].material.specular = 0.8f;
	spheres[0].material.intensity = 20.f;

	spheres[1].center.s[0] = -540;
	spheres[1].center.s[1] = -86;
	spheres[1].center.s[2] = 432;
	spheres[1].radius = 270;
	spheres[1].material.color = { 0.7f, 0.7f, 1.0f };
	spheres[1].material.ambient = 0.2f;
	spheres[1].material.diffuse = 0.7f;
	spheres[1].material.specular = 0.8f;
	spheres[1].material.intensity = 20.f;
}

void setLights(RT_Light *lights)
{
	lights[0].color = { 0.7f, 0.7f, 0.6f };
	lights[0].position = { 162.0f, 756.0f, 432.0f };
	lights[0].ls = 1.0f;

	lights[1].color = { 0.7f, 0.7f, 0.6f };
	lights[1].position = { -540.0f, 756.0f, 432.0f };
	lights[1].ls = 1.0f;
}

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

	const int sizeSpheres = 2;
	RT_Sphere in[sizeSpheres];
	setSphere(in);

	const int sizeLights = 2;
	RT_Light lights[sizeLights];
	setLights(lights);

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

	memObjIn = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(RT_Sphere)*sizeSpheres, nullptr, &ret);
	memObjOut = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(int)*qtd, nullptr, &ret);

	ret = clEnqueueWriteBuffer(prog.queue, memObjIn, CL_TRUE, 0, sizeof(RT_Sphere)*sizeSpheres, in, 0, nullptr, nullptr);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&memObjIn);
	ret = clSetKernelArg(kernel, 1, sizeof(int), (void*)&sizeSpheres);
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