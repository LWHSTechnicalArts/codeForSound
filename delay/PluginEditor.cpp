/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JeffTrevinoDelayAudioProcessorEditor::JeffTrevinoDelayAudioProcessorEditor (JeffTrevinoDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
    
    juce::AudioParameterFloat* dryWetParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
    
    // slider setup
    mDryWetSlider.setBounds(0, 0, 100, 100);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
    // slider valeu change listener methods
    mDryWetSlider.onDragStart = [dryWetParameter] {dryWetParameter->beginChangeGesture();};
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue();};
    mDryWetSlider.onDragEnd = [dryWetParameter] {dryWetParameter->endChangeGesture();};
    
    
    juce::AudioParameterFloat* feedbackParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
    // slider setup
    mFeedbackSlider.setBounds(100, 0, 100, 100);
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    mFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
    // slider valeu change listener methods
    mFeedbackSlider.onDragStart = [feedbackParameter] {feedbackParameter->beginChangeGesture();};
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue();};
    mFeedbackSlider.onDragEnd = [feedbackParameter] {feedbackParameter->endChangeGesture();};
    
    juce::AudioParameterFloat* delayTimeParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    // slider setup
    mDelayTimeSlider.setBounds(200, 0, 100, 100);
    mDelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDelayTimeSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end);
    mDelayTimeSlider.setValue(*delayTimeParameter);
    addAndMakeVisible(mDelayTimeSlider);
    // slider valeu change listener methods
    mDelayTimeSlider.onDragStart = [delayTimeParameter] {delayTimeParameter->beginChangeGesture();};
    mDelayTimeSlider.onValueChange = [this, delayTimeParameter] { *delayTimeParameter = mDelayTimeSlider.getValue();};
    mDelayTimeSlider.onDragEnd = [delayTimeParameter] {delayTimeParameter->endChangeGesture();};
    
}

JeffTrevinoDelayAudioProcessorEditor::~JeffTrevinoDelayAudioProcessorEditor()
{
}

//==============================================================================
void JeffTrevinoDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void JeffTrevinoDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
