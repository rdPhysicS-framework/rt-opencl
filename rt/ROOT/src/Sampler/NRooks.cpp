#include "NRooks.h"
#include "..\assets\Util.h"

RT::NRooks::NRooks(const unsigned int numSamples) :
	Sampler(numSamples)
{
	GenerateSamples();
}

void RT::NRooks::GenerateSamples()
{
	for (int i = 0; i < numSets; i++)
	{
		for (int j = 0; j < numSamples; j++)
		{
			samples.push_back(RT::Vec2f((j + randFloat()) / numSamples,
										(j + randFloat()) / numSamples));
		}
	}
	ShuffleXcoordinates();
	ShuffleYcoordinates();
}

