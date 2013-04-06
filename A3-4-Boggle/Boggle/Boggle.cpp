/*
 * File: Boggle.cpp
 * ----------------
 * Name: Nathan Hayes-Roth
 * Section: Tom Linford
 * This program allows the user to play a simulated game of boggle. It simulates the commercial game by
 * randomly ordering and orienting digital Boggle blocks on the screen. Users enter words for verification via the keyboard.
 * After the user ends his/her turn, the program will solve the puzzle for the remaining words and report the user's performance,
 * prompting him/her to play again if so inclined.
 */
 
#include <iostream>
#include <math.h>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Constants */
const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const string STANDARD_CUBES[16]  = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
const string BIG_BOGGLE_CUBES[25]  = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */
void welcome();
bool yesNo();
void giveInstructions();
int chooseSize();
Grid<char> createBoard(int size);
Lexicon humanPlays(Grid<char> & board, Lexicon & english);
Lexicon humanPlays(Grid<char> & board, Lexicon & english, Lexicon & usedWords);
bool isValid(string& word, Grid<char>& board, Grid<bool>& usedLetters, Lexicon& usedWords, Lexicon& english);
bool canBeMade(string word, Grid<char>& board, Grid<bool>& usedLetters, int row, int col, int spacesAway);
Lexicon computerPlays(Grid<char>& board, Lexicon& english, Lexicon& humanWords);
void findWords(string prefix, int row, int col, Grid<char>& cubes, Grid<bool>& usedLetters, 
						 Lexicon& english, Lexicon& wordsFound, Lexicon& humansWords);
void compareScores(Lexicon& humanWords, Lexicon& computerWords, float& count, float& previous, float& average, float& best);


/* Main program */
int main() {
	Lexicon english("EnglishWords.dat");
	float count = 0;
	float previous = 0;
	float average = 0;
	float best = 0;	//for measuring improvement over successive games
	welcome();
	cout<<"Do you need instractions? ";
	if(yesNo()) giveInstructions();
	int size = chooseSize();
	while(true){
		initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);	//keeping this line in while loop allows the scores/words to clear each round
		drawBoard(size, size);
		Grid<char> board = createBoard(size);
		Lexicon humanWords = humanPlays(board, english);
		Lexicon computerWords = computerPlays(board, english, humanWords);
		compareScores(humanWords, computerWords, count, previous, average, best);
		if(!yesNo()){
			cout<<"\tThanks for playing!";
			break;
		}
	}
	return 0;
}

/*
 * Function: welcome
 * -----------------
 * Print out a cheery welcome message.
 */
