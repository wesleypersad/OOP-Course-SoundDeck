/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                juce::AudioFormatManager &formatManagerToUse,
                juce::AudioThumbnailCache &cacheToUse) : player(_player), 
                waveformDisplay(formatManagerToUse, cacheToUse)
{

    // add buttons and sliders
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(repeatButton);
    addAndMakeVisible(leftNudgeButton);
    addAndMakeVisible(rightNudgeButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(loopSlider);
    addAndMakeVisible(nudgeSlider);

    // labels for sliders
    addAndMakeVisible(volTxt);
    volTxt.setText("Volume:", juce::dontSendNotification);
    volTxt.attachToComponent(&volSlider, true);

    addAndMakeVisible(speedTxt);
    speedTxt.setText("Speed :", juce::dontSendNotification);
    speedTxt.attachToComponent(&speedSlider, true);

    addAndMakeVisible(posTxt);
    posTxt.setText("Postn :", juce::dontSendNotification);
    posTxt.attachToComponent(&posSlider, true);

    // add waveform displays
    addAndMakeVisible(waveformDisplay);

    // add listeners for buttons and sliders
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    repeatButton.addListener(this);
    leftNudgeButton.addListener(this);
    rightNudgeButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    loopSlider.addListener(this);
    nudgeSlider.addListener(this);

    // set ranges and initial values for sliders
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5);
    volSlider.setNumDecimalPlacesToDisplay(2);

    speedSlider.setRange(0.0, 10.0);
    speedSlider.setValue(1.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);

    loopSlider.setRange(0.0, 5.0);
    loopSlider.setValue(2.0);
    loopSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    loopSlider.setNumDecimalPlacesToDisplay(2);

    nudgeSlider.setRange(0.0, 2.0);
    nudgeSlider.setValue(1.0);
    nudgeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    nudgeSlider.setNumDecimalPlacesToDisplay(2);

    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);

    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    // Detect if something is being dragged over deck
    if (somethingIsBeingDraggedOver)
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds(), 3);
    }
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 10; 
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    volSlider.setBounds(50, rowH, getWidth() - 50, rowH);
    speedSlider.setBounds(50, rowH * 2, getWidth(), rowH);
    posSlider.setBounds(50, rowH * 3, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 3);
    nudgeSlider.setBounds(50, rowH * 7, (getWidth() / 3) - 50, rowH);
    leftNudgeButton.setBounds(getWidth() / 3, rowH * 7, getWidth() / 3, rowH);
    rightNudgeButton.setBounds(2 * getWidth() / 3, rowH * 7, getWidth() / 3, rowH);
    loopSlider.setBounds(50, rowH * 8, (getWidth() / 3) - 50, rowH);
    repeatButton.setBounds(getWidth() / 3, rowH * 8, 2 * getWidth() / 3, rowH);
    loadButton.setBounds(0, rowH * 9, getWidth(), rowH);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{chooser.getResult()});            
        }
    }
    if (button == &repeatButton)
    {
        std::cout << "Repeat button was clicked " << std::endl;
        // do something like toggle a repeat flag ?
        if (player->repeat)
        {
            player->repeat = false;
            button->setButtonText("LOOP");
        }
        else 
        {
            player->repeat = true;
            player->repStartTime = player->getPosition();
            player->repEndTime   = player->getPosition() + loopTime;
            button->setButtonText("LOOPING !!!");
        }
    }
    if (button == &leftNudgeButton)
    {
        player->setPosition(player->getPosition() - nudgeTime);
    }
    if (button == &rightNudgeButton)
    {
        player->setPosition(player->getPosition() + nudgeTime);
    }
}

void DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &loopSlider)
    {
        loopTime = slider->getValue();
    }

    if (slider == &nudgeSlider)
    {
        nudgeTime = slider->getValue();
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;

  // check you are dragging a valid sound file
  for (auto file : files)
  {
      if (file.contains(".mp3") || file.contains(".wav") || file.contains(".aif"))
      {
          return true;
      }
  }

  return false;
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(juce::URL{juce::File{files[0]}});
    waveformDisplay.loadURL(juce::URL{ juce::File{files[0]}});
  }
  somethingIsBeingDraggedOver = false;
  repaint();
}

void DeckGUI::fileDragMove(const juce::StringArray& files, int x, int y)
{
    somethingIsBeingDraggedOver = true;
    repaint();
}

void DeckGUI::fileDragExit(const juce::StringArray& files)
{
    somethingIsBeingDraggedOver = false;
    repaint();
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
    // also check the track time if repeat selected
    if (player->repeat)
    {
        // update repEndTime first as slider may change it
        player->repEndTime = player->repStartTime + loopTime;
        if (player->getPosition() >= player->repEndTime)
        {
            player->setPosition(player->repStartTime);
        }
    }
}

bool DeckGUI::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
     // get path of sound file being dragged
    juce::String path = dragSourceDetails.description;

    // check path contains a valid sound file type
    if (path.contains(".mp3") || path.contains(".wav") || path.contains(".aif"))
    {
        return true;
    }

    return false;
}

void DeckGUI::itemDragEnter(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    somethingIsBeingDraggedOver = true;
    repaint();
}

void DeckGUI::itemDragMove(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    std::cout << "Something being moved" << std::endl;
}

void DeckGUI::itemDragExit(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    somethingIsBeingDraggedOver = false;
    repaint();
}

void DeckGUI::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    // get the track data from dragSourceDetails
    juce::String track = dragSourceDetails.description;

    somethingIsBeingDraggedOver = false;

    // load the track into the player
    player->loadURL(juce::URL{ juce::File{track} });
    waveformDisplay.loadURL(juce::URL{ juce::File{track} });

    repaint();
}