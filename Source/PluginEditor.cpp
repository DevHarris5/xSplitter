
#include "PluginProcessor.h"
#include "PluginEditor.h"

ThreeBandSplitterAudioProcessorEditor::ThreeBandSplitterAudioProcessorEditor(ThreeBandSplitterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 300);

    // Crossover sliders
    addAndMakeVisible(lowMidSlider);
    lowMidSlider.setSliderStyle(juce::Slider::Rotary);
    lowMidSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    addAndMakeVisible(midHighSlider);
    midHighSlider.setSliderStyle(juce::Slider::Rotary);
    midHighSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    lowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "lowMidFreq", lowMidSlider);
    midHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "midHighFreq", midHighSlider);

    // Gain sliders
    auto addGainSlider = [this](juce::Slider& slider, const juce::String& paramId) {
        addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        };

    addGainSlider(lowGainSlider, "gainLow");
    addGainSlider(midGainSlider, "gainMid");
    addGainSlider(highGainSlider, "gainHigh");

    lowGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "gainLow", lowGainSlider);
    midGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "gainMid", midGainSlider);
    highGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "gainHigh", highGainSlider);

    // Mute buttons
    addAndMakeVisible(muteLowButton);
    muteLowButton.setButtonText("Mute Low");
    muteLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "muteLow", muteLowButton);

    addAndMakeVisible(muteMidButton);
    muteMidButton.setButtonText("Mute Mid");
    muteMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "muteMid", muteMidButton);

    addAndMakeVisible(muteHighButton);
    muteHighButton.setButtonText("Mute High");
    muteHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "muteHigh", muteHighButton);

    // Solo buttons
    addAndMakeVisible(soloLowButton);
    soloLowButton.setButtonText("Solo Low");
    soloLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "soloLow", soloLowButton);

    addAndMakeVisible(soloMidButton);
    soloMidButton.setButtonText("Solo Mid");
    soloMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "soloMid", soloMidButton);

    addAndMakeVisible(soloHighButton);
    soloHighButton.setButtonText("Solo High");
    soloHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "soloHigh", soloHighButton);
}

ThreeBandSplitterAudioProcessorEditor::~ThreeBandSplitterAudioProcessorEditor() = default;

void ThreeBandSplitterAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Three Band Splitter", getLocalBounds(), juce::Justification::centredTop, 1);
}

void ThreeBandSplitterAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto topRow = area.removeFromTop(100);
    auto bottomRow = area.removeFromBottom(100);
    auto columnWidth = getWidth() / 3;

    lowMidSlider.setBounds(10, 10, 80, 80);
    midHighSlider.setBounds(100, 10, 80, 80);

    // Gain sliders
    lowGainSlider.setBounds(20, 110, 60, 100);
    midGainSlider.setBounds(260, 110, 60, 100);
    highGainSlider.setBounds(500, 110, 60, 100);

    // Mute buttons
    muteLowButton.setBounds(20, 220, 80, 20);
    muteMidButton.setBounds(260, 220, 80, 20);
    muteHighButton.setBounds(500, 220, 80, 20);

    // Solo buttons
    soloLowButton.setBounds(20, 245, 80, 20);
    soloMidButton.setBounds(260, 245, 80, 20);
    soloHighButton.setBounds(500, 245, 80, 20);
}
