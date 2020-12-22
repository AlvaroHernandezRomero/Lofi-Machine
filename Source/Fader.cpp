/*
  ==============================================================================

    Fader.cpp
    Created: 4 Dec 2020 1:06:30pm
    Author:  Alvaro

  ==============================================================================
*/

#include "Fader.h"
#include <JuceHeader.h>

Fader::Fader()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Fader::~Fader()
{
}
void Fader::Fadercolor(juce::Colour bck, juce::Colour ln)
{
    background = bck;
    line = ln;

}
void Fader::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle Linear, juce::Slider& slider) {

    g.setColour(juce::Colours::black);
    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, Linear, slider);
    drawLinearSliderThumb(g, x, y, width, width / 2, sliderPos, minSliderPos, maxSliderPos, Linear, slider);
}
void Fader::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float  sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle Linear, juce::Slider&) {
    juce::Path p, s;
    g.setColour(background);
    p.addRectangle(x, y - (height + height / 2), width, height);
    p.applyTransform(juce::AffineTransform::translation(x, sliderPos));
    g.fillPath(p);
    g.setColour(background);
    s.addRectangle(x + width / 4, y - (height * 1.15), width / 2, height / 4);
    s.applyTransform(juce::AffineTransform::translation(x, sliderPos));
    g.setColour(line);
    g.fillPath(s);
}