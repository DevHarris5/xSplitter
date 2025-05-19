// ===============================
// PluginEditor.h
// ===============================
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ThreeBandSplitterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ThreeBandSplitterAudioProcessorEditor(ThreeBandSplitterAudioProcessor&);
    ~ThreeBandSplitterAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ThreeBandSplitterAudioProcessor& audioProcessor;

    juce::Slider lowMidSlider;
    juce::Slider midHighSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midHighAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreeBandSplitterAudioProcessorEditor)
};