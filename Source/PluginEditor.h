/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthDemov2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthDemov2AudioProcessorEditor (SynthDemov2AudioProcessor&);
    ~SynthDemov2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider BrightnessSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> brightnessAtc;

    juce::Slider Decay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAtc;

    juce::Slider PickPosition;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pickPositionAtc;

    juce::ToggleButton PickDirectionButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pickDirectionAtc;

    juce::Slider distGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distGainAtc;

    juce::Label decayLabel;
    juce::Label brightnessLabel;
    juce::Label pickPositionLabel;
    juce::Label depthLabel;
    juce::Label pickDirectionLabel;
    juce::Label distLabel;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthDemov2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthDemov2AudioProcessorEditor)
};
