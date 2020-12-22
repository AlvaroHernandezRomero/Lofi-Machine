/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knob.h"
#include "Fader.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


//==============================================================================
/**
*/
class LofimachineAudioProcessorEditor  : public juce::AudioProcessorEditor , private Slider::Listener, private Timer
{
public:
    LofimachineAudioProcessorEditor (LofimachineAudioProcessor&);
    ~LofimachineAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback();
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void frame();// to update the screen
    juce::Rectangle<int> background; //background Gui
    juce::Rectangle<int> screen; //screen Gui
    LofimachineAudioProcessor& audioProcessor;
    Slider divisor, num_bit, wow, spd;
    Label divisor_label, num_bit_label, wow_label, speed_label;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_div;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_bit;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_speed;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sld_wow;
    Knob wow_nob, speed_nob;
    Fader div_fader, bit_fader;
    void sliderValueChanged(juce::Slider* slider);
    void paintbackground(juce::Graphics&);
    void paintscreen(juce::Graphics&);
    float samplerdivisor(int& count, float value, int div, float& valueant );
    float bitdivisor(int numbit, float value);
    std::vector<float> line;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LofimachineAudioProcessorEditor)
};
