/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1400, 800);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2); 

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!

    // set colour of all buttons
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    getLookAndFeel().setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkorange);
    getLookAndFeel().setColour(juce::TextEditor::textColourId, juce::Colours::yellow);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::darkorange);
    getLookAndFeel().setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkorange);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::yellow);
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);

}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, 2*getHeight()/3);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, 2*getHeight()/3);

    playlistComponent.setBounds(0, 2*getHeight()/3, getWidth(), getHeight()/3);

}

