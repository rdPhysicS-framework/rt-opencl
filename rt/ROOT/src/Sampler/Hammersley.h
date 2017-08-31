#ifndef	 __HAMMERSLEY_H__
#define	 __HAMMERSLEY_H__

#include "Sampler.h"

namespace RT
{
	class Hammersley : public Sampler
	{
	private:
		virtual void GenerateSamples();

	public:
		Hammersley(const unsigned int numSamples);

		double Phi(int i);
	};
}

#endif //__HAMMERSLEY_H__
