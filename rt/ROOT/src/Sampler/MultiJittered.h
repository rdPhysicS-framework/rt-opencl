#ifndef __MULTIJITTERED_H__
#define __MULTIJITTERED_H__

#include "Sampler.h"

namespace RT
{
	class MultiJittered : Sampler
	{
	public:
		MultiJittered(const unsigned int numSamples = 1);
		virtual void GenerateSamples();
	};
}

#endif //__MULTIJITTERED_H__

