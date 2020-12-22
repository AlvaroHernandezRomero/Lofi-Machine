/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <vector>
//==============================================================================
/**
*/
class LofimachineAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LofimachineAudioProcessor();
    ~LofimachineAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    int bit_depth;// number of bit for bitcrush
    int ratedivisor; // amount of rate division
    float wow_depth; // amount of pitch fluctuation
    float speed; // speed controler to pitch fluctuaton
    AudioProcessorValueTreeState::ParameterLayout createParameters(); //creamos esta funcion para inicializar parametros y vincularlos con los sliders
    AudioProcessorValueTreeState parametros;
private:
    //==============================================================================
    float ant_value_l, ant_value_r; // it store before samples to ratedivisor
    int count_l,count_r; //count to do rate division
    const float tambuff = 0.003; //circularbuffer size = round(samplerate*tambuff)
    std::vector<float> circularbuffer_l, circularbuffer_r; //circularbuffer to store wow modulation
    int buffersize; // maximum circularbuffer size
    int ratesample; // it store the samplerate
    float width;
    float delta; //the step for the wow sine modulator
    float samplerdivisor(int& count, float value, int div, float& valueant);
    float bitdivisor(int numbit, float value);
    int preset; // store the actual preset selected
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LofimachineAudioProcessor)
};
