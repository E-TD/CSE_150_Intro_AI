////////////////////////////////////////
// HW 4 Programming Assignment
// Unigrams & Bigrams
// By: Eric Duong
// Class: CSE 150
////////////////////////////////////////

#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <regex>
#include <sstream>
#include <math.h> 

using namespace std;

class bigWord {
public:
	string wordVal;
	double probability;
	bigWord(string val, double prob);
private:
};

bigWord::bigWord(string val, double prob) {
	wordVal = val;
	probability = prob;
}

class Master {
public:

	Master();

	pair<string, int> vocabList[500];
	void getVocab();

	int totalUnigramCount = 0;
	int unigramDict[500];
	void getUnigrams();

	int bigramLength = 0;
	tuple<int, int, int>* bigramDict;
	tuple<int, int, int>* getBigrams(tuple<int, int, int>* arr);

	void partA(char start);
	void partB(string word);
	void logLike(string sentence); // part C and part D
	void logLike(string sentence, bool partE);
	double logHelper(int parent, int child, bool partE);


private:

};

struct less_than_prob
{
	inline bool operator() (const bigWord& lhs, const bigWord& rhs)
	{
		return (lhs.probability > rhs.probability);
	}
} myObj;

Master::Master() {
	tuple<int, int, int>* bigramDict_temp = new tuple<int, int, int>[144981];

	getVocab();
	getUnigrams();
	bigramDict = getBigrams(bigramDict_temp);
}

void Master::getVocab() {
	string vocabWord;
	int index = 0;

	// Parse vocab file and store into map
	ifstream infile("vocab.txt");

	while (infile >> vocabWord) {
		for (int i = 0; i < vocabWord.length(); i++) {
			vocabWord[i] = tolower(vocabWord[i]);
		}
		vocabList[index] = make_pair(vocabWord, (index + 1));
		index++;
	}
}

void Master::getUnigrams() {
	int unigram;
	int index = 0;

	// Parse unigram file and store into map
	ifstream infile("unigram.txt");

	while (infile >> unigram) {
		unigramDict[index] = unigram;
		totalUnigramCount += unigram;
		index++;
	}
}

tuple<int, int, int>* Master::getBigrams(tuple<int, int, int>* arr) {
	int w1_index;
	int w2_index;
	int countW1W2;
	int index = 0;

	// Parse bigram file and store into map
	ifstream infile("bigram.txt");

	while (infile >> w1_index >> w2_index >> countW1W2) {
		arr[index] = make_tuple(w1_index, w2_index, countW1W2);
		index++;
	}

	bigramLength = index;
	return arr;
}

void Master::partA(char start) {
	for (int i = 0; i < 500; i++) {
		string temp = vocabList[i].first;
		if (temp[0] == (int) start) {
			double uni = (double) unigramDict[i];
			double pUni = (double) uni / totalUnigramCount;
			if (temp == "military" || temp == "minister") {
				cout << temp << " | P(Unigram) = " << pUni << endl;
			}
			else {
				cout << temp << "\t | P(Unigram) = " << pUni << endl;
			}
		}
	}
}

void Master::partB(string word) {
	int oneIndex;
	int oneUni;

	unordered_map<int, int> followIndexCount;
	unordered_map<int, int>::iterator it;

	vector<bigWord> finalList;

	int numHits = 0;

	for (int i = 0; i < 500; i++) {
		string temp = vocabList[i].first;
		if (temp == word) {
			oneIndex = vocabList[i].second;
			oneUni = unigramDict[i];
			break;
		}
	}

	for (int i = 0; i < bigramLength; i++) {
		tuple<int, int, int> temp = bigramDict[i];
		if (get<0>(temp) == oneIndex) {
			followIndexCount.insert(make_pair(get<1>(temp), get<2>(temp)));
			numHits++;
		}
	}

	for (it = followIndexCount.begin(); it != followIndexCount.end(); it++) {
		for (int i = 0; i < 500; i++) {
			if (vocabList[i].second == it->first) {
				string temp = vocabList[i].first;
				double w1w2Count = (double) it->second;
				double pBig = (double) w1w2Count / (double) oneUni;

				bigWord following = bigWord(temp, pBig);
				finalList.push_back(following);
			}
		}
	}

	sort(finalList.begin(), finalList.end(), myObj);

	for (vector<bigWord>::iterator it = finalList.begin(); it != finalList.begin() + 10; it++) {
		if (it->wordVal == "thousand") {
			cout << it->wordVal << " | P(Bigram) = " << it->probability << endl;
		}
		else {
			cout << it->wordVal << "\t | P(Bigram) = " << it->probability << endl;
		}
		
	}


}

double Master::logHelper(int parent, int child, bool partE) {

	bool found = false;
	int parentUni = unigramDict[parent - 1];
	double retVal = 0.0;

	for (int i = 0; i < bigramLength; i++) {
		tuple<int, int, int> temp = bigramDict[i];
		if (get<0>(temp) == parent && get<1>(temp) == child) {
			retVal = (double) get<2>(temp) / parentUni;

			if (!partE) {
				retVal = log(retVal);
			}
			found = true;
			break;
		}
	}

	if (!found) {
		cout << "The bigram (" << vocabList[child - 1].first << "|" << vocabList[parent - 1].first << ") was not found!" << endl;
	}

	return retVal;
}


