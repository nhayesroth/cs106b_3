/*
 * File: MakeChange.cpp
 * ----------------------------
 * Name: Nathan Hayes-Roth
 * Section: Linford
 * makeChange takes an amount of money (in cents) and a vector of coin denominations
 * and returns the smallest number of coins needed to reach the amount.
 */

#include <iostream>
#include <string>
#include <vector>
#include "console.h"
#include "simpio.h"

using namespace std;
int MakeChange(int amount, vector<int>& denominations, int start);
int MakeChange(int amount, vector<int>& denominations);

int main(){
    vector<int> denominations;
	/*add all denominations*/
	denominations.push_back(25); denominations.push_back(10); denominations.push_back(5); denominations.push_back(1);
    while(true){
        int amount = getInteger("How many cents do you need to make change for? ");
        cout << "That would take " << MakeChange(amount, denominations)<< " coins." << endl << endl;
    }
    return 0;
}

/* MakeChange is broken into two functions, one with an index and one without. 
 * Together, they calculate the minimum number of coins (given a vector of
 * denominations) needed to sum to a specified amount.
 * ---------------------------------------------------------------------------
/* calling MakeChange with only an amount and vector of denominations,
 * as in main(), calls the other version with an initial index of 0
 */
int MakeChange(int amount, vector<int>& denominations){
    return MakeChange(amount, denominations, 0);
}
int MakeChange(int amount, vector<int>& denominations, int index){	
	int count = -1;

	/*base case*/
	if(amount == 0) return 0;
	
	/*amount cannot be made*/
    if(amount < 0) return -1;
	if(index == denominations.size()) return -1;

	/*recursively solve for the the remaining amount of change by checking progressively smaller denominations*/

	/*only increase the count if using the largest coin does not cause the amount to become negative
	 * maintain the index in case multiples of a coin are necessary (e.g. .50 needs 2 quarters)
	 */
    int with = MakeChange(amount - denominations[index], denominations, index);
    if(with != -1)
        count = 1 + with;

	/*otherwise, increase the index to try smaller denominations*/
    int without = MakeChange(amount, denominations, index +1);
    if((count == -1)||(without < count && without != -1))
        count = without;

	/*continue until the amount reaches 0, then return out*/
    return count;
}