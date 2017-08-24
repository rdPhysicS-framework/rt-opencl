#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "src\assets\Util.h"
#include "src\assets\Info.h"

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

void setSpheres(RT_Sphere *spheres)
{
	spheres[0] = { {  162,  54, 432 }, 216, { { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 1.0f} };
	spheres[1] = { { -540, -86, 432 }, 270, { { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 20.0f } };
	/*spheres[2] = { { 640, 54, 432 }, 200,{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 20.0f } };
	spheres[3] = { { 162, 300, 432 }, 150,{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 20.0f } };
	spheres[4] = { { -540, 386, 432 }, 200,{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 20.0f } };
	spheres[5] = { { 700, -300, 432 }, 150,{ { 0.7f, 0.7f, 1.0f }, 0.2f, 0.7f, 0.8f, 20.0f } };*/
}

void setLights(RT_Light *lights)
{
	lights[0] = { {  162, -756, 432 }, { 0.6f, 0.6f, 0.7f }, 1.0f };
	lights[1] = { { -540, -756, 432  }, { 0.5f, 0.8f, 0.8f }, 1.0f };
}

int main(int argc, char **argv)
{
	Program program = CreateProgram();
	DetailsProgram(program);
	program.BildProgram();

	/*input*/
	const int sizeSpheres = 2;
	RT_Sphere spheres[sizeSpheres];
	setSpheres(spheres);
	/*dados do cavas*/
	RT_ViewPlane vp = { WIDTH_CANVAS, HEIGHT_CANVAS, { 1, 1 } };
	/*dados da luz*/
	const int sizeLights = 2;
	RT_Light lights[sizeLights];
	setLights(lights);
	
	/*output*/
	const int sizeBuffer = WIDTH_CANVAS * HEIGHT_CANVAS;
	int *bufferImage = new int[sizeBuffer];
	
	cl_int status;

	cl_kernel kernel = clCreateKernel(program.program, "render2", &status);

	cl_mem mem_vp = clCreateBuffer(program.context, CL_MEM_READ_WRITE, sizeof(RT_ViewPlane), nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_lights = clCreateBuffer(program.context, CL_MEM_READ_WRITE, sizeof(RT_Light)*sizeLights, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_spheres = clCreateBuffer(program.context, CL_MEM_READ_WRITE, sizeof(RT_Sphere)*sizeSpheres, nullptr, &status);
	/*se houver, tratar erro*/
	cl_mem mem_bfImage = clCreateBuffer(program.context, CL_MEM_READ_WRITE, sizeof(int)*sizeBuffer, nullptr, &status);
	/*se houver, tratar erro*/

	status = clEnqueueWriteBuffer(program.queue, mem_vp, CL_TRUE, 0, sizeof(RT_ViewPlane), &vp, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_lights, CL_TRUE, 0, sizeof(RT_Light)*sizeLights, lights, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	status = clEnqueueWriteBuffer(program.queue, mem_spheres, CL_TRUE, 0, sizeof(RT_Sphere)*sizeSpheres, spheres, 0, nullptr, nullptr);
	/*se houver, tratar erro*/

	status  = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&mem_vp);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&mem_lights);
	status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&mem_spheres);
	status |= clSetKernelArg(kernel, 3, sizeof(const int), (void*)&sizeLights);
	status |= clSetKernelArg(kernel, 4, sizeof(const int), (void*)&sizeSpheres);
	status |= clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&mem_bfImage);
	/*se houver, tratar erro*/

	//status = clEnqueueTask(program.queue, kernel, 0, nullptr, nullptr);
	/*se houver, tratar erro*/
	size_t paramSize;
	status = clGetDeviceInfo(program.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, nullptr, &paramSize);
	size_t *ret = new size_t[paramSize];
	status = clGetDeviceInfo(program.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, paramSize, ret, nullptr);
	size_t globalItemSize = sizeBuffer;
	size_t localItemSize = 1;//*ret;

	std::cout << "Kernel work group size: " << localItemSize << std::endl;

	if (globalItemSize % localItemSize != 0)
		globalItemSize = (globalItemSize / localItemSize + 1) * localItemSize;

	std::cout << "Global groupe size: " << globalItemSize << std::endl;
	
	status = clEnqueueNDRangeKernel(program.queue, kernel, 1, nullptr, &globalItemSize, &localItemSize, 0, nullptr, nullptr);
	/*se houver, tratar erro*/

	status = clEnqueueReadBuffer(program.queue, mem_bfImage, CL_TRUE, 0, sizeof(int)*sizeBuffer, bufferImage, 0, nullptr, nullptr);
	/*se houver, tratar erro*/

	status = clReleaseKernel(kernel);
	status |= clReleaseMemObject(mem_vp);
	status |= clReleaseMemObject(mem_lights);
	status |= clReleaseMemObject(mem_spheres);
	status |= clReleaseMemObject(mem_bfImage);
	/*se houver, tratar erro*/

	status = Save(bufferImage, 1920, 1080);
	if (status != CL_SUCCESS)
		std::cerr << "ERRO ao salvar a imagem\n";

	system("pause");
	return 0;
}