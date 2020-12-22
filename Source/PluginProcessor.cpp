/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
#include <vector>
//==============================================================================
LofimachineAudioProcessor::LofimachineAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parametros(*this, nullptr, "EstadoSliders", createParameters())
#endif
{
    parametros.state = ValueTree("EstadoSliders");
    ratedivisor = 1;
    bit_depth = 24;
    wow_depth = 0;
    speed = 0;
    count_l = ratedivisor;
    count_r = count_l;
    ant_value_l = 0;
    ant_value_r = 0;
    ratesample = 0;
    buffersize = 0;
    circularbuffer_l.clear();
    circularbuffer_r.clear();
    delta = 0;
    preset = 0;
}

LofimachineAudioProcessor::~LofimachineAudioProcessor()
{

}

//==============================================================================
const juce::String LofimachineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LofimachineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LofimachineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LofimachineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LofimachineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LofimachineAudioProcessor::getNumPrograms()
{
    return 9;   
}

int LofimachineAudioProcessor::getCurrentProgram()
{
       return preset;
}

void LofimachineAudioProcessor::setCurrentProgram (int index)
{
    preset = index;
    switch (index)
    {
    case 0:
        ratedivisor = 1;
        bit_depth = 24;
        wow_depth = 0;
        speed = 0;
        break;
    case 1:
        ratedivisor = 8;
        bit_depth = 24;
        wow_depth = 0;
        speed = 0;
        break;
    case 2:
        ratedivisor = 6;
        bit_depth = 7;
        wow_depth = 0;
        speed = 0;
        break;
    case 3:
        ratedivisor = 1;
        bit_depth = 24;
        wow_depth = 100;
        speed = 1.10;
        break;
    case 4:
        ratedivisor = 6;
        bit_depth = 24;
        wow_depth = 0;
        speed = 0;
        break;
    case 5:
        ratedivisor = 10;
        bit_depth = 5;
        wow_depth = 100;
        speed = 2.30;
        break;
    case 6:
        ratedivisor = 4;
        bit_depth = 8;
        wow_depth = 74;
        speed = 0.7;
        break;
    case 7:
        ratedivisor = 6;
        bit_depth = 24;
        wow_depth = 100;
        speed = 1.10;
        break;
    case 8:
        ratedivisor = 1;
        bit_depth = 5;
        wow_depth = 0;
        speed = 0;
        break;
    }
}

const juce::String LofimachineAudioProcessor::getProgramName (int index)
{
    juce::String name;
    switch (index)
    {
    case 0:
        name = "High-Quality";
        break;
    case 1:
        name = "Gritty";
        break;
    case 2:
        name = "Dirty";
        break;
    case 3:
        name = "Vinyl Sim";
        break;
    case 4:
        name = "Raw";
        break;
    case 5:
        name = "Bad Fidelity";
        break;
    case 6:
        name = "Golden Era";
        break;
    case 7:
        name = "Lofi Sound";
        break;
    case 8:
        name = "Crunch";
        break;
    }
    return {name};
}

void LofimachineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LofimachineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    width = (int)round(tambuff * sampleRate);
    buffersize = 2 + width + width * 2;
    ratesample = sampleRate;
    count_l = ratedivisor;
    count_r = count_l;
    circularbuffer_l.resize(buffersize,0);
    circularbuffer_r.resize(buffersize,0);
}

void LofimachineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LofimachineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void LofimachineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float gain = wow_depth / 100;
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float modfreq = speed / ratesample;
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {

        if (delta >= 1) {
            delta = 0;
        }
        float mod = sin(delta * 2 * M_PI );
        delta = delta + modfreq;
        float zeiger = 1 + width + width * mod*gain;
        int i = (int)floor(zeiger);
        auto frac = zeiger - i;
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer(channel);
            if (channel == 0) {
                circularbuffer_l.resize(buffersize - 1);
                circularbuffer_l.insert(begin(circularbuffer_l), channelData[sample]);
                float out = 0;
                out = circularbuffer_l[i + 1] *frac + circularbuffer_l[i] * (1 - frac);
                out = samplerdivisor(count_l, out, ratedivisor, ant_value_l);
                if (out != 0) {
                    channelData[sample] = bitdivisor(bit_depth, out);
                }
                else {
                    channelData[sample] = out;
                }
                
            }
            if (channel == 1) {
                circularbuffer_r.resize(buffersize - 1);
                circularbuffer_r.insert(begin(circularbuffer_r), channelData[sample]);
                float out = 0;
                out = circularbuffer_r[i + 1] *frac + circularbuffer_r[i] * (1 - frac);
                out = samplerdivisor(count_r, out, ratedivisor, ant_value_r);
                if (out != 0) {
                    channelData[sample] = bitdivisor(bit_depth, out);
                }
                else {
                    channelData[sample] = out;
                }
                
            }
        }
    }
}

float LofimachineAudioProcessor::samplerdivisor(int& count, float value, int div, float& valueant)
{
    float y;
    if (div == 1) {
        y = value;
        count = 0;
        valueant = value;
    }
    else {
        if (count == div) {
            valueant = value;
            count = 0;
            y = value;
        }
        else {
            count++;
            if (count > div) {
                count = 0;
            }
            y = valueant;
        }
    }
    return y;
}
float LofimachineAudioProcessor::bitdivisor(int numbit, float value)
{
    double level = pow(2, numbit);
    float step = 2 / level;
    float y = 0;
    y = step * roundf(value / step);
    if (y == 0) {
        y = step * ceil(value / step);

        if (y == 0) {
            y = y + step;
        }
    }
    if (level == 2) {
        if (value < 0) {
            y = -1;
        }
        else {
            y = 1;
        }
    }
    return y;
}
//==============================================================================
bool LofimachineAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LofimachineAudioProcessor::createEditor()
{
    return new LofimachineAudioProcessorEditor (*this);
}

//==============================================================================
void LofimachineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<XmlElement> xml(parametros.copyState().createXml());
    copyXmlToBinary(*xml, destData);
}

void LofimachineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> params(getXmlFromBinary(data, sizeInBytes));
    if (params != nullptr)
    {
        if (params->hasTagName(parametros.copyState().getType())) {
            parametros.state = ValueTree::fromXml(*params);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessorValueTreeState::ParameterLayout
LofimachineAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<AudioParameterInt>("bit", "Bit", 1, 24, 24));
    parameters.push_back(std::make_unique<AudioParameterInt>("ratedivisor", "Ratedivisor", 1, 10, 1));
    parameters.push_back(std::make_unique<AudioParameterInt>("wow", "Wow", 0, 100, 0));
    parameters.push_back(std::make_unique<AudioParameterFloat>("speed", "Speed", 0.0f, 10.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterInt>("preset","Preset",0,8,0));
    return { parameters.begin(), parameters.end() };
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LofimachineAudioProcessor();
}
