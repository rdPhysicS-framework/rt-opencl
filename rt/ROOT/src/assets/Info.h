#ifndef	__INFO_H__
#define	__INFO_H__

#include "Config.h"
#include <iostream>

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


#endif//__INFO_H__