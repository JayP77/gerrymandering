//  Project 1 Starter Code - GerryMandering
//  Jay Patel
//  Net ID : Jpate260
//  CS 251 data strcutures
//  This program will read in 2 files and use that data to check if the state
//  is gerrymandering or not along with other functions such as displaying
//  a plot and a creative componenet.

#include "ourvector.h"
#include <string>
#include <fstream>
#include <math.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>


using namespace std;
//
// This struct stores data from district.txt files
//
struct districtInfo{
    	string stateName;
    	int districtNumb;
    	ourvector<int> demV;
    	ourvector<int> repV;
};

//
//  This struct stores data from eligible_voters.txt files
//
struct eligibleInfo{
	int eligibleVoters;
	string stateName2;
};

//
//  getState will get the state name from each file and delete it from
//  list by parsing through 0 index until comma
//
string getState(string &line) {
	size_t pos = line.find(",");
	string stateName = line.substr(0, pos);
	line = line.substr(pos+1, line.size()-stateName.size()-1);
	return stateName;
}  //  end of getState

//
// getEligibleVote will get the number of eligible voters from
// eligible_voters.txt files by parsing commas
//
int getEligibleVote(string &line) {
	size_t pos = line.find(",");
	string eligibleStr = line.substr(0, pos);
	line = line.substr(pos+1, line.size()-eligibleStr.size()-1);
	int eligibleVote = stoi(eligibleStr);
	return eligibleVote;
}  //  end of getEligibleVote

//
//  getDistrictData will parse thorugh commas to store and delete district
//  number democrat votes and republican votes and keep looping in that order
//  until the line ends
//
int getDistrictData(string &line, ourvector<districtInfo> &district, ourvector<int> &demV, ourvector<int> &repV) {
	int districtNumb;
	while (!line.empty()) {
		size_t pos = line.find(",");
		if(line.substr(0, pos) == "AL") {
			line.replace(0, pos, "1");
			pos = line.find(",");
		}
		string strDistrict = line.substr(0, pos);
		line = line.substr(pos+1, line.size()-strDistrict.size()-1);
		pos = line.find(",");
		string dVotes;
		string rVotes;
		if ((int)pos == -1) {
            line = "";
		} else {
		dVotes = line.substr(0, pos);
		line = line.substr(pos+1, line.size()-dVotes.size()-1);
		pos = line.find(",");
		rVotes = line.substr(0, pos);
		line = line.substr(pos+1, line.size()-rVotes.size()-1);
		districtNumb = stoi(strDistrict);
		repV.push_back(stoi(rVotes));  //  access and stores in vecotor
		demV.push_back(stoi(dVotes));  //  access and stores in vecotor
		}
	}

	return districtNumb;
}  // end of getDistrictData

//
//  loadFile will read in all files requested to load once and then store it
//  inside ourvector structs
//
void loadFile(ourvector <districtInfo>&district, string &fileName, ourvector <eligibleInfo>&eligible) {
	ifstream infile(fileName);
	string line;
	if(!infile.is_open()) {
		cout << "file is not found" << endl;
	}
	line = "";
	getline(infile, line, '\n');
	while(!infile.eof()) {
		if(fileName == "eligible_voters.txt" ||
		fileName == "eligible_voters_fantasy.txt" ||
		fileName == "eligible_voters_short.txt") {
			eligibleInfo eligibleV;
			eligibleV.stateName2 = getState(line);
			eligibleV.eligibleVoters = getEligibleVote(line);
			if(eligibleV.eligibleVoters != 281000) {
				eligible.push_back(eligibleV);
			}
			getline(infile, line, '\n');
		} else if (fileName == "districts.txt" ||
		fileName == "districts_fantasy.txt" ||
		fileName == "districts_short.txt") {
		ourvector<int> demV;
    	ourvector<int> repV;
		districtInfo infoDist;
		infoDist.stateName = getState(line);
		infoDist.districtNumb = getDistrictData(line, district, demV, repV);
		infoDist.demV = demV;
		infoDist.repV = repV;
		getline(infile, line, '\n');
		district.push_back(infoDist);
		}
	}
}  //  end of loadFile

