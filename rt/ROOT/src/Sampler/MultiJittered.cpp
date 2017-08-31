#include "MultiJittered.h"
#include "..\assets\Util.h"

RT::MultiJittered::MultiJittered(const unsigned int numSamples) : 
	Sampler(numSamples)
{
	GenerateSamples();
}

void RT::MultiJittered::GenerateSamples()
{
	int n = (int)sqrt((float)numSamples);
	float subcellWidth = 1.0 / (float)numSamples;

	RT::Vec2f fillPoint;
	int size = numSamples * numSets;
	for (int i = 0; i < size; i++)
		samples.push_back(fillPoint);

	/*distribute points in the initial patterns*/
	for (int p = 0; p < numSets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
				samples[i * n + j + p * numSamples].x = (i * n + j) * subcellWidth + randFloat(0, subcellWidth);
				samples[i * n + j + p * numSamples].y = (j * n + i) * subcellWidth + randFloat(0, subcellWidth);
			}

	/*shuffle x coordinates*/
	for (int p = 0; p < numSets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
				int k = randInt(j, n - 1);
				float t = samples[i * n + j + p * numSamples].x;
				samples[i * n + j + p * numSamples].x = samples[i * n + k + p * numSamples].x;
				samples[i * n + k + p * numSamples].x = t;
			}

	/*shuffle y coordinates*/
	for (int p = 0; p < numSets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
				int k = randInt(j, n - 1);
				float t = samples[j * n + i + p * numSamples].y;
				samples[j * n + i + p * numSamples].y = samples[k * n + i + p * numSamples].x;
				samples[k * n + i + p * numSamples].y = t;
			}
}
