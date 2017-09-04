#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "src\assets\Util.h"
#include "src\assets\Info.h"
#include "src\Sampler\Regular.h"
#include "src\Sampler\MultiJittered.h"
#include <time.h>

static cl_int SetPlatforms(std::vector<cl_platform_id> &platforms)
{
	cl_uint num = 0;
	cl_int status = clGetPlatformIDs(0, nullptr, &num);
	if (status != CL_SUCCESS)
		return -1;

	cl_platform_id *p = new cl_platform_id[num];
	status = clGetPlatformIDs(num, p, nullptr);
	if (status != CL_SUCCESS)
		return -1;

	platforms.assign(&p[0], &p[num]);

	for (cl_platform_id id : platforms)
	{
		displayPlatformInfo(id, CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
	}
	std::cout << std::endl;
	return CL_SUCCESS;
}

static cl_int SetDevices(const cl_platform_id platform, 
						 const cl_device_type type,
						 std::vector<cl_device_id> &devices)
{
	cl_uint num = 0;
	cl_int status = clGetDeviceIDs(platform, type, 0, nullptr, &num);
	if (status != CL_SUCCESS)
		return -1;

	cl_device_id *ids = new cl_device_id[num];
	status = clGetDeviceIDs(platform, type, num, ids, nullptr);
	if (status != CL_SUCCESS)
		return -1;

	devices.assign(&ids[0], &ids[num]);

	for (cl_device_id id : devices)
	{
		displayDeviceDetails(id, CL_DEVICE_NAME, "CL_DEVICE_NAME");
		displayDeviceDetails(id, CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_NAME");
		displayDeviceDetails(id, CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_NAME");
	}
	std::cout << std::endl;
	return CL_SUCCESS;
}

int Option(int size)
{
	if (size == 1)
		return 0;

	int input = 0;

	std::cout << "Choice one of the options!" << std::endl
		      << "Option: ";
	std::cin >> input;

	while (input < 1 || input > size)
	{
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');
		std::cout << "No such option. Choose again!" << std::endl <<"nOption: ";
		std::cin >> input;
	}

	return input - 1;
}

cl_program CreateClProgram(const cl_context context,
						   std::initializer_list<std::string> kernels)
{
	char *source = nullptr;
	size_t sourceSize;

	try
	{
		std::string s = LoadSource(kernels);
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

	int status;
	cl_program p = clCreateProgramWithSource(context, 1, (const char**)&source, (const size_t*)&sourceSize, &status);
	/*tratar erro*/
	return p;
}

static Program CreateProgram()
{
	Program prog;
	std::vector<cl_platform_id> platforms;
	SetPlatforms(platforms);

	int p = Option(platforms.size());
	prog.platform = platforms[p];

	std::vector<cl_device_id> devices;
	SetDevices(prog.platform, CL_DEVICE_TYPE_ALL, devices);

	int d = Option(devices.size());
	prog.device = devices[d];

	cl_int status;
	prog.context = clCreateContext(nullptr, 1, &prog.device, nullptr, nullptr, &status);
	prog.queue = clCreateCommandQueue(prog.context, prog.device, 0, &status);

	prog.program = CreateClProgram(prog.context, 
								  { PATH"rtUtil.cl",
									PATH"rtStruct.cl",
									PATH"rtSampler.cl",
									PATH"rtGeometricObject.cl",
									PATH"rtWorld.cl",
									PATH"rtLights.cl",
								    PATH"rtTracer.cl",
									PATH"rtKernel.cl" });

	/*Destroi a lista de platfom_id exceto a escolhida pelo usuario*/
	/*for (int i = 0; i < platforms.size(); i++)
	{
		if (i != p)
			delete platforms[i];
	}
	platforms.clear();*/

	/*Destroi a lista de device_id exceto a escolhida pelo usuario*/
	/*for (int i = 0; i < devices.size(); i++)
	{
		if (i != d)
			delete devices[i];
	}
	devices.clear();*/

	return prog;
}

void DetailsProgram(const Program &program)
{
	std::cout << std::endl;
	displayPlatformInfo(program.platform, CL_PLATFORM_NAME, "CL_PLATFORM_NAME");

	displayDeviceDetails(program.device, CL_DEVICE_NAME, "CL_DEVICE_NAME");
	displayDeviceDetails(program.device, CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
	displayDeviceDetails(program.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE");
	std::cout << std::endl;
}

int Save(const int *image, const int w, const int h)
{
	if (!image)
		return -1;

	SDL_Surface *img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	if (!img)
		return -1;
	
	img->pixels = (cl_uint*)image;
	IMG_SavePNG(img, FILE_NAME);
	
	//SDL_FreeSurface(img);

	return 0;
}

void setScene(RT_Plane *planes)
{
	planes[0] = { { 0, -324, 0 }, {  0,  1,  0 }, { { 0.4f, 0.3f, 0.3f }, 0.2f, 0.7f, 0.5f, 25.0f } };
	planes[1] = { { 0, 0, 2419 }, {  0,  0, -1 }, { { 0.6f, 0.5f, 0.5f }, 0.2f, 1.0f, 0.8f, 25.f } };
	planes[2] = { {-1296, 0, 0 }, {  1,  0,  0 }, { { 0.5f, 0.6f, 0.6f }, 0.2f, 1.0f, 0.8f, 25.0f } };
	planes[3] = { { 1296, 0, 0 }, { -1,  0,  0 }, { { 0.5f, 0.6f, 0.6f }, 0.2f, 1.0f, 0.8f, 25.0f } };
	planes[4] = { { 0, 800, 0 },  {  0, -1,  0 },  { { 0.7f, 0.7f, 0.7f }, 0.2f, 1.0f, 0.8f, 25.0f } };
}

void printPlanes(RT_Plane *planes, int numPlanes)
{
	std::cout << std::endl;
	for (int i = 0; i < numPlanes; i++)
	{
		std::cout << "normal: " << planes[i].normal.x << " - " << planes[i].normal.y << " - "<< planes[i].normal.z << std::endl;;
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

void setSpheres(RT_Sphere *spheres)
{
	spheres[0] = { {  162,  54, 432 }, 216, { { 0.6f, 0.6f, 0.6f }, 0.2f, 0.7f, 0.8f, 1.0f} };
	spheres[1] = { { -540, -86, 432 }, 270, { { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 1.0f } };
}

void setLights(RT_Light *lights)
{
	lights[0] = { {  162, 756, 432 }, { 0.6f, 0.6f, 0.7f }, 1.0f };
	lights[1] = { { -540, 756, 432  }, { 0.5f, 0.8f, 0.8f }, 1.0f };
}

int main(int argc, char **argv)
{
	Program program = CreateProgram();
	DetailsProgram(program);
	program.BuildProgram();

	/*input*/
	//paredes
	const int numPlanes = 5;
	RT_Plane planes[numPlanes];
	setScene(planes);

	const int sizeSpheres = 2;
	RT_Sphere spheres[sizeSpheres];
	setSpheres(spheres);

	const int numBox = 1;
	RT_Box box[numBox] = { { {0, -324, 270}, {324, 162, 324},
				   { {0.7f, 0.7f, 1.0f}, 0.2f, 0.7f, 0.2f, 20.0f} } };

	/*dados do cavas*/
	RT_ViewPlane vp = { WIDTH_CANVAS, HEIGHT_CANVAS, { 1, 1 } };
	/*dados da camera*/
	/*RT_Camera camera = { { 0, 0, -270 },
						 { 0, 0, 0 },
						 { 0, 1, 0 },
						  600, 1.f, 0, 1,
						 {},{},{} };*/
	RT_Camera camera = { {-702, -108, -270},
						 {-540, -100, 0},
						 {0, 1, 0},
						  800, 1.5f, 0, 1,
						 {}, {}, {} };
	camera.computeUVW();

	/*dados da luz*/
	const int sizeLights = 2;
	RT_Light lights[sizeLights];
	setLights(lights);

	RT::Regular sp(1);
	const int sp_size = sp.GetSamples().size();
	const int shuffledIndicesSize = sp.GetShuffledIndices().size();
	/*dados da cena*/
	RT_DataScene world = { sizeLights, sizeSpheres,
						  numPlanes, numBox,
						  sp.GetNumSamples(), sp.GetNumSets(),
						  0, 0, sp.GetShuffledIndices().size(),
					      time(NULL) };

	/*output*/
	const int sizeBuffer = WIDTH_CANVAS * HEIGHT_CANVAS;
	int *bufferImage = (int*)calloc(sizeof(int), sizeBuffer);//new int[sizeBuffer];

	//const int sp_size = sp.GetSamples().size();
	//RT::Vec2f *outSp = new RT::Vec2f[sp_size];

	cl_int status;

	cl_kernel kernel = clCreateKernel(program.program, "render", &status);

	cl_mem mem_vp = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_ViewPlane), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_camera = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_Camera), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_sp = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT::Vec2f)*sp_size, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_shuffledIndices = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(cl_int)*shuffledIndicesSize, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_lights = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_Light)*sizeLights, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_planes = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_Plane)*numPlanes, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_spheres = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_Sphere)*sizeSpheres, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_boxs = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_Box)*numBox, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_dataScene = clCreateBuffer(program.context, CL_MEM_READ_ONLY, sizeof(RT_DataScene), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_bfImage = clCreateBuffer(program.context, CL_MEM_WRITE_ONLY, sizeof(int)*sizeBuffer, nullptr, &status);
	/*se houver, tratar erro*/
	//cl_mem mem_temp = clCreateBuffer(program.context, CL_MEM_READ_WRITE, sizeof(RT::Vec2f)*sp_size, nullptr, &status);

	status = clEnqueueWriteBuffer(program.queue, mem_vp, CL_TRUE, 0, sizeof(RT_ViewPlane), &vp, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_camera, CL_TRUE, 0, sizeof(RT_Camera), &camera, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_sp, CL_TRUE, 0, sizeof(RT::Vec2f)*sp_size, sp.GetSamples().data(), 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_shuffledIndices, CL_TRUE, 0, sizeof(cl_int)*shuffledIndicesSize, sp.GetShuffledIndices().data(), 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_lights, CL_TRUE, 0, sizeof(RT_Light)*sizeLights, lights, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_planes, CL_TRUE, 0, sizeof(RT_Plane)*numPlanes, planes, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_spheres, CL_TRUE, 0, sizeof(RT_Sphere)*sizeSpheres, spheres, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_boxs, CL_TRUE, 0, sizeof(RT_Box)*numBox, box, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_dataScene, CL_TRUE, 0, sizeof(RT_DataScene), &world, 0, nullptr, nullptr);
	/*se houver, tratar erro*/

	status =  clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&mem_vp);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&mem_camera);
	status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&mem_sp);
	status |= clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&mem_shuffledIndices);
	status |= clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&mem_lights);
	status |= clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&mem_planes);
	status |= clSetKernelArg(kernel, 6, sizeof(cl_mem), (void*)&mem_spheres);
	status |= clSetKernelArg(kernel, 7, sizeof(cl_mem), (void*)&mem_boxs);
	status |= clSetKernelArg(kernel, 8, sizeof(cl_mem), (void*)&mem_dataScene);
	status |= clSetKernelArg(kernel, 9, sizeof(cl_mem), (void*)&mem_bfImage);
	/*se houver, tratar erro*/

	//status = clEnqueueTask(program.queue, kernel, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	size_t paramSize;
	status = clGetDeviceInfo(program.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, nullptr, &paramSize);
	size_t *ret = new size_t[paramSize];
	status = clGetDeviceInfo(program.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, paramSize, ret, nullptr);
	size_t offSet = 0;
	size_t globalItemSize = sizeBuffer;// { WIDTH_CANVAS, HEIGHT_CANVAS };
	size_t localItemSize = *ret/2; //{ (int)sqrt((float)*ret), (int)sqrt((float)*ret) };

	std::cout << "Kernel work group size: " << localItemSize << std::endl;

	//if (globalItemSize % localItemSize != 0)
		//globalItemSize = (globalItemSize / localItemSize + 1) * localItemSize;

	std::cout << "Global groupe size: " << globalItemSize << std::endl;

	cl_event rtEnd;
	status = clEnqueueNDRangeKernel(program.queue, kernel, 1, &offSet, &globalItemSize, &localItemSize, 0, nullptr, &rtEnd);
	/*se houver, tratar erro*/
	//clFlush(program.queue);
	cl_event rbEnd;
	status = clEnqueueReadBuffer(program.queue, mem_bfImage, CL_TRUE, 0, sizeof(int)*sizeBuffer, bufferImage, 0, nullptr, &rbEnd);

	/*ulong rtStart = 0, rtKEnd = 0, rtCopyEnd = 0;
	clGetEventProfilingInfo(rtEnd, CL_PROFILING_COMMAND_QUEUED, sizeof(ulong), &rtStart, nullptr);
	clGetEventProfilingInfo(rtEnd, CL_PROFILING_COMMAND_END, sizeof(ulong), &rtKEnd, nullptr);
	clGetEventProfilingInfo(rbEnd, CL_PROFILING_COMMAND_END, sizeof(ulong), &rtCopyEnd, nullptr);

	printf("\nRaytracing: %f", ((rtKEnd - rtStart) / 1000000.0f));
	printf("\nCopy BufferImage: %f\n", ((rtCopyEnd - rtKEnd) / 1000000.0f));*/

	if (status != CL_SUCCESS)
	{
		bufferImage = nullptr;
	}

	//status = clEnqueueReadBuffer(program.queue, mem_temp, CL_TRUE, 0, sizeof(RT::Vec2f)*sp_size, outSp, 0, nullptr, nullptr);
	/*if (status != CL_SUCCESS)
	{
		delete outSp;
		outSp = nullptr;
	}
	else
	{
		for (int i = 0; i < sp_size; i++)
		{
			std::cout << outSp[i];
		}
		std::cout << std::endl;
	}*/
	/*se houver, tratar erro*/
	//std::cout << "t: " << t[0] << ", tmin: " << t[1] << std::endl;

	//printPlanes(planes2, numPlanes);
	/*std::cout << "color: " << l[0].color.x << " - " << l[0].color.y << " - " << l[0].color.z << std::endl;
	std::cout << "position: " << l[0].position.x << " - " << l[0].position.y << " - " << l[0].position.z << std::endl;
	std::cout << "position: " << l[0].ls << std::endl;
	std::cout << "color: " << l[1].color.x << " - " << l[1].color.y << " - " << l[1].color.z << std::endl;
	std::cout << "position: " << l[1].position.x << " - " << l[1].position.y << " - " << l[1].position.z << std::endl;
	std::cout << "position: " << l[1].ls << std::endl;*/

	//status = clReleaseEvent(rtEnd);
	//status |= clReleaseEvent(rbEnd);
	status = clReleaseKernel(kernel);
	status |= clReleaseMemObject(mem_vp);
	status |= clReleaseMemObject(mem_camera);
	status |= clReleaseMemObject(mem_lights);
	status |= clReleaseMemObject(mem_planes);
	status |= clReleaseMemObject(mem_spheres);
	status |= clReleaseMemObject(mem_bfImage);
	//status |= clReleaseMemObject(mem_temp);
	/*se houver, tratar erro*/

	status = Save(bufferImage, WIDTH_CANVAS, HEIGHT_CANVAS);
	if (status != CL_SUCCESS)
		std::cerr << "ERRO ao salvar a imagem\n";

	if (bufferImage)
		delete bufferImage;

	system("pause");
	return 0;
}