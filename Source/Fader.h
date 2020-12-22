/*
  ==============================================================================

    Fader.h
    Created: 4 Dec 2020 1:06:30pm
    Author:  Alvaro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//==============================================================================
/*
*/
class Fader : public juce::LookAndFeel_V4
{
public:
    Fader();
    ~Fader() override;
    void Fadercolor(juce::Colour bck, juce::Colour ln);
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle Linear, juce::Slider&) override;
    void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float  sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle Linear, juce::Slider&) override;

private:
    juce::Colour background, line; //Fader colour
};