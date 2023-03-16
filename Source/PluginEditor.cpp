/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthDemov2AudioProcessorEditor::SynthDemov2AudioProcessorEditor (SynthDemov2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    Decay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    Decay.setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 25);
    //Decay.setTex(" seconds");
    addAndMakeVisible(Decay);
    decayAtc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.paramTree, "decay", Decay);
    
    BrightnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    BrightnessSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 70, 25);
    BrightnessSlider.setTextValueSuffix(" seconds");
    addAndMakeVisible(BrightnessSlider);
    brightnessAtc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.paramTree, "brightness", BrightnessSlider);

    PickPosition.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PickPosition.setTextBoxStyle(juce::Slider::NoTextBox, false, 70, 25);
    PickPosition.setTextValueSuffix(" position");
    addAndMakeVisible(PickPosition);
    pickPositionAtc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.paramTree, "pickposition", PickPosition);


    distGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    distGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 70, 25);
    distGainSlider.setTextValueSuffix(" units");
    addAndMakeVisible(distGainSlider);
    distGainAtc = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.paramTree, "distgain", distGainSlider);
    
    addAndMakeVisible(PickDirectionButton);
    pickDirectionAtc = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.paramTree, "pickdirection", PickDirectionButton);

    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::whitesmoke);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::yellowgreen);

    brightnessLabel.setText("Damping", juce::NotificationType::dontSendNotification);
    brightnessLabel.setSize(100, 100);
    addAndMakeVisible(brightnessLabel);

    decayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    decayLabel.setSize(100, 100);
    addAndMakeVisible(decayLabel);

   

    pickPositionLabel.setText("Pick Position", juce::NotificationType::dontSendNotification);
    pickPositionLabel.setSize(100, 100);
    addAndMakeVisible(pickPositionLabel);

    pickDirectionLabel.setText("Direction", juce::NotificationType::dontSendNotification);
    pickDirectionLabel.setSize(100, 100);
    addAndMakeVisible(pickDirectionLabel);

    distLabel.setText("Distortion Gain", juce::NotificationType::dontSendNotification);
    distLabel.setSize(100, 100);
    addAndMakeVisible(distLabel);











    setSize (575, 275);
}

SynthDemov2AudioProcessorEditor::~SynthDemov2AudioProcessorEditor()
{
}

//==============================================================================
void SynthDemov2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    
}

void SynthDemov2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto x = getWidth() / 10;
    auto y = getHeight() / 10;

    BrightnessSlider.setBounds(0.5*x, 2.5 * y, 90, 90);
    Decay.setBounds(2.5 * x, 2.5 * y, 90, 90);
    PickPosition.setBounds(4.5 * x, 2.5 * y, 90, 90);
    PickDirectionButton.setBounds(6.75 * x, 2.5 * y, 90, 90);
    distGainSlider.setBounds(7.75 * x, 2.5 * y, 90, 90); 

   
    brightnessLabel.setBounds(0.7 * x, 4.5 * y, 100, 100);
    decayLabel.setBounds(2.8 * x, 4.5 * y, 100, 100);
    pickPositionLabel.setBounds(4.58 * x, 4.5 * y, 100, 100);
    pickDirectionLabel.setBounds(6.45 * x, 4.5 * y, 100, 100);
    distLabel.setBounds(7.75 * x, 4.5 * y, 100, 100);





}
