#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "src\assets\Util.h"
#include "src\assets\Info.h"
#include "src\Sampler\Regular.h"

void setPlanes(RT_Plane *planes)
{
	planes[0] = { { 0, -324, 0 },{ 0,  1,  0 },{ { 0.4f, 0.3f, 0.3f }, 0.2f, 0.7f, 0.5f, 25.0f } };
	planes[1] = { { 0, 0, 2419 },{ 0,  0, -1 },{ { 0.6f, 0.5f, 0.5f }, 0.2f, 1.0f, 0.8f, 25.f } };
	planes[2] = { { -1296, 0, 0 },{ 1,  0,  0 },{ { 0.5f, 0.6f, 0.6f }, 0.2f, 1.0f, 0.8f, 25.0f } };
	planes[3] = { { 1296, 0, 0 },{ -1,  0,  0 },{ { 0.5f, 0.6f, 0.6f }, 0.2f, 1.0f, 0.8f, 25.0f } };
	planes[4] = { { 0, 800, 0 },{ 0, -1,  0 },{ { 0.7f, 0.7f, 0.7f }, 0.2f, 1.0f, 0.8f, 25.0f } };
}

void printPlanes(RT_Plane *planes, int numPlanes)
{
	std::cout << std::endl;
	for (int i = 0; i < numPlanes; i++)
	{
		std::cout << "normal: " << planes[i].normal.x << " - " << planes[i].normal.y << " - " << planes[i].normal.z << std::endl;;
		std::cout << "point: " << planes[i].point.x << " - " << planes[i].point.y << " - " << planes[i].point.z << std::endl;;
		//std::cout << "distance: " << planes[i].dist << std::endl;
		std::cout << "color: " << planes[i].material.color.x << " - " << planes[i].material.color.y << " - " << planes[i].material.color.z << std::endl;
		std::cout << "ambient: " << planes[i].material.ambient << std::endl;
		std::cout << "diffuse: " << planes[i].material.diffuse << std::endl;
		std::cout << "specular: " << planes[i].material.specular << std::endl;
		std::cout << "intensity: " << planes[i].material.intensity << std::endl;
		std::cout << std::endl;
	}
}

void setSphere(RT_Sphere *spheres)
{
	spheres[0] = { { 162,  54, 432 }, 216,{ { 0.6f, 0.6f, 0.6f }, 0.2f, 0.7f, 0.8f, 1.0f } };
	spheres[1] = { { -540, -86, 432 }, 270,{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 1.0f } };
}

void setLights(RT_Light *lights)
{
	lights[0] = { { 162, 756, 432 },{ 0.6f, 0.6f, 0.7f }, 1.0f };
	lights[1] = { { -540, 756, 432 },{ 0.5f, 0.8f, 0.8f }, 1.0f };
}

