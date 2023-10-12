#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : state(IDLE)
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

    // Register basic formats.
    formatManager.registerBasicFormats();

    // Setup Listener for AudioTransportSource
    transportSource.addChangeListener(this);

    // Set input and output channels.
    setAudioChannels(1, 2);

    // Set the window size.
    setSize (300, 250);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::buttonClicked(juce::Button* button) 
{
    if (button == &openButton)
    {
        DBG("Open button clicked!");
        chooser = std::make_unique<juce::FileChooser>("Select a WAV file to play...", juce::File{}, "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(chooserFlags, [this] (const juce::FileChooser& fc) 
            {
                auto file = fc.getResult();

                if (file != juce::File{})
                {
                    auto* reader = formatManager.createReaderFor(file);

                    if (reader != nullptr)
                    {
                        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

                        // Enabling startButton once file has been loaded.
                        startButton.setEnabled(true);
                        readerSource.reset(newSource.release());
                    }
                }
            });
    }
    else if (button == &startButton)
    {
        DBG("Start button clicked!");
        changeState(PLAYING);
    }
    else if (button == &stopButton)
    {
        DBG("Stop button clicked!");
        changeState(IDLE);
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
        {
            changeState(PLAYING);
        }
        else {
            changeState(IDLE);
        }
    }
}

void MainComponent::changeState(AppState newState)
{
    if (state != newState)
    {
        AppState oldState = state;
        // Update the state.
        state = newState;

        switch (state)
        {
        case IDLE:
            stopButton.setEnabled(false);
            startButton.setEnabled(true);
            if (oldState == PLAYING)
            {
                transportSource.stop();
            }
            transportSource.setPosition(0.0);
            break;

        case PLAYING:
            transportSource.start();
            stopButton.setEnabled(true);
            startButton.setEnabled(false);
            break;

        case RECORDING:
            break;
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
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    startButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
}
