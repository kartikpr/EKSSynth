/*
  ==============================================================================

    ExtendedKarplus.cpp
    Created: 9 Nov 2022 5:14:59pm
    Author:  user

  ==============================================================================
*/

#include "ExtendedKarplus.h"

void Karplus::ExtendedKarplus::setSampleRate(const uint32& rate) {
    sampleRate = rate;
}

void Karplus::ExtendedKarplus::initaliseKarplus(const float& frequency) {

    
    if (!isPLaying) {
        index = 0;
        frequencyPlaying = frequency;
        float dividerFreq = sampleRate / frequency;
        delayLength = juce::roundFloatToInt(dividerFreq);
        circularBuffer.initialseDelay(delayLength);

        pickDirectionVector.resize(delayLength);
        std::fill(pickDirectionVector.begin(), pickDirectionVector.end(), 0);

        pickDirection = pickDirectionTemp;
        pickPositionInteger = juce::roundFloatToInt(delayLength * pickPositionTemp);
        rho = pow(0.001, 1.0 / (frequency * decayTemp));
        brightness = brightnessTemp;
        dynamicLowPassOutput = 0;
        delayedSampleProcessed = 0;
        pickDirectionProcessed = 0;
        pickPositionProcessed = 0;
        pickDirectionPrevSample = 0;
        dynamicLowPassPrevSample = 0;
        isPLaying = true;

        
        
    }


}
    float Karplus::ExtendedKarplus::process(float sample) {
        if (index < delayLength ) {
            
             pickDirectionProcessed = processPickDirection(sample);
             pickPositionProcessed = processPickPosition(pickDirectionProcessed);

            delayedSampleProcessed = circularBuffer.processSample(pickPositionProcessed + dynamicLowPassOutput);
            dynamicLowPassOutput = processDynamicLowPass(delayedSampleProcessed);
            index++;
        }
         else if (index >= delayLength) {

             delayedSampleProcessed = circularBuffer.processSample(dynamicLowPassOutput);
            dynamicLowPassOutput = processDynamicLowPass(delayedSampleProcessed);

        }

        float delayedDistortedSampleProcessed = delayedSampleProcessed * distortionGain;

        if (delayedDistortedSampleProcessed >= 1) {
            delayedDistortedSampleProcessed = 2.0f / 3.0f;
        }
        else if (delayedDistortedSampleProcessed <= 1 && delayedDistortedSampleProcessed >= -1)
        {
            delayedDistortedSampleProcessed = delayedDistortedSampleProcessed - (pow(delayedDistortedSampleProcessed, 3.0f) / 3);
        }
        else if (delayedDistortedSampleProcessed <= -1) {
            delayedDistortedSampleProcessed = -2.0f / 3.0f;
        }
      
        return delayedDistortedSampleProcessed;
        


 }

    float Karplus::ExtendedKarplus::processPickDirection(float sample) {
        float pickDirectionSample = sample * (1 - pickDirection) + (pickDirection * pickDirectionPrevSample);
        pickDirectionPrevSample = pickDirectionSample;
        pickDirectionVector[index] = pickDirectionSample;
        return pickDirectionSample;
    }

    float Karplus::ExtendedKarplus::processPickPosition(float sample) {
        float pickPostionSample;
        if (index <= pickPositionInteger)
            pickPostionSample = sample;
        else if (index > pickPositionInteger)
            pickPostionSample = sample - pickDirectionVector[index - pickPositionInteger];
        return pickPostionSample;
    }

    float Karplus::ExtendedKarplus::processDynamicLowPass(float sample) {
      
            float b1 = 0.5f * brightness;
            float b0 = 1.0f - b1;
        float dynamicLowPassSample = rho * ((b0 * sample) + (b1 * dynamicLowPassPrevSample));
        dynamicLowPassPrevSample = sample;
        return dynamicLowPassSample;
    }

    void Karplus::ExtendedKarplus::setParameters(int pickDirectionParam, float pickPosition, float brightnessParam, float decay, float distGain) {
        if (pickDirectionParam == 0) {
            pickDirectionTemp = 0.0f;
        }
        else if (pickDirectionParam == 1) {
            pickDirectionTemp = 0.9f;
        }

        pickPositionTemp =  pickPosition;

        brightnessTemp = brightnessParam;

        decayTemp = decay;

        distortionGain = distGain;



    }

    void Karplus::ExtendedKarplus::stop() {
          isPLaying = false;
    }
