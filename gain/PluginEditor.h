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
class JeffTrevinoKadenzePlugin1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JeffTrevinoKadenzePlugin1AudioProcessorEditor (JeffTrevinoKadenzePlugin1AudioProcessor&);
    ~JeffTrevinoKadenzePlugin1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JeffTrevinoKadenzePlugin1AudioProcessor& audioProcessor;
    juce::Slider mGainControlSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JeffTrevinoKadenzePlugin1AudioProcessorEditor)
};
