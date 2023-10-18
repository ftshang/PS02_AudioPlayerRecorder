/*
  ==============================================================================

    Gui_record_play.cpp
    Created: 12 Oct 2023 1:24:55pm
    Author:  tatao

  ==============================================================================
*/

#include "Gui_record_play.h"

AudioToFileWriter::AudioToFileWriter()
{
    DBG("IN AUDIOTOFILEWRITER CONSTRUCTOR");
}

AudioToFileWriter::~AudioToFileWriter()
{
    DBG("In AUDIOTOFILEWRITER DESTRUCTOR");
    writer.reset();
    DBG("WRITER RESET");
    //fileStream.reset();
    //DBG("FILESTREAM RESET");
    writer.release();
    DBG("WRITER RELEASED");
    fileStream.release();
    DBG("FILESTREAM RELEASED");
}

bool AudioToFileWriter::setup(const juce::File& outputFile, int sampleRate, int numChannels)
{
    if (outputFile.getFileNameWithoutExtension() == "")
    {
        DBG("no file name");
        return false;
    }
    // If the output file exists, delete it and create a new one.
    if (outputFile.exists())
    {
        outputFile.deleteFile();
    }

    outputFile.create();

    // I. Creates a file stream to which the writer will place audio data.
    fileStream = outputFile.createOutputStream();

    if (fileStream != nullptr)
    {
        DBG("setting up filestream in setup method");
        std::unique_ptr<juce::WavAudioFormat> wavFormat = std::make_unique<juce::WavAudioFormat>();
        DBG("wavFormat was created");
        // II. & III. Initialize and reset writer.
        writer.reset(wavFormat->createWriterFor(fileStream.get(), sampleRate, numChannels, 16, {}, 0));
        if (writer != nullptr)
        {
            fileStream.release();
        }
        DBG("writer reset");
        return true;
    }
    return false;
}

void AudioToFileWriter::writeOutputToFile(const juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);
}

void AudioToFileWriter::closeFile()
{
    writer->flush();
    if (fileStream != nullptr)
    {
        fileStream->flush();
    }
}


// DisplayAudioWaveForm methods

// Constructor
DisplayAudioWaveForm::DisplayAudioWaveForm() : AudioVisualiserComponent(1)
{
    setBufferSize(1024);
    setSamplesPerBlock(256);
    setNumChannels(1);
    setColours(juce::Colours::black, juce::Colours::red);
}

// Destructor
DisplayAudioWaveForm::~DisplayAudioWaveForm()
{

}

// addAudioData
void DisplayAudioWaveForm::addAudioData(const juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    //buffer.getArrayOfReadPointers();
    DBG("Display::AudioWaveForm::addAudioData: called");
}
