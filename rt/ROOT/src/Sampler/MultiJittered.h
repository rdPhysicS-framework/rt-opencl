#ifndef __MULTIJITTERED_H__
#define __MULTIJITTERED_H__

#include "Sampler.h"

namespace RT
{
	class MultiJittered : public Sampler
	{
	private:
		virtual void GenerateSamples();

	public:
		MultiJittered(const unsigned int numSamples = 1);
	};
}

#endif //__MULTIJITTERED_H__

