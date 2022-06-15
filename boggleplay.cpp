/*Karen Munyan and Candice Murray
 * CS 106B, Spring 2015
 * Problem Set 4: Boggle
 * Citations: Lecture notes, www.cplusplus.com, Stanford libraries, Aubrey Colter, LaIR worker
 *
 * This file generates boggle games from the boggle class, and plays one game with the user. (It is
 * called by the bogglemain.cpp file, which continues playing new games until the user quits.
 * In the boggle game, the human player enters words first, then the computer finds all remaining words.
 */


#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "bogglegui.h"
#include "Boggle.h"
using namespace std;

/* This method prints out the current status of the game. It takes in the boggle game
 * and a string message as parameters. It then clears the console, prints out the message, prints the
 * boggle board to the console, prints out the human's score and found words,
 * and updates the GUI status message.
 */
void printGameState(Boggle game, string message)
{
    clearConsole();
    cout << message << endl;
    //print boggle board
    Set<string> humanFound = game.getHumanFound();
    cout << game << endl;
    cout << "Your words (" << humanFound.size() << "): " << humanFound << endl;
    cout << "Your score: " << game.getScoreHuman() << endl;
    BoggleGUI::setStatusMessage(message);
}

/* This method plays one game of boggle. It asks the user if they want a random board or to manually
 * enter the board letters, sets up the board, generates a boggle game from the boggle class, and plays
 * one game with the user. In the boggle game, the human player enters words first, then the computer
 * finds all remaining words.
 * (Takes in a Lexicon dictionary as parameter.)
 */
void playOneGame(Lexicon& dictionary)
{
    if (!(BoggleGUI::isInitialized())){
       BoggleGUI::initialize(4, 4); //rows, cols);
    }
    else {
       BoggleGUI::reset();
    }
    string boardText = "";
    bool random = getYesOrNo("Do you want to generate a random board? ");
    if(!random)
    {
        boardText = getLine("Type the 16 letters to appear on the board: ");
        while(boardText.length() != 16)
        {
            cout << "That is not a valid 16-letter board string. Try again. " << endl;
            boardText = getLine("Type the 16 letters to appear on the board: ");
        }
        BoggleGUI::labelAllCubes(toUpperCase(boardText));
    }

    clearConsole();
    Boggle game = Boggle(dictionary, boardText); //Generates a boggle game from the boggle class.
    BoggleGUI::setAnimationDelay(100);
    printGameState(game, "It's your turn! ");
    string word;
    word = getLine("Type a word(or Enter to stop): "); //Prompts the user to enter words they find.

    while(word != "") //Keeps playing while the user does not press enter to stop.
    {
        if(!game.checkWord(word))
        {
            string message = "You must enter an unfound 4+ letter word from the dictionary. ";
            printGameState(game, message);
        }
        else
        {
            bool result = game.humanWordSearch(word); //Checks if the word can be found in the board.
            if(result)
            {
                string message = "You found a new word! \"" + toUpperCase(word) + "\" ";
                printGameState(game, message);
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN); //Records the new word
                BoggleGUI::setScore(game.getScoreHuman(), BoggleGUI::HUMAN); //Updates the user's score
            }
            else
            {
                string message = "That word can't be formed on this board. ";
                printGameState(game, message);
            }
        }
        word = getLine("Type a word(or Enter to stop): "); //Re-prompts the user to enter a new word.
        cout << endl;
    }
    cout << "It's my turn! " << endl;
    Set<string> computerFound = game.computerWordSearch(); //Finds the possible set of words for the board
    BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);

    //for each string in set, record word to GUI
    for (string word:computerFound){
        BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
    }

    cout << "My words (" << computerFound.size() << "): " << computerFound << endl;
    cout << "My score: " << game.getScoreComputer() << endl;

    if(game.getScoreComputer() > game.getScoreHuman()) //If the computer wins, sets the status message.
    {
        string message = "Ha ha ha, I destroyed you. Better luck next time, puny human! ";
        BoggleGUI::setStatusMessage(message);
        cout << message << endl << endl;
    }
    else //If there is a tie or the human wins (not really possible), sets the status message.
    {
        string message = "Human player wins! ";
        BoggleGUI::setStatusMessage(message);
        cout << message << endl;
    }

}
