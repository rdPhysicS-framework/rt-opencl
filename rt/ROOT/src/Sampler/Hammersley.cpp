#include "Hammersley.h"

RT::Hammersley::Hammersley(const unsigned int numSamples) :
	Sampler(numSamples)
{
	GenerateSamples();
}

double RT::Hammersley::Phi(int i)
{
	double x = 0.0;
	double f = 0.5;

	while (i)
	{
		x += f * (double)(i % 2);
		i /= 2;
		f *= 0.5;
	}

	return x;
}

void RT::Hammersley::GenerateSamples()
{
	for (int i = 0; i < numSets; i++)
		for (int j = 0; j < numSamples; j++)
		{
			samples.push_back(
				RT::Vec2f((float)j / 
						  (float)numSamples, 
						  Phi(j)));
		}
}