void Master::logLike(string sentence) {

	bool notFound = false;

	double uTotal = 0;
	double bTotal = 0;
	
	for (int i = 0; i < sentence.length(); i++) {
		sentence[i] = tolower(sentence[i]);
	}
	sentence = sentence.substr(0, sentence.length() - 1);

	string buf; 
	stringstream ss(sentence); 
	list<string> tokens;
	list<int> indices;

	while (ss >> buf) {
		tokens.push_back(buf);
	}

	for (list<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		for (int i = 0; i < 500; i++) {
			if (*it == vocabList[i].first) {
				double uni = (double) unigramDict[i];
				double pUni = (double)uni / totalUnigramCount;
				uTotal += log(pUni);

				indices.push_back(vocabList[i].second);
			}
		}
	}

	for (int i = 0; i < 500; i++) {
		if (vocabList[i].first == "<s>") {
			indices.push_front(vocabList[i].second);
		}
	}

	int parent = 0;
	int child = 0;
	bool firstPass = true;
	
	for (list<int>::iterator it = indices.begin(); it != indices.end(); it++) {
		if (!firstPass) {
			parent = child;
			child = *it;

			double pBig = logHelper(parent, child, false);

			if (pBig == 0.0) {
				notFound = true;
			}

			bTotal += pBig;
		}
		else {
			child = *it;
			firstPass = false;
		}
	}


	cout << "LogLikelihood(Unigram) \t = " << uTotal << endl;
	
	if (notFound) {
		cout << "LogLikelihood(Bigram) \t = log(0) = -infinity, because we were unable to find certain bigrams." << endl;
	}
	else {
		cout << "LogLikelihood(Bigram) \t = " << bTotal << endl;
	}
	
}

void Master::logLike(string sentence, bool partE) {

	bool notFound = false;

	int uIndex = 0;
	int bIndex = 0;

	double lambdaArr[101];
	double uArr[6];
	double bArr[6];

	for (int i = 0; i < sentence.length(); i++) {
		sentence[i] = tolower(sentence[i]);
	}
	sentence = sentence.substr(0, sentence.length() - 1);

	string buf;
	stringstream ss(sentence);
	list<string> tokens;
	list<int> indices;

	while (ss >> buf) {
		tokens.push_back(buf);
	}

	for (list<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		for (int i = 0; i < 500; i++) {
			if (*it == vocabList[i].first) {
				double uni = (double)unigramDict[i];
				double pUni = (double)uni / totalUnigramCount;

				uArr[uIndex] = pUni;
				uIndex++;

				indices.push_back(vocabList[i].second);
			}
		}
	}

	for (int i = 0; i < 500; i++) {
		if (vocabList[i].first == "<s>") {
			indices.push_front(vocabList[i].second);
		}
	}

	int parent = 0;
	int child = 0;
	bool firstPass = true;

	for (list<int>::iterator it = indices.begin(); it != indices.end(); it++) {
		if (!firstPass) {
			parent = child;
			child = *it;

			double pBig = logHelper(parent, child, true);

			bArr[bIndex] = pBig;

			if (pBig == 0.0) {
				notFound = true;
			}

			bIndex++;
		}
		else {
			child = *it;
			firstPass = false;
		}
	}

	int lambIndex = 0;
	for (double lamb = 0.00; lamb < 1.00; lamb += 0.01) {
		double lambTotal = 0.00;
		for (int i = 0; i < 6; i++) {
			lambTotal += log((double)(((double) 1.00 - lamb) * uArr[i]) + (lamb * bArr[i]));
		}
		lambdaArr[lambIndex] = lambTotal;
		lambIndex++;

		cout << "Lambda(" << lamb << ") \t | P(Mixed) = " << lambTotal << endl;
	}

	double lambTotal = 0.00;
	for (int i = 0; i < 6; i++) {
		lambTotal += log((double)(((double) 1.00 - 0.999999) * uArr[i]) + (0.999999 * bArr[i]));
	}
	lambdaArr[lambIndex] = lambTotal;
	cout << "Lambda(" << 0.999999 << ") | P(Mixed) = " << lambTotal << endl;

}

int main() {

	Master masterBlaster;
	cout << endl << endl;

	// Part A
	cout << "Problem 4.3(a): " << endl;
	masterBlaster.partA('m');
	cout << endl << endl;

	// Part B
	cout << "Problem 4.3(b): " << endl;
	masterBlaster.partB("one");
	cout << endl << endl;

	// Part C
	cout << "Problem 4.3(c): " << endl;
	masterBlaster.logLike("The market fell by one hundred points last week.");
	cout << endl << endl;

	// Part D
	cout << "Problem 4.3(d): " << endl;
	masterBlaster.logLike("The fourteen officials sold fire insurance.");
	cout << endl << endl;

	// Part E
	cout << "Problem 4.3(e): " << endl;
	masterBlaster.logLike("The fourteen officials sold fire insurance.", true);
	cout << endl << endl;

	cin.get();
	return 0;
}