//
//  search will search the state user inputted and return both the index for it
//  on district.txt files and eligible_voters.txt files
//
int search(ourvector<districtInfo>&district, string stateToSearch, string &state, ourvector <eligibleInfo>&eligible, int &indE, int &indD) {
	int x = district.size();
	for(int i = 0; i < x; i++) {
		if(district[i].stateName == stateToSearch) {  //  index of district
			state = district[i].stateName;
			indD = i;
		}
	}
	for(int i = 0; i < x; i++) {
		if(eligible[i].stateName2 == stateToSearch) {  // index of eligible
			state = eligible[i].stateName2;
			indE = i;
			return i;
		}
	}
	for(int i = 0; i < x; i++) {
		if(district[i].stateName != stateToSearch) {
				cout << "State does not exist, search again.\n" << endl;
				return -1;
			}
	}
	return -1;
}  //  end of search

//
//  showStats will display stats of the specific state user has searched by
//  using the index returned from search function
//
void showStats(ourvector<districtInfo> &district, ourvector <eligibleInfo>&eligible, ourvector<int> &demV, ourvector<int> &repV, int indE, int indD) {
	int totalVotes = 0;
	int wastedRep = 0;
	int wastedDem = 0;
	int wastedRepT = 0;
	int wastedDemT = 0;
	int tempTotal = 0;
	int x = 0;
	int r = 0;
	int d = 0;
	while(x < district[indD].demV.size()) {
		d = district[indD].demV[x];
		r = district[indD].repV[x];
		tempTotal = d + r;
		totalVotes = totalVotes + d + r;
		if(d == r) {
			wastedDem = wastedDem;
			wastedRep = wastedRep;
		} else if (d > r) {
			wastedDemT = d - ((tempTotal/2)+1);
			wastedRep = r + wastedRep;
			wastedDem = wastedDemT + wastedDem;
		} else {
			wastedRepT = r - ((tempTotal/2)+1);
			wastedDemT = d;
			wastedRep = wastedRepT + wastedRep;
			wastedDem = d + wastedDem;
		}
		x++;
	}
		string gManAgainst;
		double eff;
		eff = (abs(double(wastedDem) - double(wastedRep))/double(totalVotes)) * 100;
		string GerryMandering;
		if(eff >= 7.00 && district[indD].districtNumb >= 3) {  
			GerryMandering = "Yes";
			cout <<"Gerrymandered: " << GerryMandering << endl;
			if(wastedDem > wastedRep) {
				cout << "Gerrymandered against: Democrats" << endl;
			} else {
				cout << "Gerrymandered against: Republicans" << endl;
			}
			cout << "Efficiency Factor: " << eff << "%" << endl;
		} else if (eff < 7.00 || district[indD].districtNumb < 3) {
			GerryMandering = "No";
			cout <<"Gerrymandered: " << GerryMandering << endl;
		}
		cout << "Wasted Democratic votes: " << wastedDem << endl;
		cout << "Wasted Republican votes: " << wastedRep << endl;
		cout << "Eligible voters: " << eligible[indE].eligibleVoters << endl;
		cout << endl;
}  //  end of showStats

//
//  plotData will plot data a total of 100 charectors either D or R respective
//  to the percentage of voters per district
//
void plotData(ourvector<districtInfo> &district, ourvector<int> &demV, ourvector<int> &repV, int indE, int indD) {
	int x = 0;
	int districtNumber = 1;
	while(x < district[indD].demV.size()) {
		double d = 0;
		double r = 0;
		double percentageD = 0.00;
		double percentageR = 0.00;
		int total = 0;
		int j = 0;
		int k = 0;
		d = district[indD].demV[x];
		r = district[indD].repV[x];
		total = d + r;
		percentageD = ((d/total)*100);
		percentageD = trunc(percentageD);
		percentageR = 100 - percentageD;
		cout << "District: " << districtNumber << endl;
		while(k < percentageD) {
			cout << "D";
			k++;
		}
		while(j < percentageR) {
			cout << "R";
			j++;
		}
		 cout << endl;
		districtNumber++;
		x++;
	}
	cout << endl;
}  //  end of plotData

//
//  votingInfo is a creative component function this function will calculate
//  the percentage of eligible voters that did not vote by state
//
void votingInfo(ourvector<districtInfo> &district, ourvector<int> &demV, ourvector<int> &repV, int indE, int indD, ourvector <eligibleInfo>&eligible) {
	int d = 0;
	int r = 0;
	int total = 0;
	int tempTotal = 0;
	int x = 0;
	while(x < district[indD].demV.size()) {
		d = district[indD].demV[x];
		r = district[indD].repV[x];
		tempTotal = d + r;
		total = total + tempTotal;
		x++;
	}
	double noVote;
	int temp;
	temp = eligible[indE].eligibleVoters - total;
	noVote = (double(temp)/double(eligible[indE].eligibleVoters)) * 100;
	cout << "Percentage of people that didn't Vote: " << noVote << "%" << endl;
	cout << endl;
}  //  end of votingInfo

