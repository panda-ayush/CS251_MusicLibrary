//###############################################################################################################
// musiclibrary.cpp, Ayush Panda, apand6, CS 251(Data Structures), Adam Koehler, 1PM Monday Wednesday Friday
//###############################################################################################################
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Program Description~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Music revoles around our lives wherever, we go. This can be just listening to the radio or saving a playlist
// on your phone. In this program we replicate loading music data into a data structure using maps and sets. We 
// then input all our information being give, then determine how to store it in our data structure, then display
// the data that we have found and figure out ways to effectively search through the data we have found. All in 
// all we are essentially creating a music load search engine that can manipulate data using input from the user.
//###############################################################################################################

/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <ctype.h>
#include <string> //string function
#include <fstream> //input output function
#include <math.h> //math functions
#include <map>
#include <set> 
#include <algorithm> //set and map functions


// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE
using namespace std;
//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//
struct albumInfo { //struct to hold album artists and song titles
    set<string> authors;
    set<string> songs;
};


/*
function to load in data based on user input
parameters: file name from user input, map to store all data in
*/
void loadFunction (string fileName, map<string, albumInfo> &albumData) { 
    string albumName; //saved album name after it is getlined
    string tempLine; //temp line that is either song name or artist or repeat of album name

    ifstream infs;

    infs.open(fileName);

    if(infs.is_open()) {
        while(getline(infs, albumName)) {
            albumInfo data;
            while(getline(infs, tempLine)) {

                if (tempLine == albumName) {
                    break;
                }
                else if (isdigit(tempLine[0])) {
                    data.songs.insert(tempLine);
                }
                else {
                    data.authors.insert(tempLine);
                }
            }
            albumData[albumName] = data; //passed album name as key and passed the instance of the sets to the map to
            //save the data retrieved

        }
    }
    else if (!infs.is_open()){ //checks if file did not open
        cout << "Error: Could not open music library file - " << fileName << endl;
    }
    infs.close();

}
/*
function to output the stats of the file loaded in from load functoin
parameters: map holding all data from user input, remains is if stats -d is passed in
*/
void statsFunction (map<string, albumInfo> albumData, string remains) {
    set <string> uniqueArtists;
    int recordSize = 0;
    recordSize = albumData.size();
    int songNumber = 0;
    for (const auto &data: albumData) {
        albumInfo setData = data.second;
        songNumber += setData.songs.size();
        for (const auto &artist: setData.authors) {
            uniqueArtists.insert(artist);//finds unique artist names
        }

    }

    cout << "Overall Music Library Stats" << endl;
    cout << "===========================" << endl;
    cout << "Total Records: " << recordSize << endl;
    cout << "Total Unique Artists: " << uniqueArtists.size() << endl;
    cout << "Total Songs: " << songNumber << endl << endl;

    if (remains == "-d") { //output if user inputs stats -d
        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;
        for (const auto &data: albumData) {
            albumInfo setData = data.second;
            cout << data.first << endl;
            for (const auto &artist: setData.authors) {
                cout << " " << artist << endl;
            }
            for (const auto &song: setData.songs) {
                cout << "   " << song << endl;
            }
        }
        cout << endl;
    }
    
}

/*
function to clear all data in main map
parameters: map holding all information that is loaded in
*/
void clearFunction(map<string, albumInfo> &albumData) {
    albumData.clear();
}

/*
function to search through albums and see if it matches the album inputted by the user
parameters: map holding all data, string input passed in from user, map to hold the name of albums found from user input
*/
void searchAlbum(map<string, albumInfo> albumData, string albumSearch, set<string> &foundAlbum) {
    for (auto albumName: albumData) {//searches for album names based on album input
        string temp = albumName.first;
        tolower(temp);
        tolower(albumSearch);
        if (temp.find(albumSearch) != string::npos) {//if user input matches map data then it gets passed into temp set
            foundAlbum.insert(albumName.first);
            }
        }
    }
/*
function for plus modifier under artist search
parameters: set that hold found album names from previous search, map that holds all data loaded in from function, string being
compared to find new album names, set that outputs new answer after modifier input, temporary set that holds new comparision
album names based on user input
*/
void plusModifierArtist (set<string> &foundAlbum, map<string, albumInfo> albumData, string search, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searches for album name based on artist name input
        albumInfo getData = albumName.second;
        tolower(search);
        for (auto artist: getData.authors) {
            string temp = artist;
            tolower(temp);
            if (temp.find(search) != string::npos) {
                tempSet.insert(albumName.first);
            }
        }
    }
