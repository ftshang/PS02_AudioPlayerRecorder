#pragma once

#include <JuceHeader.h>
#include "Gui_record_play.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Button::Listener, public juce::ChangeListener, public juce::Timer, public juce::Slider::Listener
{
public:

    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void buttonClicked(juce::Button* button) override;
    void openFile(bool forOutput);
    bool loadAudioFile(juce::File& file);
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Open, Start, Stop, and Record Buttons.
    juce::TextButton openButton;
    juce::TextButton startButton;
    juce::TextButton stopButton;
    juce::TextButton recordButton;

    // AudioFormatManager containing a list of audio file forms.
    juce::AudioFormatManager formatManager;

    // readerSource is used to read the AudioFormatReader object.
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    // transportSource handles the starting and stopping of the playback.
    juce::AudioTransportSource transportSource;
   
    // state handles the current "mode" of the program.
    AppState state = IDLE;

    // Handles file selection.
    std::unique_ptr<juce::FileChooser> chooser;

    // AudioToFileWriter instance to handle recording.
    std::unique_ptr<AudioToFileWriter> fileWriter;

    // DisplayAudioWaveForm
    DisplayAudioWaveForm waveForm;

    // Scrubber
    juce::Slider scrubber;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
