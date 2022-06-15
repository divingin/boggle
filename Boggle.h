/*Candice Murray and Karen Munyan
 * CS 106B, Spring 2015
 * Problem Set 4: Boggle
 * Citations: Lecture notes, www.cplusplus.com, Stanford libraries, Aubrey Colter, LaIR worker
 *
 * This is the header file for a class that generates a boggle object that can be used to play a boggle game.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"
#include "vector.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    /*This function creates a boggle object for the given dictionary and the manual board text if given.
     * if boardText not given, generates a random boggle board using the boggle cubes array */
    char getLetter(int row, int col);
    /*This function returns the char letter stored at the given row and column of the boggle  board.*/
    bool checkWord(string word);
    /*this function checks wheter a word is valid, i.e. that the word is in the
     * dictionary, that the word is longer than 4 letters, and that the word has not
     * already been found.*/
    bool humanWordSearch(string word);
    /*This function searchs the board using recursive backtracking for the given word to ensure that it can be found
     * on the board.  As it goes, it highlights the cubes in the GUI that make up the word.  When the word is found,
     * it increases the humanScore accordingly and adds the word to a set of found words, humanFound. */
    Set<string> computerWordSearch();
    /*This function searches the board recursively to find all possible words that have not already been found by the human player
     * and returns them in a set.  The computerScore is also increased appropriately for each word found.*/
    int getScoreHuman();
    /*this function returns the humanScore private variable.*/
    int getScoreComputer();
    /*This function returns the computerScore private variable.*/
    const int getRows();
    /*Returns the number of rows in the boggle board. */
    const int getCols();
    /*Returns the number of columns in the boggle board. */
    const Set<string> getHumanFound();
    /*Returns the set of words that the human has found so far. */
    const Set<string> getComputerFound();
    /*Returns the set of words that the computer has found that the human did not find.*/


    friend ostream& operator<<(ostream& out, Boggle& boggle);
    /*overrides the << operator to enable printing of the boggle board.  Accepts a boggle object.*/

private:
    const int rows = 4;
    /*stores the number of rows in the boggle board.*/
    const int cols = 4;
    /*stores the number of columns in the boggle board. */
    Grid<char> board;
    /*stores the contents of the boggle board. */
    Lexicon dict;
    /*stores the dictionary used to check if words are valid or not.*/
    Set<string> humanFound;
    /*stores the set of words the human finds. */
    Set<string> computerFound;
    /*stores the set of words the computer finds.*/
    int humanScore = 0;
    /*initializes the human score to zero.*/
    int computerScore = 0;
    /*initalizes the computer score to zero*/
    Set<string> computerWordSearchHelper (string prefix, Vector<int> location, Set<Vector<int> > &visited);
    /*A helper function which takes a prefix, a location on the boggle board and the set of visited locations and checks if the prefix is a
     * valid word and if any other valid words can be formed starting from that prefix.*/
    Set<string> neighborIterator(string prefix, Vector<int> location, Set<Vector<int> >& visited);
    /*A helper function for the compuerWordSearch which performs the neighbor itteration for a given location and then calls computerWordSearchHelper
     * for each neighbor location.  */
    bool humanWordSearchHelper(string word, Vector<int> location, Set<Vector<int> > &visited);
    /*A helper function for the humanWordSearch function. Iterates through neighbors of the passed in location to check for the next letter in the
     * word.  Returns true if all letters in the word have been found.*/
};

#endif // _boggle_h
