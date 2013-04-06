/* 
 * Nathan Hayes-Roth
 * CS106B
 * Assignment 3-1
 * -----------------
 * Cell phone mind reading: this program uses recursion to decipher keypad inputs, determining
 * which words the user could be trying to spell.
 */
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"

using namespace std;

/*constants*/
static Lexicon lex("EnglishWords.dat");
static string letterSets[10] = {"", "", "abc", "def", "ghi", 
								"jkl", "mno", "pqrs", "tuv", "wxyz"};	//0 and 1 have no letter translation

/*prototypes*/
void ListCompletions(string digits, Lexicon & lex); 

/*Main Function*/
int main() {
	while (true){
		string digits = getLine("Please enter a digit sequence: ");
		for (int i=0;i<(int)digits.length();i++){
			if(digits[i]=='0'||digits[i]=='1'){
				cout << "0 and 1 do not translate to letters."<<endl<<endl;
			}
		}
		ListCompletions(digits, lex);
	}
	return 0;
}

/* ListCompletions(string, Lexicon):
 * Prints all words from the lexicon that can be formed by extending
 * the given digit sequence.
 */
void ListCompletions(string digits, Lexicon & lex) {
	int index=0;
	
	/*find the first digit*/
	while(index<(int)digits.length() && !isdigit(digits[index]))
		index++;

	/*Don't start checking until the digits have been entirely converted to letters*/
	if(index == digits.length() ){
		string word = digits; //redundant, but helps me understand
		/*Lexicon doesn't containPrefix=>these aren't the droids I'm looking for*/
		if(!lex.containsPrefix(word)) return;
		
		/*Otherwise check if the current word is in the lexicon and print it if it is*/
		if( lex.contains(word)){
			cout << "\t" << word << endl;
		}

		/*...and then try recursively checking after adding each letter*/
		for( char ch='a'; ch<='z'; ++ch){
			string newWord = digits + ch;
			ListCompletions(newWord, lex);
		}
		return;
	}

	/*make a prefix out of the current letters, check what could come next, and recursively check each*/
	string prefix = digits.substr(0, index);
	string digitLetters = letterSets[digits[index] - '0'];	//subtracting '0' converts digits[index] a char to an int
	int i=0;												//not intuitive at all
	while(i<digitLetters.length()){
		ListCompletions(prefix + digitLetters[i] + digits.substr(index + 1), lex );
		i++;
	}
}