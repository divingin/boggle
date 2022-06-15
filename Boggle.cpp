/*Candice Murray and Karen Munyan
 * CS 106B, Spring 2015
 * Problem Set 4: Boggle
 * Citations: Lecture notes, www.cplusplus.com, Stanford libraries, Aubrey Colter, LaIR worker
 *
 * This class generates a boggle object that can be used to play a boggle game.
 */

#include "Boggle.h"
#include "grid.h"
#include "set.h"
#include "bogglegui.h"
#include "simpio.h"
#include "shuffle.h"
#include "string.h"
#include "strlib.h"
#include <string>


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};


Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dict = dictionary;
    string inputText = toUpperCase(boardText);
    if (!BoggleGUI::isInitialized()){
        BoggleGUI::initialize(rows,cols);
    }
    board = Grid<char> (rows,cols);
    if (inputText != ""){ //labels the cubes for the manual board
        for (int i = 0; i<rows; i++){
            for (int j = 0; j<cols; j++){
                board[i][j] = inputText[0];
                inputText = inputText.substr(1);
            }
        }
       BoggleGUI::labelAllCubes(toUpperCase(boardText));
    }else {  //labels the cubes for the random board
        shuffle(CUBES,16);
        int counter = 0;
        for (int i = 0; i<rows; i++){
            for (int j = 0; j<cols; j++){
                char letter = shuffle(CUBES[counter])[0]; //grabs the first shuffled letter of the first shuffled element of the CUBES array
                board[i][j] = letter;
                counter++;
                BoggleGUI::labelCube(i,j,letter,false);
            }
        }      
    }
}

const int Boggle::getRows() {
    return rows;
}

const int Boggle::getCols() {
    return cols;
}

const Set<string> Boggle::getHumanFound(){
    return humanFound;
}

const Set<string> Boggle::getComputerFound(){
    return computerFound;
}


char Boggle::getLetter(int row, int col) {
    return board[row][col];
}

bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if (!dict.contains(word)||!(word.size()>=4)||humanFound.contains(word)||computerFound.contains(word)){
        return false;
    }
    return true;
}

bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting(); //clears highlighting when finding a new word
    word = toUpperCase(word);
    Vector<int> location(2,0);  //this vector gets added to the set of visited locations
    for (int i = 0; i<rows; i++) {
        location[0] = i;
        for (int j = 0; j<cols; j++) {
            Set<Vector<int> > visited;
            location[1] = j;
            BoggleGUI::setHighlighted(i,j,true);
            //choose
            if (getLetter(i,j) == word[0]){
                visited += location;
                string oldWord = word;
                string substring = word.erase(0,1);
                //explore
                bool found = humanWordSearchHelper(substring,location,visited);
                //unchoose
                word = oldWord;
                if (found){ //we found the word, yay!
                    humanScore += (oldWord.length()-3);
                    humanFound += oldWord;
                    return true;
                }else{ //clear the highlighting
                    BoggleGUI::setHighlighted(i,j,false);
                }
            }else{ //clear the highlighting and unchoose the location
                BoggleGUI::setHighlighted(i,j,false);
                visited -= location;
            }
        }
    }
    BoggleGUI::clearHighlighting(); //clears highlighting if the word isn't there
    return false;
}

bool Boggle::humanWordSearchHelper (string word, Vector<int> location, Set<Vector<int> >& visited){
    int r = location[0];
    int c = location[1];
    BoggleGUI::setHighlighted(r,c,true);
    //recursive base case
    if (word.empty()){
        return true;
    }else {
        Vector<int> neighborLocation(2,0); //this stores the location of the neighbor being checked
        for (int x = r-1; x<=r+1; x++){ //iterating through all of the neighbors
            neighborLocation[0] = x;
            for (int y = c-1; y<=c+1; y++){
                neighborLocation[1]= y;
                if (neighborLocation!=location && board.inBounds(x,y) && !visited.contains(neighborLocation) && (word[0] == getLetter(x,y))){
                    //choose
                    string oldWord = word;
                    string substring = word.erase(0,1);
                    visited += neighborLocation;
                    //explore
                    bool found = humanWordSearchHelper(substring,neighborLocation, visited);
                    if (found){
                        return true;
                    } else{ //unchoose
                        word = oldWord;
                        visited -= neighborLocation;
                        if (!visited.contains(neighborLocation)){ //clears the highlighting as long as the letter isn't used elsewhere in the word
                            BoggleGUI::setHighlighted(neighborLocation[0],neighborLocation[1],false);
                        }
                    }
                }
            }
        }
   }
   return false;
}


int Boggle::getScoreHuman() {
    return humanScore;
}

Set<string> Boggle::computerWordSearch() {
    //first the computer iterates through each letter in the board
    Vector<int> location(2,0);  //this vector gets added to the set of visited locations
    for (int i = 0; i < rows; i++) {
        location[0] = i;
        for (int j = 0; j < cols; j++) {
            location[1] = j;
            Set<Vector<int> > visited; //keeps track of where we've been
            visited += location;
            string prefix;
            prefix += getLetter(i,j);
            //next, the computer iterates through each neighbor of the letter it's at and checks to see if that prefix is a word
            computerFound += neighborIterator(prefix, location, visited);
        }
    }
    return computerFound;
}



Set<string> Boggle::neighborIterator(string prefix, Vector<int> location, Set<Vector<int> >& visited) {
    string oldPrefix = prefix;
    int r = location[0];
    int c = location[1];
    Vector<int> neighborLocation(2,0); //this stores the location of the neighbor being checked
    for (int x = r-1; x<=r+1; x++){ //iterating through all of the neighbors
        neighborLocation[0] = x;
        for(int y = c-1; y<=c+1; y++){
            neighborLocation[1]= y;
            if (neighborLocation!=location && board.inBounds(x,y) && !visited.contains(neighborLocation)){
                //choose
                visited += neighborLocation;
                //explore
                computerFound += computerWordSearchHelper(prefix, neighborLocation, visited);
                //unchoose
                prefix = oldPrefix;
                visited -= neighborLocation;
            }
        }
    }
    return computerFound;
}


Set<string> Boggle::computerWordSearchHelper (string prefix, Vector<int> location, Set<Vector<int> > & visited){
    prefix += getLetter(location[0],location[1]); //changes the prefix to include the current letter
    //base case
    if (!dict.containsPrefix(prefix)){ //if the prefix is not part of any valid words, we're done with this prefix.
        return computerFound;
    //recursive case
    } else {
        if (checkWord(prefix)){
            computerScore += (prefix.length()-3);
            computerFound += prefix;
        }
        neighborIterator(prefix,location,visited);
   return computerFound;
    }
}







int Boggle::getScoreComputer() {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int row = 0; row < boggle.getRows(); row++) {
           for(int col = 0; col < boggle.getCols(); col++){
               out << boggle.getLetter(row, col);
           }
           out << endl;
       }
       return out;
    return out;
}
