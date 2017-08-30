#ifndef __REGULAR_H__
#define __REGULAR_H__

#include "Sampler.h"

namespace RT
{
	class Regular : public Sampler
	{
	public:
		Regular(const unsigned int numSamples = 1);

		//virtual Sampler *Clone();
		virtual void GenerateSamples();
	};

}

#endif //__REGULAR_H__

