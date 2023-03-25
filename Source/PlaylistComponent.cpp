/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 19 Aug 2020 7:29:17pm
    Author:  Wesley Persad

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
    // Add the code to read the file
    fileComp.reset(new juce::FilenameComponent("fileComp",
        {},                       // current file
        false,                    // can edit file name,
        false,                    // is directory,
        false,                    // is for saving,
        {},                       // browser wildcard suffix,
        {},                       // enforced suffix,
        "Select file to open"));  // text when nothing selected
    addAndMakeVisible(fileComp.get());
    addAndMakeVisible(enterSearchTxt);
    enterSearchTxt.setText("Enter Search Text:", juce::dontSendNotification);
    enterSearchTxt.attachToComponent(&searchText, true);

    fileComp->addListener(this);

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    searchText.addListener(this);

    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Full Path", 2, 800);
    tableComponent.getHeader().addColumn("Artist/Group", 3, 200);
    tableComponent.getHeader().addColumn("Length (seconds)", 4, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);

    addAndMakeVisible(searchText);

    addAndMakeVisible(saveButton);
    saveButton.addListener(this);

    setName("Drag-and-Drop");

    //startTimer(100);
}

PlaylistComponent::~PlaylistComponent()
{
    //stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
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
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    // Detect if something is being dragged over deck
    if (FileIsBeingDraggedOver)
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds(), 3);
    }
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //searchText.setBounds(0, 0, getWidth(), getHeight() / 10);
    searchText.setBounds(125, 0, getWidth() / 2 - 125, getHeight() / 10);
    //textContent->setBounds(0, getHeight() / 10, getWidth(), 2 * getHeight() / 10);
    fileComp->setBounds(0, getHeight() / 10, getWidth() / 2, getHeight() / 10);
    saveButton.setBounds(getWidth() / 2, getHeight() / 10, getWidth() / 2, getHeight() / 10);
    tableComponent.setBounds(0, 2 * getHeight() / 10, getWidth(), 8 * getHeight() /10);

}

int PlaylistComponent::getNumRows()
{
    //return trackTitles.size();
    return trackData.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int 	rowNumber,
    int 	width,
    int 	height,
    bool 	rowIsSelected)
{

    std::string what = searchText.getText().toStdString();
    int fpos = trackData[rowNumber][0].find(what);

    if (((fpos > -1) && (what != "")) || rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }

}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int 	rowNumber,
    int 	columnId,
    int 	width,
    int 	height,
    bool 	rowIsSelected)
{
    /** there is a bug where juce tries to draw an additional row which 
    crashes the app on resize (an empty row of vector) :
    Acknowledgement to Irfan on the CM2005 Slack channel for supplying this fix */
    if (rowNumber >= trackData.size()) return;

    g.drawText(trackData[rowNumber][((int) columnId - 1)],
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true); 
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                        int 	columnId,
                        bool 	isRowSelected,
                        Component* existingComponentToUpdate)
{
    //if (columnId == 2)
    //{
    //    if (existingComponentToUpdate == nullptr)
    //    {
    //        juce::TextButton* btn = new juce::TextButton{ "play" };
    //        juce::String id{std::to_string(rowNumber)};
    //        btn->setComponentID(id);
    //        btn->addListener(this);
    //        existingComponentToUpdate = btn;
    //    }
    //}
        
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextEditor* field = new juce::TextEditor;
            juce::String id{std::to_string(rowNumber)};
            // get the Artist/Group information from the trackData array
            field->setText(trackData[rowNumber][columnId-1]);
            field->setComponentID(id);
            field->addListener(this);
      
            existingComponentToUpdate = field;
        }
        // highlight background if row selected
        if (isRowSelected)
        {
            existingComponentToUpdate->setColour(juce::TextEditor::backgroundColourId, juce::Colours::orange);
        }
        else {
            existingComponentToUpdate->setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
        }     
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &saveButton)
    {
        // save button writes trackData to csv file
        std::cout << "Save playlist button was clicked " << std::endl;

        if (!PlaylistComponent::writeCSV("PlayList.csv", trackData))
        {
            std::cout << "Problem writing to file " << std::endl;
        }

    }
 
    /*int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "PlaylistComponent::buttonClicked" << trackTitles[id] << std::endl;
    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);*/
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& searchtext)
{
    std::cout << "PlaylistComponent::buttonClicked" << searchtext.getText() << std::endl;
    DBG("PlaylistComponent::buttonClicked " << searchtext.getText());

    //As the string in the text editor box is changed, we can redraw
    //the playlist based on the new search string in
    repaint();
}

