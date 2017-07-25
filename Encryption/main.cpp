////////////////////////////////////////
// HW 6 Programming Assignment
// By: Eric Duong
// Class: CSE 150
////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <math.h>
#include <sstream>
#include <queue>

using namespace std;

class Master {
public:

	Master();

	int stateCount;
	int obsCount;
	int totalCount;

	int *charSequence;
	
	vector<float> initialD;
	void getInitialDistances();

	vector<vector<float>> transVec;
	void getTransitions();

	vector<vector<float>> emissVec;
	void getEmissions();

	vector<int> obsVec;
	void getObservations();

	void viterbiFunc();
	void revealMessage();

private:

};

Master::Master() {
	stateCount = 27;
	obsCount = 2;
	totalCount = 180000;

	getInitialDistances();
	getTransitions();
	getEmissions();
	getObservations();

	charSequence = new int[totalCount];
	for (int i = 0; i < totalCount; i++) {
		charSequence[i] = 0;
	}
}

void Master::getInitialDistances() {
	float dist;

	ifstream infile("initialStateDistribution.txt");
	while (infile >> dist) {
		initialD.push_back(log(dist));
	}
	infile.close();
}

void Master::getTransitions() {
	string trans;

	ifstream infile("transitionMatrix.txt");
	while (getline(infile, trans)) {
		int index = 0;
		stringstream ss(trans);
		vector<float> currLine;
		string nextVal;

		while (ss.good() && index < stateCount) {

			ss >> nextVal;
			float temp = log(stof(nextVal));

			currLine.push_back(temp);
			index++;
		}
		transVec.push_back(currLine);
	}
	infile.close();
}

void Master::getEmissions() {
	string emiss;
	
	ifstream infile("emissionMatrix.txt");
	while (getline(infile, emiss)) {
		int index = 0;
		stringstream ss(emiss);
		vector<float> currLine;
		string nextVal;
		
		while (ss.good() && index < obsCount) {
			ss >> nextVal;
			float temp = log(stof(nextVal));

			currLine.push_back(temp);
			index++;
		}
		emissVec.push_back(currLine);
	}
	infile.close();
}

void Master::getObservations() {
	string obs;

	ifstream infile("observations.txt");
	while (getline(infile, obs)) {
		int index = 0;
		stringstream ss(obs);
		string nextVal;

		while (ss.good() && index < totalCount) {
			ss >> nextVal;
			int temp = stoi(nextVal);

			obsVec.push_back(temp);
			index++;
		}
	}
	infile.close();
}

void Master::revealMessage()
{
	bool firstPass = true;
	int toAdd = 65;
	int temp = -1;

	for (int i = 0; i < totalCount + 1; i++) {
		char tempChar;

		// Reveal the secret message
		if (temp != charSequence[i]) {

			if (firstPass) {
				cout << endl << " ";
				firstPass = false;
			}

			// Check if the char is a space
			if (charSequence[i] != 26) {
				tempChar = (char)(toAdd + charSequence[i]);
				cout << tempChar << '|';
			}
			// Print letter out
			else {
				cout << '_';
			}

			temp = charSequence[i];
		}
	}

	cout << endl << endl << " Message Received." << endl;
}

void Master::viterbiFunc() {
	float maxP;
	int maxI;

	float resFloat = FLT_MAX;

	vector<float> dummy1;
	vector<vector<float>> probs;

	vector<int> dummy2;
	vector<vector<int>> prevProbs;

	int index = 0;
	while (index < stateCount) {
		dummy1.push_back((float) 0);
		dummy2.push_back(0);

		index++;
	}
	for (int i = 0; i < totalCount; i++) {
		probs.push_back(dummy1);
		prevProbs.push_back(dummy2);
	}

	// Base Case
	float tempIndex = obsVec[0];
	for (int i = 0; i < stateCount; i++) {
		float temp = initialD[i];
		float temp2 = emissVec[i][tempIndex];

		probs[0][i] = temp + temp2;
	}

	// t to t + 1 (Recursion)
	for (int i = 1; i < totalCount; i++) {
		for (int j = 0; j < stateCount; j++) {
			int obsInd = obsVec[i];
			float temp = emissVec[j][obsInd];

			maxI = -1;
			maxP = -resFloat;

			for (int k = 0; k < stateCount; k++) {
				float temp2 = transVec[k][j];
				float temp3 = probs[i - 1][k];

				if (maxP <= (temp2 + temp3)) {
					maxI = k;
					maxP = temp2 + temp3;
				}
			}

			prevProbs[i - 1][j] = maxI;
			probs[i][j] = temp + maxP;
		}
	}

	// Backtracking
	maxP = -resFloat;
	maxI = -1;

	for (int i = 0; i < stateCount; i++) {
		float temp = probs[totalCount - 1][i];

		if (maxP <= temp) {
			maxI = i;
			maxP = temp;
		}
	}

	// Base Case
	charSequence[totalCount - 1] = maxI;

	// t - 1 to 1
	for (int i = totalCount - 2; i >= 0; i--) {
		int seqIndex = charSequence[i + 1];
		int temp = prevProbs[i][seqIndex];

		if (temp != 0) {
			charSequence[i] = temp;
		}
	}
}

int main() {

	Master masterBlaster;
	masterBlaster.viterbiFunc();
	masterBlaster.revealMessage();

	cin.get();
	return 0;
}