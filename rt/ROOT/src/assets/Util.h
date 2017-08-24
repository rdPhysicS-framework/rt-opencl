#ifndef	__UTIL_H__
#define	__UTIL_H__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

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

#endif//__UTIL_H__ 