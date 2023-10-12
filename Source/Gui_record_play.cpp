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

}

bool AudioToFileWriter::setup(const juce::File& outputFile, int sampleRate, int numChannels)
{
    // fileName contains the outputFile's full absolute path name.
    const juce::String fileName = outputFile.getFullPathName();

    // out will be the file to be exported out.
    juce::File audioFile = outputFile;

    // If the output file exists, delete it and create a new one.
    if (outputFile.exists())
    {
        outputFile.deleteFile();
        audioFile = juce::File(fileName);
    }

    // I. Creates a file stream to which the writer will place audio data.
    fileStream = std::make_unique<juce::FileOutputStream>(audioFile.createOutputStream()->getFile());

    if (fileStream != nullptr)
    {
        juce::WavAudioFormat wavFormat = juce::WavAudioFormat();

        // II. & III. Initialize and reset writer.
        writer.reset(juce::WavAudioFormat().createWriterFor(fileStream.get(), sampleRate, numChannels, 16, {}, 0));
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
        fileStream->flush();
    }
}
