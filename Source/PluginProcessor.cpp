/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthDemov2AudioProcessor::SynthDemov2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), paramTree(*this, nullptr, "parameters", createParameters())
#endif
{
}

SynthDemov2AudioProcessor::~SynthDemov2AudioProcessor()
{
}

//==============================================================================
const juce::String SynthDemov2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthDemov2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthDemov2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthDemov2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthDemov2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthDemov2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthDemov2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthDemov2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthDemov2AudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthDemov2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthDemov2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.prepareToPlay(sampleRate);

    
}

void SynthDemov2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthDemov2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthDemov2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto& pickPosition = *paramTree.getRawParameterValue("pickposition");
    auto& pickDirection = *paramTree.getRawParameterValue("pickdirection");
    auto& decay = *paramTree.getRawParameterValue("decay");
    auto& brightness = *paramTree.getRawParameterValue("brightness");
    auto& distGain = *paramTree.getRawParameterValue("distgain");


 
    synth.setParameters(pickDirection, pickPosition, brightness, decay, distGain);


    synth.render(buffer, midiMessages);
}



juce::AudioProcessorValueTreeState::ParameterLayout SynthDemov2AudioProcessor::createParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("pickposition", "PickPosition", juce::NormalisableRange<float>{ 0.1f, 1.0f, 0.1f}, 0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float>{ 0.1f, 5.0f, 0.1f}, 2.0f));

   params.push_back(std::make_unique<juce::AudioParameterFloat>("brightness", "Brighntess", juce::NormalisableRange<float>{ 0.0f, 1.0f, 0.1f}, 0.5f));

   params.push_back(std::make_unique<juce::AudioParameterFloat>("distgain", "DistGain", juce::NormalisableRange<float>{ 0.1f, 8.0f, 0.1f}, 1.0f));


    auto attributes = juce::AudioParameterBoolAttributes().withStringFromValueFunction([](auto x, auto) { return x ? "On" : "Off"; }).withLabel("enabled");
    params.push_back(std::make_unique<juce::AudioParameterBool>("pickdirection", "PickDirection", false, attributes));

    return { params.begin(), params.end() };


}


























//==============================================================================
bool SynthDemov2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthDemov2AudioProcessor::createEditor()
{
    return new SynthDemov2AudioProcessorEditor (*this);
}

//==============================================================================
void SynthDemov2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthDemov2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthDemov2AudioProcessor();
}