int main() {
    string choice;
    string fileName;
    string fileName2;  // acts as a temporary file until fileName is done
    string state = "N/A";
    string searchV = "No";  //  used to indicate if search is done
    int indE = -1;  //  index for eligibleVoters
    int indD = -1;  //  index for district
    bool terminate = false;
    string dataLoaded = "No";
    ourvector <districtInfo> district;  // vector for district.txt files
    ourvector <eligibleInfo> eligible;  // vector for eligible_voters.txt files
    ourvector <int> demV;  //  used for storing democratic votes per district
    ourvector <int> repV;  //  used for storing republican votes per district
    cout << "Welcome to the Gerrymandering App!\n" << endl;
    do {
    	cout << "Data loaded? " << dataLoaded << endl;
    	cout << "State: " << state << "\n" << endl;
    	cout << "Enter command: ";
    	cin >> choice;
    	if(choice == "search") {
    		cin.ignore();
    	}
    	cout << "\n-----------------------------" <<endl;
    	cout << endl;
    	if(choice == "Exit" || choice == "exit") {
    		terminate = true;
    	}
    	if(choice == "load" || choice == "Load") {
    		cin >> fileName;
    		cin >> fileName2;
    		bool fileOneLoaded = false;
    		if(district.size() > 0) {
				cout << "Already read data in, exit and start over.\n" << endl;
			} else if (fileName == "districts.txt" ||
    		fileName == "districts_fantasy.txt" ||
    		fileName == "districts_short.txt") {
    			loadFile(district, fileName, eligible);
    			if(district.size()> 0) {
	    			dataLoaded = "Yes";
	    		}
	    		fileOneLoaded = true;
				cout << "Reading: " << fileName << endl;
	    		for (auto &e : district) {
					cout <<"..." << e.stateName << "..." << e.districtNumb;
					cout << " districts total" << endl;
				}
				cout << endl;
    		} else {
    			cout << "Invalid first file, try again.\n" << endl;
    		}
			fileName = fileName2;
			if(fileOneLoaded == true) {
				if(eligible.size() > 0) {
					dataLoaded = dataLoaded;
				} else if (fileName == "eligible_voters.txt" ||
				fileName =="eligible_voters_fantasy.txt" ||
				fileName == "eligible_voters_short.txt") {
	 				loadFile(district, fileName, eligible);
					cout << "Reading: " << fileName2 << endl;
					for (auto &a : eligible) {
						cout << "..." << a.stateName2 << "..." << a.eligibleVoters;
						cout << " eligible voters" << endl;
					}
					cout << endl;
				} else {
	    			cout << "Invalid second file, try again.\n" << endl;
	    			dataLoaded = "No";
	    		}
			}
    	}
    	if(choice == "search") {
    		string stateToSearch;
    		getline(cin, stateToSearch);
    		if(dataLoaded == "No") {
    			cout << "No data loaded, please load data first." << endl;
    		}
    		transform(stateToSearch.begin(), stateToSearch.end(), stateToSearch.begin(), :: tolower);
    		int pos = stateToSearch.find(" ");
    		stateToSearch[0] = toupper(stateToSearch[0]);
    		stateToSearch[pos+1] = toupper(stateToSearch[pos+1]);
    		int pos1 = stateToSearch.find(" ", pos + 1);
    		stateToSearch[pos1+1] = toupper(stateToSearch[pos1+1]);
    		search(district, stateToSearch, state, eligible, indE, indD);
    		searchV = "Yes";
    	}
    	if(choice == "stats") {
    		if(dataLoaded == "No") {
    			cout << "No data loaded, please load data first.\n" << endl;
    		} else if (indD <= 0) {
    			cout << "No state indicated,";
    			cout << " please search for state first.\n" << endl;
    		} else {
    			showStats(district, eligible, demV, repV, indE, indD);
    		}
    	}
    	if(choice == "plot") {
    		if(dataLoaded == "No") {
    			cout << "No data loaded, please load data first.\n" << endl;
    		} else if (indD <= 0) {
    			cout << "No state indicated,";
    			cout << " please search for state first.\n" << endl;
    		} else {
    			plotData(district, demV, repV, indE, indD);
    		}
    	}
    	if(choice == "votedata") {
    		votingInfo(district, demV, repV, indE, indD, eligible);
    	}
    } while (terminate != true);
    return 0;
}  //  end of int main

