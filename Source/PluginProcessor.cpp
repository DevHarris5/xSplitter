
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cstdint>

ThreeBandSplitterAudioProcessor::ThreeBandSplitterAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "PARAMETERS", {
        std::make_unique<juce::AudioParameterFloat>("lowMidFreq", "Low-Mid Crossover", juce::NormalisableRange<float>(20.0f, 2000.0f), 400.0f),
        std::make_unique<juce::AudioParameterFloat>("midHighFreq", "Mid-High Crossover", juce::NormalisableRange<float>(1000.0f, 20000.0f), 2500.0f),
        std::make_unique<juce::AudioParameterFloat>("gainLow", "Low Gain", -48.0f, 12.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("gainMid", "Mid Gain", -48.0f, 12.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("gainHigh", "High Gain", -48.0f, 12.0f, 0.0f),
        std::make_unique<juce::AudioParameterBool>("muteLow", "Mute Low", false),
        std::make_unique<juce::AudioParameterBool>("muteMid", "Mute Mid", false),
        std::make_unique<juce::AudioParameterBool>("muteHigh", "Mute High", false),
        std::make_unique<juce::AudioParameterBool>("soloLow", "Solo Low", false),
        std::make_unique<juce::AudioParameterBool>("soloMid", "Solo Mid", false),
        std::make_unique<juce::AudioParameterBool>("soloHigh", "Solo High", false),
        })
{
}

ThreeBandSplitterAudioProcessor::~ThreeBandSplitterAudioProcessor() {}

const juce::String ThreeBandSplitterAudioProcessor::getName() const { return "xSplitter"; }
bool ThreeBandSplitterAudioProcessor::acceptsMidi() const { return false; }
bool ThreeBandSplitterAudioProcessor::producesMidi() const { return false; }
bool ThreeBandSplitterAudioProcessor::isMidiEffect() const { return false; }
double ThreeBandSplitterAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int ThreeBandSplitterAudioProcessor::getNumPrograms() { return 1; }
int ThreeBandSplitterAudioProcessor::getCurrentProgram() { return 0; }
void ThreeBandSplitterAudioProcessor::setCurrentProgram(int) {}
const juce::String ThreeBandSplitterAudioProcessor::getProgramName(int) { return {}; }
void ThreeBandSplitterAudioProcessor::changeProgramName(int, const juce::String&) {}


void ThreeBandSplitterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{
        sampleRate,
        static_cast<uint32_t>(samplesPerBlock),
        static_cast<uint32_t>(getTotalNumInputChannels())
    };

    lowPass.prepare(spec);
    highPass.prepare(spec);
    midLowPass.prepare(spec);
    midHighPass.prepare(spec);

    lowPass.reset();
    highPass.reset();
    midLowPass.reset();
    midHighPass.reset();
}


void ThreeBandSplitterAudioProcessor::releaseResources() {}

void ThreeBandSplitterAudioProcessor::applyGainAndMute(juce::AudioBuffer<float>& buffer, const juce::String& gainID, const juce::String& muteID)
{
    if (apvts.getRawParameterValue(muteID)->load() > 0.5f)
    {
        buffer.clear();
        return;
    }

    float gain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue(gainID)->load());
    buffer.applyGain(gain);
}



void ThreeBandSplitterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // BYPASS MODE: Just let the audio through untouched
    // This test determines if audio routing is the issue (not filters or logic)
}



bool ThreeBandSplitterAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* ThreeBandSplitterAudioProcessor::createEditor() {
    return new ThreeBandSplitterAudioProcessorEditor(*this);
}

void ThreeBandSplitterAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void ThreeBandSplitterAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) apvts.replaceState(tree);
}


bool ThreeBandSplitterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThreeBandSplitterAudioProcessor();
}
