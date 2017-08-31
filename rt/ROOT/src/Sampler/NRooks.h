#ifndef	 __NROOKS_H__
#define	 __NROOKS_H__

#include "Sampler.h"

namespace RT
{
	class NRooks : public Sampler
	{
	private:
		virtual void GenerateSamples();

	public:
		NRooks(const unsigned int numSamples = 1);
	};
}

#endif //__NROOKS_H__
