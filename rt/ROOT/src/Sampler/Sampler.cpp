#include "Sampler.h"
#include <algorithm>

RT::Sampler::Sampler(unsigned int numSamples, 
				   unsigned int numSets) :
	numSamples(numSamples), 
	numSets(numSets),
	count(0), jump(0)
{
	samples.reserve(numSamples * numSets);
	SetupShuffledIndices();
}

RT::Sampler::~Sampler()
{
	samples.clear();
	discSamples.clear();
	hemisphereSamples.clear();
	sphereSamples.clear();
	shuffledIndices.clear();
}

void RT::Sampler::ShuffleXcoordinates()
{
	for(int i = 0; i < numSets; i++)
		for (int j = 0; j < numSamples - 1; j++)
		{
			int target = rand() % numSamples + i * numSamples;
			float temp = samples[j + i * numSamples + 1].x;
			samples[j + i * numSamples + 1].x = samples[target].x;
			samples[target].x = temp;
		}
}

void RT::Sampler::ShuffleYcoordinates()
{
	for (int i = 0; i < numSets; i++)
		for (int j = 0; j < numSamples - 1; j++)
		{
			int target = rand() % numSamples + i * numSamples;
			float temp = samples[j + i * numSamples + 1].y;
			samples[j + i * numSamples + 1].y = samples[target].y;
			samples[target].y = temp;
		}
}

void RT::Sampler::SetupShuffledIndices()
{
	shuffledIndices.reserve(numSamples * numSets);

	std::vector<int> indices;

	for (int i = 0; i < numSamples; i++)
		indices.push_back(i);

	for (int i = 0; i < numSets; i++)
	{
		std::random_shuffle(indices.begin(), indices.end());

		for (int j = 0; j < numSamples; j++)
		{
			shuffledIndices.push_back(indices[j]);
		}
	}
}

void RT::Sampler::MapSamplesToUnitDisc()
{
	int size = samples.size();

	float r, phi;
	discSamples.reserve(size);

	RT::Vec2f sp;

	for (int i = 0; i < size; i+=2)
	{
		sp.x = 2.0 * samples[i].x - 1.0; 
		sp.x = 2.0 * samples[i].y - 1.0;

		if (sp.x > -sp.y) 
		{
			if (sp.x > sp.y)
			{
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else 
			{
				r = sp.y;
				phi = 2.0 - sp.x / sp.y;
			}
		}
		else 
		{
			if (sp.x < sp.y) 
			{
				r = -sp.x;
				phi = 4.0 + sp.y / sp.x;
			}
			else 
			{
				r = -sp.y;
				if (sp.y != 0.0)
					phi = 6.0 - sp.x / sp.y;
				else
					phi = 0.0;
			}
		}

		phi *= RT_PI / 4.0;

		discSamples.push_back(RT::Vec2f(r * cos(phi), r * sin(phi)));
	}
}

void RT::Sampler::MapSamplesToHemisphere(const float e)
{
	int size = samples.size();
	hemisphereSamples.reserve(numSamples * numSets);

	for (int i = 0; i < size; i+=3)
	{
		float cosPhi = cos(2.0 * RT_PI * samples[i].x);
		float sinPhi = sin(2.0 * RT_PI * samples[i].x);
		float cosTheta = pow((1.0 - samples[i].y), 1.0 / (e + 1.0));
		float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		float u = sinTheta * cosPhi;
		float v = sinTheta * sinPhi;
		float w = cosTheta;
		hemisphereSamples.push_back(RT::Vec3f(u, v, w));
	}
}

void RT::Sampler::MapSamplesToSphere()
{
	float r1, r2;
	RT::Vec3f v;
	float r, phi;

	int size = numSamples*numSets;
	sphereSamples.reserve(size);

	for (int i = 0; i < size; i++)
	{
		r1 = samples[i].x;
		r2 = samples[i].y;

		v.z = 1.0 - 2.0 * r1;
		r = sqrt(1.0 - v.z * v.z);
		phi = RT_2PI * r2;
		v.x = r * cos(phi);
		v.y = r * sin(phi);

		sphereSamples.push_back(v);
	}
}
