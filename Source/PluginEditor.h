
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ThreeBandSplitterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ThreeBandSplitterAudioProcessorEditor(ThreeBandSplitterAudioProcessor&);
    ~ThreeBandSplitterAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ThreeBandSplitterAudioProcessor& audioProcessor;

    juce::Label lowMidLabel, midHighLabel;

    juce::Slider lowMidSlider, midHighSlider;
    juce::Slider lowGainSlider, midGainSlider, highGainSlider;

    juce::ToggleButton muteLowButton, muteMidButton, muteHighButton;
    juce::ToggleButton soloLowButton, soloMidButton, soloHighButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midHighAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highGainAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteLowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteHighAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloLowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> soloHighAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreeBandSplitterAudioProcessorEditor)
};
