#include "Regular.h"

RT::Regular::Regular(const unsigned int numSamples) :
	Sampler(numSamples)
{
	GenerateSamples();
}

void RT::Regular::GenerateSamples()
{
	int n = (int)sqrt(numSamples);

	for (int i = 0; i < numSets; i++)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				samples.push_back(RT::Vec2f((x + 0.5) / n, (y + 0.5) / n));
			}
		}
	}
}
