#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <queue>
using namespace std;

struct Word {
	string word;
	float count;
	float prob;
};
class CompareWords {
public:
	bool operator()(Word& w1, Word& w2)
	{
		if (w1.prob < w2.prob) return true;
		return false;
	}
};

Word *words;
int wordSize;
float totalProb;

void calculations(char& letter, float& probability, string correct, string incorrect) {
	float * pli;
	float pewtop;
	float pewbot = 0;
	float pwe = 0;
	float sum = 0;
	float plipwe = 0;
	float letters[26];
	float * top;
	float * total;
	Word word;
	for (int i = 65; i < 91; i++) {
		pli = new float[wordSize];
		top = new float[wordSize];
		for (int j = 0; j < wordSize; j++) {
			word = words[j];
			pli[j] = 0;
			int index = 0;
			for (char& c : word.word) {
				if (c == (char)i && correct[index] == '-') {
					pli[j] = 1;
				}
				index++;
			}

			// First case if just starting out
			if (incorrect.size() == 0 && correct == "-----") {

				top[j] = word.prob;
				pewbot = totalProb;
			}

			// Other cases
			else {
				top[j] = word.prob;
				int index = 0;
				for (char& c : word.word) {
					for (char& inc : incorrect) {
						if (c == inc) {
							top[j] = 0;
						}
					}
					if (correct[index] != '-' && c != correct[index]) {
						top[j] = 0;
					}
					index++;
				}
				pewbot += top[j];
			}

		}

		// Post calculations
		for (int j = 0; j < wordSize; j++) {
			word = words[j];
			//cout << pewbot << endl;
			// cout << top[j] << endl;
			pwe = top[j] / pewbot;
			// cout << pwe << endl;
			plipwe = pli[j] * pwe;
			sum += plipwe;

		}
		cout << " sum is: " << sum << endl;
		letters[i - 65] = sum;
		delete[] pli;
		delete[] top;
		pewbot = 0;
		sum = 0;
	}
	for (int i = 0; i < 26; i++) {
		cout << letters[i] << endl;
	}

}

int main() {
	ifstream infile("hw2_word_counts_05.txt");
	priority_queue<Word, vector<Word>, CompareWords> probQueue;
	unordered_map<string, int> wordCounts;
	unordered_map<string, int>::iterator itr;
	string word;
	int count;
	int totalNum = 0;
	float prob;

	wordSize = 0;
	totalProb = 0;
	while (infile >> word >> count) {
		totalNum += count;
		wordCounts.insert(make_pair(word, count));
		wordSize++;
	}
	words = new Word[wordSize];
	int i = 0;
	for (itr = wordCounts.begin(); itr != wordCounts.end(); itr++) {
		prob = (float)itr->second / totalNum;
		totalProb += prob;
		Word newWord;
		newWord.word = itr->first;
		newWord.count = itr->second;
		newWord.prob = prob;
		words[i] = newWord;
		i++;
	}
	for (i = 0; i < wordSize; i++) {
		probQueue.push(words[i]);
	}
	while (!probQueue.empty()) {
		Word w1 = probQueue.top();
		// cout << "Probability of " << w1.word << "is: " << w1.prob << endl;
		probQueue.pop();
	}

	char* letter = NULL;
	float* probability = NULL;

	string correct;
	string incorrect;

	/* Case 1*/
	correct = "-----";
	incorrect = "";
	calculations(*letter, *probability, correct, incorrect);

	/* Case 6 */
	correct = "-----";
	incorrect = "EO";
	calculations(*letter, *probability, correct, incorrect);

	/* Case 7 */
	correct = "D--I-";
	incorrect = "";
	calculations(*letter, *probability, correct, incorrect);

	/* Case 8 */
	correct = "D--I-";
	incorrect = "A";
	calculations(*letter, *probability, correct, incorrect);

	/* Case 9 */
	correct = "-U---";
	incorrect = "AEIOS";
	calculations(*letter, *probability, correct, incorrect);

	delete[] words;
	cin.get();
	return 0;


}