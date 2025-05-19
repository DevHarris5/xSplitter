// ===============================
// PluginProcessor.cpp
// ===============================
#include "PluginProcessor.h"
#include "PluginEditor.h"

ThreeBandSplitterAudioProcessor::ThreeBandSplitterAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

void ThreeBandSplitterAudioProcessor::prepareToPlay(double, int) {}
void ThreeBandSplitterAudioProcessor::releaseResources() {}
void ThreeBandSplitterAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) {}

juce::AudioProcessorEditor* ThreeBandSplitterAudioProcessor::createEditor()
{
    return new ThreeBandSplitterAudioProcessorEditor(*this);
}

bool ThreeBandSplitterAudioProcessor::hasEditor() const { return true; }
const juce::String ThreeBandSplitterAudioProcessor::getName() const { return JucePlugin_Name; }

juce::AudioProcessorValueTreeState::ParameterLayout ThreeBandSplitterAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lowMidCrossover", "Low-Mid Crossover",
        juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f, 0.5f), 400.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "midHighCrossover", "Mid-High Crossover",
        juce::NormalisableRange<float>(1000.0f, 10000.0f, 1.0f, 0.5f), 2000.0f));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThreeBandSplitterAudioProcessor();
}
