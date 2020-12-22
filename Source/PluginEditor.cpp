/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Knob.h"
#include "Fader.h"
#include <cmath>

//==============================================================================
LofimachineAudioProcessorEditor::LofimachineAudioProcessorEditor (LofimachineAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 600);
    background.setSize(400, 600);
    background.setX(0);
    background.setY(0);
    juce::Colour bck = juce::Colours::darkgrey;
    juce::Colour ln = juce::Colours::lightgrey;
    //screen
    screen.setSize(250, 200);
    screen.setX(75);
    screen.setY(100);
    /*screen.setSize(400, 400);
    screen.setX(0);
    screen.setY(0);*/

    //bit depth slider
    addAndMakeVisible(num_bit);
    num_bit.setSliderStyle(juce::Slider::LinearVertical);
    num_bit.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    num_bit.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    num_bit.onValueChange = [this] {num_bit.getValue(); };
    audioProcessor.bit_depth = (int)num_bit.getValue();
    num_bit.setRange(1, 24, 1);
    bit_fader.Fadercolor(bck, ln);
    num_bit.setLookAndFeel(&bit_fader);
    num_bit.addListener(this);
    sld_bit = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parametros, "bit", num_bit);
    addAndMakeVisible(num_bit_label);
    num_bit_label.setText("BIT DEPTH", dontSendNotification);

    //rate divisor slider
    addAndMakeVisible(divisor);
    divisor.setSliderStyle(juce::Slider::LinearVertical);
    divisor.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    divisor.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    divisor.onValueChange = [this] {divisor.getValue(); };
    audioProcessor.ratedivisor = (int)divisor.getValue();
    divisor.setRange(1, 10, 1);
    div_fader.Fadercolor(bck, ln);
    divisor.setLookAndFeel(&div_fader);
    divisor.addListener(this);
    sld_div = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parametros, "ratedivisor", divisor);
    addAndMakeVisible(divisor_label);
    divisor_label.setText("RATE/DIVISOR", dontSendNotification);
    // wow slider
    addAndMakeVisible(wow);
    wow.setRange(0, 100, 1);
    wow.setSliderStyle(juce::Slider::Rotary);
    wow.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    wow.setTextValueSuffix("%");
    wow.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    wow.onValueChange = [this] {wow.getValue(); };
    audioProcessor.wow_depth = wow.getValue();
    wow_nob.Knobcolor(bck, ln);
    wow.setLookAndFeel(&wow_nob);
    wow.addListener(this);
    sld_wow = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parametros, "wow", wow);
    addAndMakeVisible(wow_label);
    wow_label.setText("WOW", dontSendNotification);

    //speed slider
    addAndMakeVisible(spd);
    spd.setSliderStyle(juce::Slider::Rotary);
    spd.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    spd.setTextValueSuffix("Hz");
    spd.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    spd.onValueChange = [this] {spd.getValue(); };
    audioProcessor.speed = (float)spd.getValue();
    speed_nob.Knobcolor(bck, ln);
    spd.setLookAndFeel(&speed_nob);
    spd.addListener(this);
    addAndMakeVisible(speed_label);
    sld_speed = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parametros, "speed", spd);
    spd.setRange(0.0f, 10.0f, 0.1f);
    speed_label.setText("SPEED", dontSendNotification);
    line.clear();
    frame();
    startTimer(50);
}

LofimachineAudioProcessorEditor::~LofimachineAudioProcessorEditor()
{
}

//==============================================================================
void LofimachineAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    paintbackground(g);
    frame();
    paintscreen(g);
}

void LofimachineAudioProcessorEditor::resized()
{
    num_bit.setBounds(306, 344, 25, 184);
    divisor.setBounds(216, 344, 25, 184);
    wow.setBounds(52, 332, 75, 75);
    spd.setBounds(116, 372, 75, 75);
    num_bit_label.setBounds(286, 320, 75, 30);
    num_bit_label.setFont(Font("OCR A",15, juce::Font::bold));
    divisor_label.setBounds(180, 320, 100, 30);
    divisor_label.setFont(Font("OCR A", 15, juce::Font::bold));
    wow_label.setBounds(72, 315, 40, 20);
    wow_label.setFont(Font("OCR A", 15, juce::Font::bold));
    speed_label.setBounds(129, 355, 60, 20);
}