//finds the intersection of previous sets and temporary set saved into the new set.
    set_intersection(foundAlbum.begin(), foundAlbum.end(), tempSet.begin(), tempSet.end(), inserter(newSet, newSet.begin()));
    foundAlbum.clear();
    foundAlbum = newSet;
    newSet.clear();//clears new set

    return;

}
/*
function for minus modifier under artist search
parameters: set that hold found album names from previous search, map that holds all data loaded in from function, string being
compared to find new album names, set that outputs new answer after modifier input, temporary set that holds new comparision
album names based on user input
*/
void minusModifierArtist (set<string> &foundAlbum, map<string, albumInfo> albumData, string search, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searches for album names based on artist name input
        albumInfo getData = albumName.second;
        tolower(search);
        for (auto artist: getData.authors) {
            string temp = artist;
            tolower(temp);
            if (temp.find(search) != string::npos) {
                tempSet.insert(albumName.first);
            }
        }
    }
//finds the difference of sets between previous set and temporary set and saves it into the new set to be outputted
    set_difference(foundAlbum.begin(), foundAlbum.end(), tempSet.begin(), tempSet.end(), inserter(newSet, newSet.begin()));
    foundAlbum.clear();
    foundAlbum = newSet;
    newSet.clear();//clears new set

    return;

}
/*
function for plus modifier under song search
parameters: set that hold found album names from previous search, map that holds all data loaded in from function, string being
compared to find new album names, set that outputs new answer after modifier input, temporary set that holds new comparision
album names based on user input
*/
void plusModifierSong (set<string> &foundAlbum, map<string, albumInfo> albumData, string search, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searches for album names based on song name input
        albumInfo getData = albumName.second;
        tolower(search);
        for (auto song: getData.songs) {
            string temp = song;
            tolower(temp);
            if (temp.find(search) != string::npos) {
                tempSet.insert(albumName.first);
            }
        }
    }
//finds the intersection of sets with previous seach and temp set and saves in new set
    set_intersection(foundAlbum.begin(), foundAlbum.end(), tempSet.begin(), tempSet.end(), inserter(newSet, newSet.begin()));
    foundAlbum.clear();
    foundAlbum = newSet;
    newSet.clear();//clears new set

    return;

}
/*
function for minus modifier under song search
parameters: set that hold found album names from previous search, map that holds all data loaded in from function, string being
compared to find new album names, set that outputs new answer after modifier input, temporary set that holds new comparision
album names based on user input
*/
void minusModifierSong (set<string> &foundAlbum, map<string, albumInfo> albumData, string search, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searchs for album names based on song input
        albumInfo getData = albumName.second;
        tolower(search);
        for (auto song: getData.songs) {
            string temp = song;
            tolower(temp);
            if (temp.find(search) != string::npos) {
                tempSet.insert(albumName.first);
            }
        }
    }
