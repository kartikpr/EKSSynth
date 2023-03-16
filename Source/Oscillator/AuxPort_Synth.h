#ifndef AuxPort_Synth_H
#define AuxPort_Synth_H
#include "AuxPort_Osc.h"
#include <vector>
#include "JuceHeader.h"
#include "../ExtendedKarplus.h"
namespace AuxPort
{
	
	class Synth
	{
	public:
		Synth() = default;
		~Synth() = default;
		Synth(const Synth& synth) = default;

		void prepareToPlay(const uint32& sampleRate)
		{
			sawToothOscillators.resize(128);
			extendedKarplus.resize(128);
			if (this->sampleRate != sampleRate)
			{
				for (uint32 i = 0; i < sawToothOscillators.size(); i++)
				{
					sawToothOscillators[i].setSampleRate(sampleRate);
					extendedKarplus[i].setSampleRate(sampleRate);

				}
				this->sampleRate = sampleRate;
			}
		}

		void setParameters(const int& pickDirectionParam, const float& pickPosition, const float& brightnessParam, const float& decay, const float& distGain)
		{
			for (int i = 0;i < extendedKarplus.size();i++)
			{
				extendedKarplus[i].setParameters(pickDirectionParam, pickPosition, brightnessParam, decay, distGain);
			}
		}

		void render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& messages)
		{
			auto currentSample = 0;
			for (const auto midi : messages)
			{
				auto message = midi.getMessage();
				auto messagePosition = static_cast<int> (message.getTimeStamp());
				process(buffer, currentSample, messagePosition);
				currentSample = messagePosition;
				handleMidiEvent(message);
			}
			process(buffer, currentSample, buffer.getNumSamples());
		}
	private:
		float midiToFreq(const uint32& midiNote)
		{
			constexpr float A4_FREQ = 440;
			constexpr float A4_MIDINOTE = 69;
			constexpr float NOTES_IN_OCTAVE = 12.f;
			return A4_FREQ * std::powf(2, (static_cast<float>(midiNote) - A4_MIDINOTE) / NOTES_IN_OCTAVE);
		}

		void handleMidiEvent(const juce::MidiMessage& message)
		{
			if (message.isNoteOn())
			{
				sawToothOscillators[message.getNoteNumber()].setFrequency(midiToFreq(message.getNoteNumber()));
				extendedKarplus[message.getNoteNumber()].initaliseKarplus(midiToFreq(message.getNoteNumber()));
			}

			if (message.isNoteOff())
			{
				sawToothOscillators[message.getNoteNumber()].stop();
				extendedKarplus[message.getNoteNumber()].stop();

			}

			if (message.isAllNotesOff())
			{
				for (uint32_t i = 0; i < sawToothOscillators.size(); i++)
				{
					sawToothOscillators[i].stop();
				}
			}
		}

		void process(juce::AudioBuffer<float>& buffer, const uint32& startSample, const uint32& endSample)
		{
			auto* firstChannel = buffer.getWritePointer(0);
			for (uint32 i = 0; i < sawToothOscillators.size(); i++)
			{
				if (sawToothOscillators[i].isPlaying())
				{
					for (auto j = startSample; j < endSample; j++)
					{
						firstChannel[j] += extendedKarplus[i].process(0.5);
					}
				}
			}

			for (uint32 channel = 1; channel < buffer.getNumChannels(); channel++)
			{
				auto channelData = buffer.getWritePointer(channel);
				std::copy(firstChannel + startSample, firstChannel + endSample, channelData + startSample);
			}
		}


		uint32 sampleRate = 44100;
		std::vector<AuxPort::Sawtooth> sawToothOscillators;
		std::vector<Karplus::ExtendedKarplus> extendedKarplus;
		juce::Random random;
	};
	
}
#endif