#include "MultiJittered.h"

RT::MultiJittered::MultiJittered(const unsigned int numSamples) : 
	Sampler(numSamples)
{
	GenerateSamples();
}

void RT::MultiJittered::GenerateSamples()
{
}