//finds the difference between
    set_difference(foundAlbum.begin(), foundAlbum.end(), tempSet.begin(), tempSet.end(), inserter(newSet, newSet.begin()));
    foundAlbum.clear();
    foundAlbum = newSet;
    newSet.clear();//clears new set

    return;

}
/*
function searches for album names based on artist name input from user
parameters: map that holds all data loaded from previous function, string pased in from user input to search for artist,
set that holds album names based on search, sets passed in for modifier functions
*/
void searchArtist(map<string, albumInfo> albumData, string artistSearch, set<string> &foundAlbum, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searchs for album names based in artist name input
        albumInfo getData = albumName.second;
        tolower(artistSearch);
        for (auto artist: getData.authors) {
            string temp = artist;
            tolower(temp);
            if (temp.find(artistSearch) != string::npos ) {
                foundAlbum.insert(albumName.first);
            }
        }
    }
    if (artistSearch[0] == '+') {//function for plus modifier
        artistSearch = artistSearch.erase(0,1);
        plusModifierArtist(foundAlbum, albumData, artistSearch, newSet, tempSet);
    }
    if (artistSearch[0] == '-') {//function for minus modifier
        artistSearch = artistSearch.erase(0,1);
        minusModifierArtist(foundAlbum, albumData, artistSearch, newSet, tempSet);
    }
}
/*
function searches for album names based on song name input from user
parameters: map that holds all data loaded from previous function, string pased in from user input to search for song name,
set that holds album names based on search, sets passed in for modifier functions
*/
void searchSong(map<string, albumInfo> albumData, string songSearch, set<string> &foundAlbum, set <string> &newSet, set <string> &tempSet) {
    for (const auto &albumName: albumData) {//searches for album names baed on song name from user input
        albumInfo getData = albumName.second;
        tolower(songSearch);
        for (auto song: getData.songs) {
            string temp = song;
            tolower(temp);
            if (temp.find(songSearch) != string::npos) {
                foundAlbum.insert(albumName.first);
            }
        }
    }
    if (songSearch[0] == '+') {//calls plus modifier function for song search
        songSearch = songSearch.erase(0,1);
        plusModifierSong(foundAlbum, albumData, songSearch, newSet, tempSet);
    }
    if (songSearch[0] == '-') {//calls minus modifier function for song search
        songSearch = songSearch.erase(0,1);
        minusModifierSong(foundAlbum, albumData, songSearch, newSet, tempSet);
    }


}
/*
function that determines if we are search for album names, song titles, or artist names
parameters: map that holds all data based on previous function, stirng that holds all reainders after commands
set that holds the answers for search function
*/
void searchFunction(map<string, albumInfo> albumData, string remains, set <string> &foundAlbum) {
    string leftOver, searchInfo;
    splitFirstWord(remains, searchInfo, leftOver);
    tolower(searchInfo);
    set <string> newSet;//temporary sets for modifier functions
    set <string> tempSet;

    if (searchInfo == "album") {//album search
        string albumSearch, remaining;
        while(!leftOver.empty()) {
            splitFirstWord(leftOver, albumSearch, remaining);
            leftOver = remaining;
            searchAlbum(albumData, albumSearch, foundAlbum);
        }
    }
    else if (searchInfo == "artist") {//artist search
        string artistSearch, remaining;
        while(!leftOver.empty()) {
            splitFirstWord(leftOver, artistSearch, remaining);
            leftOver = remaining;
            searchArtist(albumData, artistSearch, foundAlbum, newSet, tempSet);
            tempSet.clear();//temp set clear
        }
    }
    else if (searchInfo == "song") {//song search
        string songSearch, remaining;
        while(!leftOver.empty()) {
            splitFirstWord(leftOver, songSearch, remaining);
            leftOver = remaining;
            searchSong(albumData, songSearch, foundAlbum, newSet, tempSet);
            tempSet.clear();//temp set clear
        }
    }
    else {
       cout << "Error: Search terms cannot be empty." << endl;//if input is not for album name, song name or aritst name
       cout << "No results found." << endl << endl;
    }

}
/*
function that exports data found from load function
parameters: string that holds potential file names, map that holds all data based on previous load function
*/
void exportFunction (string remains, map<string, albumInfo> albumData) {
    string fileName, leftOver;
    splitFirstWord(remains, fileName, leftOver);
    ofstream ofs;
    if (fileName.size() > 0) {//determines which file name to export
        ofs.open(fileName);
    }
    else {
    ofs.open("musicdatabase.txt");
    }

        for (const auto &data: albumData) {
            albumInfo setData = data.second;
            ofs << data.first << endl;
            for (const auto &artist: setData.authors) {
                ofs << artist << endl;
            }
            for (const auto &song: setData.songs) {
                ofs << song << endl;
            }
            ofs << data.first << endl;
        }
        cout << endl;
        ofs.close();
}
/*
main function
*/
int main()
{
    string userEntry;
    string command, remains;

    map<string, albumInfo> albumData;//map to hold all data
    set <string> foundAlbum;

    // TODO: Declare any necessary variables for the music libary

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        if (userEntry[userEntry.size()-1] == ' ') {
            userEntry = userEntry.substr(0, userEntry.size()-1);
        }
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")//clear command
        {
            clearFunction(albumData);
        }
        else if (command == "export")//export command
        {
            exportFunction(remains, albumData);
        }
        else if (command == "load")//load command
        {
            if(remains.size() > 1) {
                loadFunction(remains, albumData);
            }
            
        }
        else if (command == "stats")//stats command
        {
            statsFunction(albumData, remains);
        }          
        else if (command == "search")//search command
        {
            searchFunction(albumData, remains, foundAlbum);
            if (foundAlbum.size() > 0) {
                cout << "Your search results exist in the following albums: " << endl;
                for (const auto &info: foundAlbum) {
                    cout << info << endl;
                }
                cout << endl;
            }

            foundAlbum.clear();
        }

    }while(command != "exit"); //break function   

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
