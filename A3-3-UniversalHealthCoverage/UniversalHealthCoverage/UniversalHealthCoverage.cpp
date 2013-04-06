/* File: UniversalHealthCoverage.cpp
 * ----------------------------
 * Name: Nathan Hayes-Roth
 * Section: Linford
 *
 * A program that determines whether or not it is possible to provide
 * coverage to everyone using a fixed number of hospitals.
 *
 * I couldn't get this one to work, unfortunately.
 */
#include <iostream>
#include <string>
#include "set.h"
#include "vector.h"
#include "console.h"
#include "random.h"
using namespace std;


bool canOfferUniversalCoverage(Set<string> & cities,			//set of all cities
                               Vector<Set<string> > & locations,//list of cities-covered-by-each hospital
                               int numHospitals,				//maximum number of hospitals
                               Vector<Set<string> > & result);	//if all cities can be covered, update with a solution


int main() {
	Set<string> cities;
	string A = "A";
	string B = "B";


	cities += "A";

	int numHospitals = 1;

	Vector< Set<string> > locations (numHospitals);

	locations[0]+= A;
	locations[1]+= B;

	Vector< Set<string> > result(numHospitals);

	canOfferUniversalCoverage(cities, locations, numHospitals, result);

	return 0;
}

/* Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
							   Vector< Set<string> >& locations, 
							   int numHospitals, 
							   Vector< Set<string> >& result) {
	/*base case*/
	if(cities.isEmpty()){
		cout<<"true";
		return true;
    }
    
	/*other unique scenarios*/
    if(numHospitals == 0 || locations.isEmpty()){
		return false;
    }

	/*check if all the cities are a subset of the result (which would imply success)*/
	Set<string> sum;
	if (!result[1].isEmpty()){
		for (int i=0; i<result.size();i++)
			sum+=(result[i]);
		if (cities.isSubsetOf(sum)){
			cout<<"true";
			return true;	//SUCCESS
		}
	}

	/*recursively try combinations*/
	for (int i=0; i<numHospitals; i++){//try as many hospitals as you can
		if (result[i].isEmpty()){//find an empty slot
			for (int k=0; k<locations.size(); k++){//try adding each of the other hospitals
				result[i]=(locations[k]);
				canOfferUniversalCoverage(cities, locations, numHospitals, result);
			}
		}
	}
	canOfferUniversalCoverage(cities, locations, numHospitals, result);
}
	