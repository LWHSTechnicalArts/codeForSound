/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JeffTrevinoChorusFlangerAudioProcessorEditor::JeffTrevinoChorusFlangerAudioProcessorEditor (JeffTrevinoChorusFlangerAudioProcessor& p)
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
    // slider value change listener methods
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
    // slider value change listener methods
    mFeedbackSlider.onDragStart = [feedbackParameter] {feedbackParameter->beginChangeGesture();};
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue();};
    mFeedbackSlider.onDragEnd = [feedbackParameter] {feedbackParameter->endChangeGesture();};
    
    juce::AudioParameterFloat* depthParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    // slider setup
    mDepthSlider.setBounds(200, 0, 100, 100);
    mDepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDepthSlider.setRange(depthParameter->range.start, depthParameter->range.end);
    mDepthSlider.setValue(*depthParameter);
    addAndMakeVisible(mDepthSlider);
    // slider value change listener methods
    mDepthSlider.onDragStart = [depthParameter] {depthParameter->beginChangeGesture();};
    mDepthSlider.onValueChange = [this, depthParameter] { *depthParameter = mDepthSlider.getValue();};
    mDepthSlider.onDragEnd = [depthParameter] {depthParameter->endChangeGesture();};
    
    juce::AudioParameterFloat* rateParameter = (juce::AudioParameterFloat*)params.getUnchecked(3);
    // slider setup
    mRateSlider.setBounds(300, 0, 100, 100);
    mRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mRateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mRateSlider.setRange(rateParameter->range.start, rateParameter->range.end);
    mRateSlider.setValue(*rateParameter);
    addAndMakeVisible(mRateSlider);
    // slider value change listener methods
    mRateSlider.onDragStart = [rateParameter] {rateParameter->beginChangeGesture();};
    mRateSlider.onValueChange = [this, rateParameter] { *rateParameter = mRateSlider.getValue();};
    mRateSlider.onDragEnd = [rateParameter] {rateParameter->endChangeGesture();};
    
    juce::AudioParameterFloat* phaseOffsetParameter = (juce::AudioParameterFloat*)params.getUnchecked(4);
    // slider setup
    mPhaseOffsetSlider.setBounds(0, 100, 100, 100);
    mPhaseOffsetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPhaseOffsetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mPhaseOffsetSlider.setRange(phaseOffsetParameter->range.start, phaseOffsetParameter->range.end);
    mPhaseOffsetSlider.setValue(*phaseOffsetParameter);
    addAndMakeVisible(mPhaseOffsetSlider);
    // slider value change listener methods
    mPhaseOffsetSlider.onDragStart = [phaseOffsetParameter] {phaseOffsetParameter->beginChangeGesture();};
    mPhaseOffsetSlider.onValueChange = [this, phaseOffsetParameter] { *phaseOffsetParameter = mPhaseOffsetSlider.getValue();};
    mPhaseOffsetSlider.onDragEnd = [phaseOffsetParameter] {phaseOffsetParameter->endChangeGesture();};
    
    juce::AudioParameterInt* typeParameter = (juce::AudioParameterInt*)params.getUnchecked(5);
    
    mType.setBounds(100, 100, 100, 30);
    mType.addItem("Chorus", 1);
    mType.addItem("Flanger", 2);
    addAndMakeVisible(mType);
    
    mType.onChange = [this, typeParameter] {
        typeParameter->beginChangeGesture();
        *typeParameter = mType.getSelectedItemIndex();
        typeParameter->endChangeGesture();
    };
    
    mType.setSelectedItemIndex(*typeParameter);
    
}

JeffTrevinoChorusFlangerAudioProcessorEditor::~JeffTrevinoChorusFlangerAudioProcessorEditor()
{
}

//==============================================================================
void JeffTrevinoChorusFlangerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void JeffTrevinoChorusFlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
