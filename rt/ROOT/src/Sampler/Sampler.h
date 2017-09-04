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

		virtual void GenerateSamples() = 0;
	public:
		Sampler(const unsigned int numSamples, 
			    const unsigned int numSets = 83);

		virtual ~Sampler();

		inline void SetNumSets(const unsigned int _numSets)
		{
			numSets = _numSets;
		}

		inline int GetNumSets() const
		{
			return numSets;
		}

		inline int GetNumSamples() const
		{
			return numSamples;
		}

		inline const std::vector<RT::Vec2f> &GetSamples() const
		{
			return samples;
		}

		inline const std::vector<int> &GetShuffledIndices() const
		{
			return shuffledIndices;
		}

		inline const std::vector<RT::Vec2f> &GetDiscSamples() const
		{
			return discSamples;
		}

		inline const std::vector<RT::Vec3f> &GetHemisphereSamples() const
		{
			return hemisphereSamples;
		}

		inline const std::vector<RT::Vec3f> &GetSphereSamples() const
		{
			return sphereSamples;
		}

		//virtual Sampler *Clone() = 0;

		void ShuffleXcoordinates();
		void ShuffleYcoordinates();

		void SetupShuffledIndices();
		void MapSamplesToUnitDisc();
		void MapSamplesToHemisphere(const float e);
		void MapSamplesToSphere();
	};
}

#endif //__SAMPLER_H__
