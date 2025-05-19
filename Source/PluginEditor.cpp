// ===============================
// PluginEditor.cpp
// ===============================
#include "PluginEditor.h"

ThreeBandSplitterAudioProcessorEditor::ThreeBandSplitterAudioProcessorEditor(ThreeBandSplitterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    addAndMakeVisible(lowMidSlider);
    addAndMakeVisible(midHighSlider);

    lowMidSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowMidSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);

    midHighSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midHighSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);

    lowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "lowMidCrossover", lowMidSlider);

    midHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "midHighCrossover", midHighSlider);

    setSize(400, 200);
}

void ThreeBandSplitterAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("3-Band Splitter", getLocalBounds(), juce::Justification::centredTop, 1);
}

void ThreeBandSplitterAudioProcessorEditor::resized()
{
    lowMidSlider.setBounds(50, 60, 120, 120);
    midHighSlider.setBounds(230, 60, 120, 120);
}