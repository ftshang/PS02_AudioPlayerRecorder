/*
  ==============================================================================

    Gui_record_play.h
    Created: 12 Oct 2023 1:24:55pm
    Author:  tatao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum AppState {
    IDLE,
    PLAYING,
    RECORDING
};

class DisplayAudioWaveForm : public juce::AudioVisualiserComponent
{
public:
    DisplayAudioWaveForm();
    ~DisplayAudioWaveForm() override;
    void addAudioData(const juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
};


class AudioToFileWriter
{
public:
    AudioToFileWriter();
    ~AudioToFileWriter();
    bool setup(const juce::File& outputFile, int sampleRate, int numChannels);
    void writeOutputToFile(const juce::AudioBuffer<float>& buffer);
    void closeFile();

private:
    std::unique_ptr<juce::FileOutputStream> fileStream;
    std::unique_ptr<juce::AudioFormatWriter> writer;
};