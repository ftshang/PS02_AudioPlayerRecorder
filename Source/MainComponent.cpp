#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Open Button Setup
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open File");
    openButton.addListener(this);

    // Start Button Setup
    addAndMakeVisible(startButton);
    startButton.setButtonText("Start");
    startButton.addListener(this);
    startButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::green);
    startButton.setEnabled(false);

    // Stop Button Setup
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    stopButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    // Record Button Setup
    addAndMakeVisible(recordButton);
    recordButton.setButtonText("Record to New File");
    recordButton.addListener(this);

    // Register basic formats.
    formatManager.registerBasicFormats();

    // Setup Listener for AudioTransportSource
    transportSource.addChangeListener(this);

    // Set input and output channels.
    setAudioChannels(1, 2);

    // Setup an instance of AudioToFileWriter.
    fileWriter = std::make_unique<AudioToFileWriter>();

    // Set the window size.
    setSize (300, 250);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    openButton.removeListener(this);
    startButton.removeListener(this);
    stopButton.removeListener(this);
    recordButton.removeListener(this);
    transportSource.removeChangeListener(this);

    shutdownAudio();
}

bool MainComponent::loadAudioFile(juce::File& file)
{
    if (file != juce::File{})
    {
        // I. Creates an AudioFormatReader
        auto* reader = formatManager.createReaderFor(file);

        // II. Validate the created AudioFormatReader
        if (reader != nullptr)
        {
            // III. Instantiate an AudioFormatReaderSource 
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // IV. Stop transport source and set the current source to null.
            transportSource.stop();
            transportSource.setSource(nullptr);

            // V. Assign the AudioFormatReaderSource to the transportSource.
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            readerSource.reset(newSource.release());

            return true;
        }
    }
    return false;
}

void MainComponent::openFile(bool forOutput)
{
    chooser = std::make_unique<juce::FileChooser>("Select a WAV file...", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    if (forOutput)
    {
        chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
    }

    chooser->launchAsync(chooserFlags, [this, forOutput](const juce::FileChooser& fc)
        {
            juce::File file = fc.getResult();

            // Hands playing audio files.
            if (!forOutput)
            {
                if (loadAudioFile(file))
                {
                    startButton.setEnabled(true);
                }
            }
            else {
                //juce::File file = fc.getResult();

                if (fileWriter->setup(file, 44100, 1))
                {
                    state = RECORDING;
                }
            }

        });
}

void MainComponent::buttonClicked(juce::Button* button) 
{
    if (button == &openButton)
    {
        //DBG("Open button clicked!");

        if (transportSource.isPlaying())
        {
            transportSource.stop();
        }
        openFile(false);
    }
    else if (button == &startButton)
    {
        //DBG("Start button clicked!");
        state = PLAYING;
        transportSource.start();
        stopButton.setEnabled(true);
        startButton.setEnabled(false);
    }
    else if (button == &stopButton)
    {
        //DBG("Stop button clicked!");
        if (state == RECORDING)
        {
            fileWriter->closeFile();
        }
        else
        {
            transportSource.stop();
            startButton.setEnabled(true);
        }
        state = IDLE;
        stopButton.setEnabled(false);
    }
    else if (button == &recordButton)
    {
        //DBG("Record button clicked!");
        openFile(true);
        stopButton.setEnabled(true);
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
        {
            state = PLAYING;
        }
        else {
            state = IDLE;
            stopButton.setEnabled(false);
            //startButton.setEnabled(false);
        }
    }
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (state == IDLE)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    if (state == RECORDING)
    {
        //DBG("getNextAudioBlock: state is RECORDING");
        fileWriter->writeOutputToFile(*bufferToFill.buffer);
        bufferToFill.clearActiveBufferRegion();

    } else if (state == PLAYING)
    {
        if (readerSource.get() == nullptr)
        {
            //DBG("readerSource.get() is nullptr");
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        //DBG("getNextAudioBlock: state is PLAYING");
        transportSource.getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    startButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    recordButton.setBounds(10, 100, getWidth() - 20, 20);
}
