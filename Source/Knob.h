/*
  ==============================================================================

    Knob.h
    Created: 4 Dec 2020 1:06:20pm
    Author:  Alvaro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/*
*/
class Knob : public juce::LookAndFeel_V4
{
public:
    Knob();
    ~Knob() override;
    void Knobcolor(juce::Colour bck, juce::Colour ring);
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
private:
    juce::Colour background, edge; //knob colour
};