/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JeffTrevinoDelayAudioProcessor::JeffTrevinoDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    addParameter(mDryWetParameter = new juce::AudioParameterFloat({"drywet", 1}, "Dry Wet", 0, 1.0, 0.5));
    addParameter(mFeedbackParameter = new juce::AudioParameterFloat({"feedback", 1}, "Feedback", 0, 0.98, 0.5));
    addParameter(mDelayTimeParameter = new juce::AudioParameterFloat({"delaytime", 1}, "Delay Time", 0.01, MAX_DELAY_TIME, 0.5));
    
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    mDelayTimeInSamples = 0;
    mCircularBufferReadHead = 0;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    
    mDryWet = 0.5;
}

JeffTrevinoDelayAudioProcessor::~JeffTrevinoDelayAudioProcessor()
{
    if (mCircularBufferLeft != nullptr) {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr) {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const juce::String JeffTrevinoDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JeffTrevinoDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JeffTrevinoDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JeffTrevinoDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JeffTrevinoDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JeffTrevinoDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JeffTrevinoDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JeffTrevinoDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JeffTrevinoDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void JeffTrevinoDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JeffTrevinoDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mCircularBufferWriteHead = 0;
    mCircularBufferReadHead = 0;
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    if (mCircularBufferLeft == nullptr) {
        mCircularBufferLeft = new float [mCircularBufferLength]();
    }
    
    if (mCircularBufferRight == nullptr) {
        mCircularBufferRight = new float [mCircularBufferLength]();
    }
    
}

void JeffTrevinoDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JeffTrevinoDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JeffTrevinoDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        mDelayTimeInSamples = getSampleRate() * *mDelayTimeParameter;
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        mCircularBufferReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mCircularBufferReadHead < 0) {
            mCircularBufferReadHead += mCircularBufferLength;
        }
        
        float delay_sample_left = mCircularBufferLeft[(int)mCircularBufferReadHead];
        float delay_sample_right = mCircularBufferRight[(int)mCircularBufferReadHead];
        
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;
        
        buffer.setSample(0, i, buffer.getSample(0, i) * *mDryWetParameter + delay_sample_left * (1 - *mDryWetParameter));
        buffer.setSample(1, i, buffer.getSample(1, i) * *mDryWetParameter + delay_sample_right * (1 - *mDryWetParameter));
        
        mCircularBufferWriteHead++;
        if (mCircularBufferWriteHead >= mCircularBufferLength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool JeffTrevinoDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JeffTrevinoDelayAudioProcessor::createEditor()
{
    return new JeffTrevinoDelayAudioProcessorEditor (*this);
}

//==============================================================================
void JeffTrevinoDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JeffTrevinoDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JeffTrevinoDelayAudioProcessor();
}
