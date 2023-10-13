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
    // If the output file exists, delete it and create a new one.
    if (outputFile.exists())
    {
        outputFile.deleteFile();
    }

    outputFile.create();

    // I. Creates a file stream to which the writer will place audio data.
    fileStream = std::make_unique<juce::FileOutputStream>(outputFile);

    if (fileStream != nullptr)
    {
        std::unique_ptr<juce::WavAudioFormat> wavFormat = std::make_unique<juce::WavAudioFormat>();
        // II. & III. Initialize and reset writer.
        writer.reset(wavFormat->createWriterFor(fileStream.get(), sampleRate, numChannels, 16, {}, 0));
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
    if (fileStream != nullptr)
    {
        writer->flush();
        fileStream->flush();
        DBG("Closed filestream.");
    }
}
