/*
  ==============================================================================

    ExtendedKarplus.h
    Created: 9 Nov 2022 5:14:59pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CircularBuffer.h"
namespace Karplus {
    typedef unsigned int uint32;
    typedef int int32;
    typedef unsigned long long int uint64;
    typedef long long int int64;

    class ExtendedKarplus {
    public:
      uint32 sampleRate;
      int delayLength;
      int index;
      CircularBuffer circularBuffer;
      void setSampleRate(const uint32& sampleRate);
      float process(float sample);
      float processPickDirection(float sample);
      float processPickPosition(float sample);
      float processDynamicLowPass(float sample);
      void initaliseKarplus(const float& frequency);
      void stop();

      void setParameters(int pickDirectionParam, float pickPosition, float brightnessParam, float decay, float distGain);
    
      float brightness;
      float frequencyPlaying;
      float pickDirection;
      float pickDirectionPrevSample;
      float dynamicLowPassPrevSample;
      int pickPositionInteger;
      float rho;
      float dynamicLowPassOutput;
      float delayedSampleProcessed;
      float pickDirectionProcessed;
      float pickPositionProcessed;
      bool isPLaying;
      
      
      float brightnessTemp;
      float decayTemp;
      float pickPositionTemp;
      float pickDirectionTemp;
      float distortionGain;




      std::vector <float> pickDirectionVector;
    };
}