void welcome() {
   cout << "Welcome!  You're about to play an intense game ";
   cout << "of mind-numbing Boggle.  The good news is that ";
   cout << "you might improve your vocabulary a bit.  The ";
   cout << "bad news is that you're probably going to lose ";
   cout << "miserably to this little dictionary-toting hunk ";
   cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: yesNo
 * -----------------
 * Administers yes/no questions
 */
bool yesNo(){
	string answer=toUpperCase(getLine());
	if (answer == "YES" || answer == "Y") return true;
	if (answer == "NO" || answer == "N") return false;
	else {
		cout << "That was a yes or no question, dear."<<endl
			<<"Try again: ";
		return yesNo();
	}
}

/*
 * Function: giveInstructions
 * --------------------------
 * Print out the instructions for the user.
 */
void giveInstructions() {
   cout << endl;
   cout << "The boggle board is a grid onto which I ";
   cout << "will randomly distribute cubes. These ";
   cout << "6-sided cubes have letters rather than ";
   cout << "numbers on the faces, creating a grid of ";
   cout << "letters on which you try to form words. ";
   cout << "You go first, entering all the words you can ";
   cout << "find that are formed by tracing adjoining ";
   cout << "letters. Two letters adjoin if they are next ";
   cout << "to each other horizontally, vertically, or ";
   cout << "diagonally. A letter can only be used once ";
   cout << "in each word. Words must be at least four ";
   cout << "letters long and can be counted only once. ";
   cout << "You score points based on word length: a ";
   cout << "4-letter word is worth 1 point, 5-letters ";
   cout << "earn 2 points, and so on. After your puny ";
   cout << "brain is exhausted, I, the supercomputer, ";
   cout << "will find all the remaining words and double ";
   cout << "or triple your paltry score." << endl << endl;
   cout << "Press Enter to continue...";
   getLine();
}

/*
 * Function: chooseSize
 * -----------------
 * Administers yes/no questions
 */
int chooseSize(){
	int size = getInteger("What size board would you like to play on? ");
	if (size == 4 || size == 5) return size;
	cout << "Sorry, 4x4 and 5x5 is all I can do.\n";
	chooseSize();
}
/*
 * Function: createBoard()
 * --------------------------
 * Create the board by randomly arranging the cubes and randomly picking a face for each.
 * Return the grid of chosen characters and display graphically.
 */
Grid<char> createBoard(int size){
	//setRandomSeed(1); //makes run behavior identical each time through
	//the final output
	Grid<char> board(size, size);
	/*fill a vector with the appropriate cubes*/
	Vector<string> myVector;
	if (size==4){
		int length = sizeof STANDARD_CUBES/sizeof STANDARD_CUBES[0];
		for (int i = 0; i < length; i++)
			myVector.add(STANDARD_CUBES[i]);
	}
	if (size==5){
		int length = sizeof BIG_BOGGLE_CUBES/sizeof BIG_BOGGLE_CUBES[0];
		for (int i = 0; i < length; i++)
			myVector.add(BIG_BOGGLE_CUBES[i]);
	}
	 /*randomly reorder the cubes*/
	for (int i = 0; i < myVector.size(); i++){
		int r = randomInteger(i,myVector.size()-1);
		string cubeI = myVector.get(i);
		string cubeR = myVector.get(r);
		myVector.set(i,cubeR);
		myVector.set(r,cubeI);
	}
	/*randomly select which face is up for each cube*/
	for (int i = 0; i < myVector.size(); i++){
		string letters = myVector.get(i);
		int r = randomInteger(0,5);
		string choice = letters.substr(r,1);
		myVector.set(i, choice);
	}
	/*convert vector<string> to grid<char> and apply the values to the graphic cubes*/
	int index = 0;
	for (int row = 0; row < (int)sqrt((double)myVector.size()); row++){
		for( int col = 0; col < (int)sqrt((double)myVector.size()); col++){
			string letterStr = myVector.get(index);
			board[row][col] = letterStr[0];
			labelCube(row,col,letterStr[0]);
			index++;
		}
	}	
	return board;
}

/*
 * Function: humanPlays()
 * --------------------------
 * Manages the player's turn:
 * Allows the user to enter words, which are either accepted
 * or rejected by isValid(). 
 * highlights them and adds them to the scored list if appropriate
 * A blank line ends the turn.
 */
Lexicon humanPlays(Grid<char> & board, Lexicon & english){
	Lexicon usedWords;
	return humanPlays(board, english, usedWords);
}
Lexicon humanPlays(Grid<char> & board, Lexicon & english, Lexicon & usedWords){
	cout<<"\n\n\n\nAlright, you can go first. Type any words that you find and press Enter so I can check them. "<<
		"Pressing Enter on its own will end your turn. Good luck!\n\n\n"<<endl;
	//loop forever to accept words
	while(true){
		Grid<bool> usedLetters(board.numRows(),board.numCols());	//to flag
		string word = toUpperCase(getLine());
		//quit statement
		if (word == "")break;
		//check word validity
		if (isValid(word, board, usedLetters, usedWords, english)){
			//add to usedWords list and also to the scoring system
			usedWords.add(word);
			recordWordForPlayer(word, HUMAN);
			//highlight the letters
			for (int row = 0; row < usedLetters.numRows(); row++) {                   
				for (int col = 0; col < usedLetters.numCols(); col++) {               
					highlightCube(row, col, usedLetters[row][col]);
				}
			}
			pause(1000);	//leave them highlighted long enough to see
			//unhighlight the letters
			for (int row = 0; row < usedLetters.numRows(); row++) {                   
				for (int col = 0; col < usedLetters.numCols(); col++) {               
					highlightCube(row, col, false);
				}
			}
		}
	}
	return usedWords;
}

/*
 * Function: isValid()
 * --------------------------
 * Determines the validity of a word according to 4 requirements:
 * • It is at least four letters long.
 * • It has not already been included in the player’s word list (even if there is an alternate path on the
 *	 board to form the same word, the word is counted at most once).
 * • It is contained in the English lexicon.
 * • (function call) It can be formed on the board (i.e., it is composed of adjoining letters and each cube is used at most once).
 */
bool isValid(string& word, Grid<char>& board, Grid<bool>& usedLetters, Lexicon& usedWords, Lexicon& english){
	if (word.length()<4){
		cout<<"\tSorry, that word is too short (4 character minimum)"<<endl;
		return false;
	}
	if (usedWords.contains(word)){
		cout<<"\tSorry, that word has already been counted."<<endl;
		return false;
	}
	if (!english.contains(word)){
		cout<<"\tSorry, that word is not in my dictionary."<<endl;
		return false;
	}
	if(!canBeMade(word, board, usedLetters, 0, 0, board.numRows()-1)){
		cout<<"\tSorry, that word can't be made from the available board."<<endl;
		return false;
	}
	else return true;
}

/*
 * Function: canBeMade()
 * --------------------------
 * Recursively check to see if a word can be made on the board by 
 * checking each starting position and building the word out from there. 
 * cubesAway indicates the range of acceptable next letters. For starting cubes, any cube works.
 * After that, only cubes between the row/col immediately before/after the previous cube work.
 */
bool canBeMade(string word, Grid<char> & board, Grid<bool> & usedLetters, int row, int col, int spacesAway){
	if (word == "") return true;   
	else {
		//step through the board
		for (int r = row; r <= row + spacesAway; r++) {
			for (int c = col; c <= col + spacesAway; c++) {
				//if you find the right letter and it hasn't been used yet
				if (board.inBounds(r, c) && board[r][c] == word[0] && !usedLetters[r][c]) {
					//maintain a temporary usedLetter grid so only the letters used are highlighted and not all copies of the correct letter
					Grid<bool> tempUsedLetters = usedLetters;
					tempUsedLetters[r][c] = true;
					//recursively check all spaces around the previous letter
					if (canBeMade(word.substr(1), board, tempUsedLetters, r-1, c-1, 2)) {	//after a letter has been selected,
						//return the Grid<bool> by reference and also return true			//only check the 3x3 square surrounding the starting letter
						usedLetters = tempUsedLetters;
						return true;
					}
				}
			}
		}
		return false;
	}
}

/*
 * Function: computerPlays()
 * --------------------------
 * Manages the computer player's turn:
 * steps through starting positions, passing prefixes to findWords to discover possible words
 */
Lexicon computerPlays(Grid<char>& board, Lexicon& english, Lexicon& humanWords){
	Lexicon computerWords;
    Grid<bool> usedLetters(board.numRows(), board.numCols());	//to flag
	//step through board starting positions
    for (int row = 0; row < board.numRows(); row++){
        for (int col = 0; col < board.numCols(); col++){
			//attach a Grid<boolean> to each cube in case it can make a word
            Grid<bool> tempUsedLetters = usedLetters;
			tempUsedLetters[row][col] = true;
            string start;
            start = board[row][col];
			findWords(start, row, col, board, tempUsedLetters, english, computerWords, humanWords);               
        }
    }
    foreach (string word in computerWords) {                           
        recordWordForPlayer(word, COMPUTER);                           
    }
	return computerWords;
}

/*
* Function: findWords()
* --------------------------
* Finds all the words on the board originating from a specific coordinate.
*/
void findWords(string prefix, int row, int col, Grid<char>& board, Grid<bool>& usedLetters, Lexicon& english, Lexicon& computerWords, Lexicon& humansWords){
	//verify the current string is a prefix in the lexicon (trivial for single letters and true even if the prefix is the...
    if (!english.containsPrefix(prefix)) {	//...longest word containing it (e.g. english.containsPrefix("abdondonment") = true))                         
        return;
    }
	//if the current state of the word satisfies the requirements (4 ch, not used already and english) add it to the list
	else if (prefix.length() >= 4 && !humansWords.contains(prefix) && english.contains(prefix)) {
        computerWords.add(prefix);
	}
	/*continue trying to add onto the prefix using surrounding characters*/
	//step through surrounding characters
	for (int r = row - 1; r <= row + 1; r++) {
		for (int c = col - 1; c <= col + 1; c++) {
           //if you can use it...
            if (board.inBounds(r, c) && !usedLetters[r][c]) {           
                //add a character...                               
                string tempPrefix = prefix;
                tempPrefix += board[r][c];
				//and update the Grid<bool>
				Grid<bool> tempUsedLetters = usedLetters;
                tempUsedLetters[r][c] = true;
				//recursively call findWords() with the new prefix and grid<bool>
				findWords(tempPrefix, r, c, board, tempUsedLetters, english, computerWords, humansWords);       
            }
        }
    }
}

/*
* Function: compareScores
* --------------------------
* reports user's find rate for the current and previous round, including a best and average result as well.
* the user is complimented or patronized based on their performance. 
*/
void compareScores(Lexicon& humanWords, Lexicon& computerWords, float& count, float& previous, float& average, float& best){
	count++;
	int oldBest=best;
	float score = 100*((float)humanWords.size())/((float)humanWords.size()+(float)computerWords.size());
	average = (average*(count-1)+score)/count;
	if (score>best) best = score;
	cout<<"Words found:\t"<<score<<"%"<<endl;
	cout<< "Best:\t\t" << best <<"%"<<endl;
	cout<< "Last Round:\t" << previous <<"%"<<endl;
	cout<< "Average:\t" << average <<"%"<<endl;
	cout<< "Games Played:\t" << count <<endl<<endl;
	if (score>oldBest)
		cout << "That's your best yet! Congratulations!" << endl ;
	else if (score<best && score>average)
		cout << "Above average! Nice!" << endl ;
	else if (score<best && score>previous)
		cout << "Better than last round! Good job!" << endl ;
	else cout << "Are you even trying?"<<endl;
	previous = score;
	cout<< "\tCare for another round? ";
}