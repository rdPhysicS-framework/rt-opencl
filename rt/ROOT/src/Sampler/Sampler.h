#ifndef	 __SAMPLER_H__
#define	 __SAMPLER_H__

#include <vector>
#include <RTmath.h>

namespace RT
{
	class Sampler
	{
	protected:
		unsigned int numSamples;
		unsigned int numSets;
		unsigned long count;
		unsigned int jump;

		std::vector<RT::Vec2f> samples;
		std::vector<int> shuffledIndices;
		std::vector<RT::Vec2f> discSamples;
		std::vector<RT::Vec3f> hemisphereSamples;
		std::vector<RT::Vec3f> sphereSamples;

	public:
		Sampler(unsigned int numSamples, 
			   unsigned int numSets = 83);

		virtual ~Sampler();

		inline void SetNumSamples(const unsigned int _numSamples)
		{
			numSamples = _numSamples;
		}

		inline void SetNumSets(const unsigned int _numSets)
		{
			numSets = _numSets;
		}

		//virtual Sampler *Clone() = 0;
		virtual void GenerateSamples() = 0;

		void ShuffleXcoordinates();
		void ShuffleYcoordinates();

		void SetupShuffledIndices();
		void MapSamplesToUnitDisc();
		void MapSamplesToHemisphere(const float e);
		void MapSamplesToSphere();
	};
}

#endif //__SAMPLER_H__
