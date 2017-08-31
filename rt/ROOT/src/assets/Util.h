#ifndef	__UTIL_H__
#define	__UTIL_H__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

inline void randSeed(const int seed)
{
	srand(seed);
}

inline float randFloat()
{
	return (float)rand() / RAND_MAX;
}

inline float randFloat(int l, float h)
{
	return randFloat() * (h - l) + l;
}

inline int randInt(int l, int h)
{
	return (int)(randFloat(0, h - l + 1) + l);
}

#endif//__UTIL_H__ 