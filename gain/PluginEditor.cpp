/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JeffTrevinoKadenzePlugin1AudioProcessorEditor::JeffTrevinoKadenzePlugin1AudioProcessorEditor (JeffTrevinoKadenzePlugin1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* gainParameter = (juce::AudioParameterFloat*)params.getUnchecked(0); //cast as float — we know it was a float when we defined it as that
    
    mGainControlSlider.setBounds(0, 0, 100, 100);
    mGainControlSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mGainControlSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mGainControlSlider.setRange(gainParameter->range.start, gainParameter->range.end);
    mGainControlSlider.setValue(*gainParameter);
    
    mGainControlSlider.onDragStart = [gainParameter] {
        gainParameter->beginChangeGesture();
    };
    
    mGainControlSlider.onValueChange = [this, gainParameter] {
        DBG("value changed!");
    *gainParameter = mGainControlSlider.getValue();
    };
    
    mGainControlSlider.onDragEnd = [gainParameter] {
        gainParameter->endChangeGesture();
    };
    
    
    addAndMakeVisible(mGainControlSlider);
}

JeffTrevinoKadenzePlugin1AudioProcessorEditor::~JeffTrevinoKadenzePlugin1AudioProcessorEditor()
{
}

//==============================================================================
void JeffTrevinoKadenzePlugin1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("ho!", getLocalBounds(), juce::Justification::centred, 1);
}

void JeffTrevinoKadenzePlugin1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
