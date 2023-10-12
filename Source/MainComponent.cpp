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

    // Stop Button Setup
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    stopButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);

    // Register basic formats.
    formatManager.registerBasicFormats();

    // Setup Listener for AudioTransportSource
    transportSource.addChangeListener(this);

    // Set input and output channels.
    setAudioChannels(1, 2);

    // Set the window size.
    setSize (300, 200);
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
    }
    else if (button == &startButton)
    {
        DBG("Start button clicked!");
    }
    else if (button == &stopButton)
    {
        DBG("Stop button clicked!");
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{

}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
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
