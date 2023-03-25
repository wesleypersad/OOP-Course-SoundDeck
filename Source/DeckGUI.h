/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public juce::Component,
                   public juce::Button::Listener, 
                   public juce::Slider::Listener, 
                   public juce::FileDragAndDropTarget,
                   public juce::DragAndDropTarget,
                   public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           juce::AudioFormatManager &formatManagerToUse,
           juce::AudioThumbnailCache &cacheToUse);
    ~DeckGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (juce::Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (juce::Slider *slider) override;

    // functions for file dragging ...

    bool isInterestedInFileDrag (const juce::StringArray &files) override;

    void filesDropped (const juce::StringArray &files, int x, int y) override;

    void fileDragMove(const juce::StringArray& files, int x, int y) override;

    void fileDragExit(const juce::StringArray& files) override;

    void timerCallback() override;

    // functions for item dragging ...

    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void itemDragEnter(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void itemDragMove(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void itemDragExit(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;

private:

    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton loadButton{"LOAD"};
    juce::TextButton repeatButton{"LOOP"};
    juce::TextButton leftNudgeButton{ "<NUDGE" };
    juce::TextButton rightNudgeButton{ "NUDGE>" };
  
    juce::Slider volSlider; 
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::Slider loopSlider;
    juce::Slider nudgeSlider;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    //juce::String message{ "Drag-and-drop some rows..." };
    bool somethingIsBeingDraggedOver = false;

    // variable to hold loop and nudge times
    float loopTime, nudgeTime;

    // add some labels for sliders
    juce::Label volTxt;
    juce::Label speedTxt;
    juce::Label posTxt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
