
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
    
    juce::Label lowMidLabel;
    juce::Label midHighLabel;

    juce::Slider lowMidSlider, midHighSlider;
    juce::Slider lowGainSlider, midGainSlider, highGainSlider;
    juce::ToggleButton lowMuteButton, midMuteButton, highMuteButton;
    juce::ToggleButton lowSoloButton, midSoloButton, highSoloButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midHighAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highGainAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowMuteAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midMuteAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highMuteAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowSoloAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midSoloAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highSoloAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreeBandSplitterAudioProcessorEditor)
};