void PlaylistComponent::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fileComp.get())
    {
        readFile(fileComp->getCurrentFile());
    }
}

void PlaylistComponent::readFile(const juce::File& fileToRead)
{
    if (!fileToRead.existsAsFile())
        return;  // file doesn't exist

    juce::String filePath{fileToRead.getFullPathName()};

    trackData = readCSV(filePath.toStdString());
    repaint();
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout << "PlaylistComponent::isInterestedInFileDrag" << std::endl;

    // check you are dragging a valid sound file
    for (auto file : files)
    {
        if (file.contains(".mp3") || file.contains(".wav") || file.contains(".aif"))
        {
            FileIsBeingDraggedOver = true;
            return true;
        }
    }

    return false;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    FileIsBeingDraggedOver = false;

    std::cout << "PlaylistComponent::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        juce::File file = files[0];
        // get the tracklength of the file dropped
        this->loadURL(juce::URL{ file });
        double trackLength = this->getTrackLength();

        // write all this to trackData
        trackData.push_back({ file.getFileNameWithoutExtension().toStdString(),
                             file.getFullPathName().toStdString(),
                             "Unknown",
                             std::to_string(trackLength) });
        
        repaint();
    }
}

void PlaylistComponent::startDragging(const juce::var& sourceDescription, Component* sourceComponent)
{
    // do something in the this method
    std::cout << "THIS IS BEING CALLED !!!" << std::endl;
}

bool PlaylistComponent::writeCSV(std::string csvFile, std::vector<std::array<std::string, 4>>& rows)
{
    // Don't write to file if nothing in rows
    if (rows.size() <= 0) return false;

    // open and append to the file specified
    std::ofstream file;
    file.open(csvFile, std::ios_base::out);

    // loop throgh the vector of string string arrays and write every line to the CSV file
    for (std::array<std::string, 4> row : rows)
    {
        file << row[0] << "," << row[1] << "," << row[2] << "," << row[3]<< std::endl;
    }

    file.close();

    return true;
}

std::vector<std::array<std::string, 4>> PlaylistComponent::readCSV(std::string csvFilename)
{
    std::vector<std::array<std::string, 4>> playlist;

    std::ifstream csvFile{ csvFilename };
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try {
                std::array<std::string, 4> trackdata = tokenise(line, ',');
                playlist.push_back(trackdata);
            }
            catch (const std::exception& e)
            {
                std::cout << "readCSV bad data " << std::endl;
            }
        }
    }

    return playlist;
}

std::array<std::string, 4> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    // seperate the line read from the csv file into tokens
    std::array<std::string, 4> tokens;

    std::vector<int> pos;

    for (int i = 0; i<csvLine.size(); i++ )
    {
        if (csvLine[i] == separator)
        {
            pos.push_back(i);
        }
    }

    // load the token with the seperate metadata
    tokens[0] = csvLine.substr(0, pos[0]);
    tokens[1] = csvLine.substr((pos[0] + 1), (pos[1] - pos[0] - 1));
    tokens[2] = csvLine.substr((pos[1] + 1), (pos[2] - pos[1] - 1));
    tokens[3] = csvLine.substr((pos[2] + 1), csvLine.size() - 1);

    return tokens;
}

juce::var PlaylistComponent::getDragSourceDescription(const juce::SparseSet<int>& selectedRows)
{
    // for our drag description, we'll return the music track full path from the trackData  
    // array in row to be picked up by the drag target and displayed in its box.
    juce::String row = juce::String(trackData[selectedRows[0]][1]);

    std::cout << row << std::endl;

    return row;
}

void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& field)
{
    // get id of textEditor object to find row of trackData to update 
    int id = std::stoi(field.getComponentID().toStdString());
    trackData[id][2] = field.getText().toStdString();
}

void PlaylistComponent::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

double PlaylistComponent::getTrackLength()
{
    // return the length of the audio track in seconds
    return transportSource.getLengthInSeconds();
}

//void PlaylistComponent::timerCallback()
//{
//    // check if the size of trackData has changed
//    // force repaint if so
//
//    if (trackData.size() != numTracks)
//    {
//        numTracks = trackData.size();
//        repaint();
//    }
//}