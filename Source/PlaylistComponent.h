/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Aug 2020 7:29:17pm
    Author:  Wesley Persad

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <fstream>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::FilenameComponentListener,
                           public juce::FileDragAndDropTarget,
                           public juce::DragAndDropContainer
//                           public juce::Timer
{
public:
    PlaylistComponent(juce::AudioFormatManager& _formatManager);

    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,
                        int 	rowNumber,
                        int 	width,
                        int 	height,
                        bool 	rowIsSelected) override;

    void paintCell(juce::Graphics&,
                        int 	rowNumber,
                        int 	columnId,
                        int 	width,
                        int 	height,
                        bool 	rowIsSelected) override;

    Component* refreshComponentForCell(int 	rowNumber,
                        int 	columnId,
                        bool 	isRowSelected,
                        Component* existingComponentToUpdate) override;
    
    void buttonClicked(juce::Button* button) override;

    void textEditorTextChanged(juce::TextEditor& searchtext) override;

    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override;

    void readFile(const juce::File& fileToRead);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void startDragging(const juce::var& sourceDescription, Component* sourceComponent);

    /** This writes from a an array of strings to a csv file */
    static bool writeCSV(std::string csvFile, std::vector<std::array<std::string, 4>>& rows);

    /** This reads all the seperate lines from the csv file */
    std::vector<std::array<std::string, 4>> readCSV(std::string csvFile);

    /** This tokenise the line from the csv file */
    std::array<std::string, 4> tokenise(std::string csvLine, char separator);

    /** This gets the description of what is dragged */
    juce::var getDragSourceDescription(const juce::SparseSet<int>& selectedRows) override;

    /** Listener for when return entered in text editor used for metadata */
    void textEditorReturnKeyPressed(juce::TextEditor& field) override;

    // The following functions have been copied from DJAudioPlayer
    // so that metadata can be obtained from a file path

    /** load audio track to finds it's metadata*/
    void loadURL(juce::URL audioURL);

    /** get the length in seconds of the loaded track */
    double getTrackLength();

    /** add a timer to check if screen needs a repaint */
    //void timerCallback() override;

private:

    juce::TableListBox tableComponent;
    //std::vector<std::string> trackTitles;
    juce::TextEditor searchText;

    std::unique_ptr<juce::FilenameComponent> fileComp;

    juce::TextButton saveButton{ "SAVE PLAYLIST" };

    // create a vector of string to hold the songtrack data
    std::vector<std::array<std::string, 4>> trackData;

    juce::Label enterSearchTxt;

    bool FileIsBeingDraggedOver = false;

    //this is a test to get file length metadata
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    int numTracks = trackData.size();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