int main(int argc, char **argv)
{
	cl_uint retNumPlatforms;
	cl_uint retNumDevices;
	Program prog;

	size_t sourceSize;
	char *source = nullptr;
	cl_int status;
	//RT_Vec3f out;

	/*input*/
	//paredes
	const int numPlanes = 5;
	RT_Plane planes[numPlanes];
	setPlanes(planes);

	const int sizeSpheres = 2;
	RT_Sphere spheres[sizeSpheres];
	setSphere(spheres);

	const int numBox = 1;
	RT_Box box[numBox] = { { { 0, -324, 270 },{ 324, 162, 324 },
	{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.2f, 20.0f } } };

	/*dados do cavas*/
	RT_ViewPlane vp = { WIDTH_CANVAS, HEIGHT_CANVAS,{ 1, 1 } };
	/*dados da camera*/
	RT_Camera camera = { { -702, -108, -270 },
						 { -540, -100, 0 },
						 { 0, 1, 0 },
						  800, 1.5f, 0, 1,
						  {},{},{} };
	camera.computeUVW();

	/*dados da luz*/
	const int sizeLights = 2;
	RT_Light lights[sizeLights];
	setLights(lights);

	RT::Regular sp(4);
	const int sp_size = sp.GetSamples().size();

	/*output*/
	const int sizeBuffer = WIDTH_CANVAS * HEIGHT_CANVAS;
	int *bufferImage = new int[sizeBuffer];

	status = clGetPlatformIDs(1, &prog.platform, &retNumPlatforms);

	status = clGetDeviceIDs(prog.platform, CL_DEVICE_TYPE_GPU, 1, &prog.device, &retNumDevices);

	prog.context = clCreateContext(nullptr, 1, &prog.device, nullptr, nullptr, &status);

	prog.queue = clCreateCommandQueue(prog.context, prog.device, 0, &status);

	//só aqui que funciona, antes de criar o programa
	//no inicio antes de capturar as plataformas não funciona
	try
	{
		std::string s = LoadSource({ PATH"rtUtil.cl",
									 PATH"rtStruct.cl",
									 PATH"rtGeometricObject.cl",
									 PATH"rtWorld.cl",
									 PATH"rtLights.cl",
									 PATH"rtTracer.cl",
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
	
	prog.program = clCreateProgramWithSource(prog.context, 1, (const char**)&source, (const size_t*)&sourceSize, &status);
	prog.BuildProgram();

	cl_kernel kernel = clCreateKernel(prog.program, "render", &status);

	cl_mem mem_vp = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_ViewPlane), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_camera = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_Camera), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_sp = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT::Vec2f)*sp_size, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_lights = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_Light)*sizeLights, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_planes = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_Plane)*numPlanes, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_spheres = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_Sphere)*sizeSpheres, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_boxs = clCreateBuffer(prog.context, CL_MEM_READ_ONLY, sizeof(RT_Box)*numBox, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_bfImage = clCreateBuffer(prog.context, CL_MEM_WRITE_ONLY, sizeof(int)*sizeBuffer, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_temp = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(RT::Vec2f)*sp_size, nullptr, &status);

	status = clEnqueueWriteBuffer(prog.queue, mem_vp, CL_TRUE, 0, sizeof(RT_ViewPlane), &vp, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_camera, CL_TRUE, 0, sizeof(RT_Camera), &camera, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_sp, CL_TRUE, 0, sizeof(RT::Vec2f)*sp_size, sp.GetSamples().data(), 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_lights, CL_TRUE, 0, sizeof(RT_Light)*sizeLights, lights, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_planes, CL_TRUE, 0, sizeof(RT_Plane)*numPlanes, planes, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_spheres, CL_TRUE, 0, sizeof(RT_Sphere)*sizeSpheres, spheres, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(prog.queue, mem_boxs, CL_TRUE, 0, sizeof(RT_Box)*numBox, box, 0, nullptr, nullptr);
	/*se houver, tratar erro*/

	status  = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&mem_vp);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&mem_camera);
	status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&mem_sp);
	status |= clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&mem_lights);
	status |= clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&mem_planes);
	status |= clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&mem_spheres);
	status |= clSetKernelArg(kernel, 6, sizeof(cl_mem), (void*)&mem_boxs);
	status |= clSetKernelArg(kernel, 7, sizeof(const int), (void*)&sizeLights);
	status |= clSetKernelArg(kernel, 8, sizeof(const int), (void*)&numPlanes);
	status |= clSetKernelArg(kernel, 9, sizeof(const int), (void*)&sizeSpheres);
	status |= clSetKernelArg(kernel, 10, sizeof(const int), (void*)&numBox);
	status |= clSetKernelArg(kernel, 11, sizeof(const int), (void*)&sp_size);
	status |= clSetKernelArg(kernel, 12, sizeof(cl_mem), (void*)&mem_bfImage);

	//ret = clEnqueueTask(prog.queue, kernel, 0, nullptr, nullptr);

	size_t globalItemSize = sizeBuffer;
	size_t localItemSize = 1;
	//for (int i = 0; i < qtd; i++) {
	//	//std::cout << localItemSize << std::endl;
	//	ret = clEnqueueNDRangeKernel(prog.queue, kernel, 1, nullptr, &globalItemSize, &localItemSize, 0, nullptr, nullptr);
	//	ret = clEnqueueReadBuffer(prog.queue, memObjOut, CL_TRUE, 0, sizeof(int)*qtd, outI, 0, nullptr, nullptr);
	//}
	
	status = clEnqueueNDRangeKernel(prog.queue, kernel, 1, nullptr, &globalItemSize, &localItemSize, 0, nullptr, nullptr);
	status = clEnqueueReadBuffer(prog.queue, mem_bfImage, CL_TRUE, 0, sizeof(int)*sizeBuffer, bufferImage, 0, nullptr, nullptr);
	
	status = clReleaseKernel(kernel);
	status |= clReleaseMemObject(mem_vp);
	status |= clReleaseMemObject(mem_camera);
	status |= clReleaseMemObject(mem_lights);
	status |= clReleaseMemObject(mem_planes);
	status |= clReleaseMemObject(mem_spheres);
	status |= clReleaseMemObject(mem_bfImage);
	
	SDL_Surface *img = SDL_CreateRGBSurface(SDL_SWSURFACE, 1920, 1080, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	img->pixels = (cl_uint*)bufferImage;
	IMG_SavePNG(img, FILE_NAME);
	//std::cout << out.x << ", " << out.y << ", " << out.z << std::endl;
	delete bufferImage;
	//SDL_FreeSurface(img);

	system("pause");
	return 0;
}