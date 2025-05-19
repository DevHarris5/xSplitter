
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
    juce::dsp::ProcessSpec spec{ sampleRate, static_cast<uint32_t>(samplesPerBlock), 2 };
    lowPass.prepare(spec);
    highPass.prepare(spec);
    midLowPass.prepare(spec);
    midHighPass.prepare(spec);
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
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    const float lowMidFreq = apvts.getRawParameterValue("lowMidFreq")->load();
    const float midHighFreq = apvts.getRawParameterValue("midHighFreq")->load();

    lowPass.setCutoffFrequency(lowMidFreq);
    highPass.setCutoffFrequency(midHighFreq);
    midLowPass.setCutoffFrequency(midHighFreq);
    midHighPass.setCutoffFrequency(lowMidFreq);

    // Copy input into band buffers
    juce::AudioBuffer<float> lowBand(buffer), midBand(buffer), highBand(buffer);

    juce::dsp::AudioBlock<float> lowBlock(lowBand);
    juce::dsp::AudioBlock<float> midBlock(midBand);
    juce::dsp::AudioBlock<float> highBlock(highBand);

    // Apply filters
    lowPass.process(juce::dsp::ProcessContextReplacing<float>(lowBlock));   // LP for low
    highPass.process(juce::dsp::ProcessContextReplacing<float>(highBlock)); // HP for high
    midHighPass.process(juce::dsp::ProcessContextReplacing<float>(midBlock));
    midLowPass.process(juce::dsp::ProcessContextReplacing<float>(midBlock)); // Bandpass for mid

    // Apply gain and mute
    applyGainAndMute(lowBand, "gainLow", "muteLow");
    applyGainAndMute(midBand, "gainMid", "muteMid");
    applyGainAndMute(highBand, "gainHigh", "muteHigh");

    // Handle solo logic
    const bool soloLow = apvts.getRawParameterValue("soloLow")->load();
    const bool soloMid = apvts.getRawParameterValue("soloMid")->load();
    const bool soloHigh = apvts.getRawParameterValue("soloHigh")->load();
    const bool anySolo = soloLow || soloMid || soloHigh;

    // Clear the original buffer before mixing back
    buffer.clear();

    for (int ch = 0; ch < numChannels; ++ch)
    {
        if (!anySolo || soloLow)
            buffer.addFrom(ch, 0, lowBand, ch, 0, numSamples);
        if (!anySolo || soloMid)
            buffer.addFrom(ch, 0, midBand, ch, 0, numSamples);
        if (!anySolo || soloHigh)
            buffer.addFrom(ch, 0, highBand, ch, 0, numSamples);
    }
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

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThreeBandSplitterAudioProcessor();
}
