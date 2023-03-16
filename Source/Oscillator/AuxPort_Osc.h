#ifndef AuxPort_OSC_H
#define AuxPort_OSC_H
#include "JuceHeader.h"
namespace AuxPort
{

	typedef unsigned int uint32;
	typedef int int32;
	typedef unsigned long long int uint64;
	typedef long long int int64;
	
	class Sawtooth
	{
	public:
		Sawtooth() = default;
		~Sawtooth() = default;
		Sawtooth(const Sawtooth& oscillator) = default;
		void setSampleRate(const uint32& sampleRate)
		{
			if (this->sampleRate != sampleRate)
			{
				this->sampleRate = sampleRate;
				stop();
			}
			
		}

		void setFrequency(const float& frequency)
		{
			indexIncrement = frequency / this->sampleRate;
		}
		juce::Random random;
		float getSample()
		{
			if (isPlaying())
			{
				
				return random.nextFloat() * 2.0f - 1.0f;
			}
		}

		void stop()
		{
			indexIncrement = 0;
			mod = 0;
		}

		bool isPlaying()
		{
			return indexIncrement != 0;
		}

	private:
		float indexIncrement = 0;
		float mod = 0;
		float sample = 0;
		uint32 sampleRate;

	};
}



#endif


