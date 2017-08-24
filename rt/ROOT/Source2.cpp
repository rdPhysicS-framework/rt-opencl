#include "src\assets\Config.h"
#include "src\assets\Util.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

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
		std::cout << "No such option. Choose again!" << std::endl << "nOption: ";
		std::cin >> input;
	}

	return input - 1;
}

cl::Program CreateClProgram(const cl::Context &context,
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

	return cl::Program(context, source);
}

static RT_Program CreateProgram()
{
	RT_Program prog;
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	for (int i = 0; i < platforms.size(); i++)
	{
		std::cout << "\t" << i + 1 << " - " << platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
	}

	int p = Option(platforms.size());
	cl::Platform platform;
	platform = platforms[p];
	std::cout << "Using Opencl Platform:" << std::endl
			  << "\tCL_PLATFORM_NAME: " 
			  << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	std::cout << "Available OpenCL devices on this platform: " << std::endl << std::endl;
	for (int i = 0; i < devices.size(); i++) {
		std::cout << "\t" << i + 1 << ": " << devices[i].getInfo<CL_DEVICE_NAME>() << std::endl;
		std::cout << "\t\tMax compute units: " << devices[i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
		std::cout << "\t\tMax work group size: " << devices[i].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl << std::endl;
	}

	int d = Option(devices.size());
	prog.device = devices[d];

	std::cout << "Using Opencl Devices:" << std::endl
			  << "\tCL_DEVICE_NAME: "
			  << prog.device.getInfo<CL_DEVICE_NAME>() << std::endl;
	std::cout << "\tMax compute units: "
			  << prog.device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
	std::cout << "\tMax work group size: "
			  << prog.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl;

	prog.context = cl::Context(prog.device);
	prog.queue = cl::CommandQueue(prog.context, prog.device);

	prog.program = CreateClProgram(prog.context,
								  { PATH"rtUtil.cl",
								  	PATH"rtMaterial.cl",
								  	PATH"rtGeometricObject.cl",
								  	PATH"rtWorld.cl",
								  	PATH"rtLights.cl",
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

int Save(const int *image, const int w, const int h)
{
	if (!image)
		return -1;

	SDL_Surface *img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	if (!img)
		return -1;

	img->pixels = (cl_uint*)image;
	IMG_SavePNG(img, FILE_NAME);

	SDL_FreeSurface(img);

	return 0;
}

void setSpheres(RT_Sphere *spheres)
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
	RT_Program program = CreateProgram();
	program.BildProgram();

	/*input*/
	const int sizeSpheres = 2;
	RT_Sphere in[sizeSpheres];
	setSpheres(in);

	/*output*/
	const int sizeBuffer = WIDTH_CANVAS * HEIGHT_CANVAS;
	int *bufferImage = new int[sizeBuffer];

	cl::Kernel kernel = cl::Kernel(program.program, "render");
	
	cl::Buffer input = cl::Buffer(program.context, CL_MEM_READ_WRITE, sizeof(RT_Sphere)*sizeSpheres);
	cl::Buffer output = cl::Buffer(program.context, CL_MEM_READ_WRITE, sizeof(int)*sizeBuffer);
	program.queue.enqueueWriteBuffer(input, CL_TRUE, 0, sizeof(RT_Sphere)*sizeSpheres, in);

	kernel.setArg(0, input);
	kernel.setArg(1, sizeSpheres);
	kernel.setArg(2, output);

	size_t globalItemSize = sizeBuffer;
	size_t localItemSize = 1;//kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(program.device);

	std::cout << "Kernel work group size: " << localItemSize << std::endl;

	//if (globalItemSize % localItemSize != 0)
	//	globalItemSize = (globalItemSize / localItemSize + 1) * localItemSize;

	std::cout << "Rendering started..." << std::endl;

	program.queue.enqueueNDRangeKernel(kernel, NULL, globalItemSize, localItemSize);
	program.queue.finish();
	
	program.queue.enqueueReadBuffer(output, CL_TRUE, 0, sizeof(int) * sizeBuffer, bufferImage);
	
	cl_int status = Save(bufferImage, WIDTH_CANVAS, HEIGHT_CANVAS);
	if (status != CL_SUCCESS)
		std::cerr << "ERRO ao salvar a imagem\n";

	delete bufferImage;
	system("pause");
	return 0;
}