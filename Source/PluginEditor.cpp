#include "PluginProcessor.h"
#include "PluginEditor.h"

ThreeBandSplitterAudioProcessorEditor::ThreeBandSplitterAudioProcessorEditor(ThreeBandSplitterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);

    addAndMakeVisible(lowMidSlider);
    lowMidSlider.setSliderStyle(juce::Slider::Rotary);
    lowMidSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    addAndMakeVisible(midHighSlider);
    midHighSlider.setSliderStyle(juce::Slider::Rotary);
    midHighSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    addAndMakeVisible(lowMidLabel);
    lowMidLabel.setText("Low-Mid", juce::dontSendNotification);
    lowMidLabel.attachToComponent(&lowMidSlider, false);

    addAndMakeVisible(midHighLabel);
    midHighLabel.setText("Mid-High", juce::dontSendNotification);
    midHighLabel.attachToComponent(&midHighSlider, false);

    lowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "lowMidFreq", lowMidSlider);

    midHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "midHighFreq", midHighSlider);
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
    auto area = getLocalBounds().reduced(20);
    auto width = area.getWidth() / 2;

    lowMidSlider.setBounds(area.removeFromLeft(width));
    midHighSlider.setBounds(area);
}