void LofimachineAudioProcessorEditor::timerCallback()
{
     num_bit.setValue(audioProcessor.bit_depth, juce::sendNotification);
     divisor.setValue(audioProcessor.ratedivisor, juce::sendNotification);
     wow.setValue(audioProcessor.wow_depth, juce::sendNotification);
     spd.setValue(audioProcessor.speed, juce::sendNotification);
     repaint();
}

void LofimachineAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &num_bit) {
        num_bit.setValue(num_bit.getValue(), juce::dontSendNotification);
        audioProcessor.bit_depth = (int)num_bit.getValue();
    }
    if (slider == &divisor) {
        divisor.setValue(divisor.getValue(), juce::dontSendNotification);
        audioProcessor.ratedivisor = (int)divisor.getValue();
    }
    if (slider == &wow) {
        wow.setValue(wow.getValue(), juce::dontSendNotification);
        audioProcessor.wow_depth = wow.getValue();
    }
    if (slider == &spd) {
        spd.setValue(spd.getValue(), juce::dontSendNotification);
        audioProcessor.speed = (float)spd.getValue();
    }
}
void LofimachineAudioProcessorEditor::paintbackground(juce::Graphics& g)
{
    juce::Colour fillColour = juce::Colour(0xff00cb09);

    g.setColour(fillColour);
    g.fillRoundedRectangle(background.getX(), background.getY(), background.getWidth(), background.getHeight(), 10.0f);

}
void LofimachineAudioProcessorEditor::paintscreen(juce::Graphics& g)
{
    juce::Colour ln = juce::Colours::lightgrey;
    juce::Colour strokeColour = juce::Colours::black;
    //[UserPaintCustomArguments] Customize the painting arguments here..
    //[/UserPaintCustomArguments]
    g.setColour(strokeColour);
    g.fillRect(screen.getX()-2,screen.getY()-2,screen.getWidth()+4,screen.getHeight()+4);
    g.setColour(ln);
    g.fillRect(screen);

    
    g.setColour(juce::Colours::darkgrey);
    juce::Path p;
    int numsamples = screen.getWidth();
    p.startNewSubPath(screen.getX(), screen.getHeight());
    for (int sample = 0; sample < numsamples; sample++) {
        auto point = juce::jmap<float>(line[sample], -1.0f, 1.0f, screen.getHeight()+screen.getY(), screen.getY());
        p.lineTo(sample+ screen.getX(), point);
    }
    g.strokePath(p, juce::PathStrokeType(2));
    
}
float LofimachineAudioProcessorEditor::samplerdivisor(int& count, float value, int div, float& valueant)
{
    float y;
    if (div == 1) {
        y = value;
    }
    else {
        if (count == div) {
            valueant = value;
            count = 0;
            y = value;
        }
        else {
            count++;
            y = valueant;
        }
    }
    return y;
}
float LofimachineAudioProcessorEditor::bitdivisor(int numbit, float value)
{
    double level = pow(2, numbit);
    float step = 2 / level;
    float y = 0;
    y = step * roundf(value / step);
    if (y == 0) {
        y = step * ceilf(value / step);

        if (y == 0) {
            y = y + step;
        }
    }
    if (level == 2){
        if (value < 0) {
            y = -1;
        }
        else {
            y = 1;
        }
    }
    return y;
}
void LofimachineAudioProcessorEditor::frame() {
    int numsamples = screen.getWidth();
    float ant = 0;
    int count = divisor.getValue();
    float value;
    line.clear();
    for (int sample = 0; sample < numsamples; sample++) {  
        value = sin(2 * M_PI * sample / numsamples);
        value = samplerdivisor(count, value, divisor.getValue(), ant);
        value = bitdivisor(num_bit.getValue(), value);
        line.push_back(value);
    }
}