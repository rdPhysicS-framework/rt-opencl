#ifndef __REGULAR_H__
#define __REGULAR_H__

#include "Sampler.h"

namespace RT
{
	class Regular : public Sampler
	{
	private:
		virtual void GenerateSamples();

	public:
		Regular(const unsigned int numSamples = 1);
	};

}

#endif //__REGULAR_H__

