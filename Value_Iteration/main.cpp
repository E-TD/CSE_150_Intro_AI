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

	float rewards[81];
	void getRewards();

	float a1[81][81];
	float a2[81][81];
	float a3[81][81];
	float a4[81][81];

	float** v;
	float vStar[81];
	int piStar[81];
	float gamma = 0.9925;

	void getA1();
	void getA2();
	void getA3();
	void getA4();

	void valueIteration();
	void valueIteration2();

private:

};

Master::Master() {

	float** ary = new float*[81];
	for (int i = 0; i < 81; i++) {
		ary[i] = new float[4000];
	}
	v = ary;

	for (int i = 0; i < 81; i++) {
		for (int j = 0; j < 81; j++) {
			a1[i][j] = 0;
			a2[i][j] = 0;
			a3[i][j] = 0;
			a4[i][j] = 0;
		}
		v[i][0] = 0;
	}


	getRewards();
	getA1();
	getA2();
	getA3();
	getA4();
}

void Master::getRewards() {
	float rew;
	int index = 0;

	ifstream infile("rewards.txt");
	while (infile >> rew) {
		rewards[index] = rew;
		index++;
	}
	infile.close();
}

void Master::getA1() {
	int s, s_prime;
	float prob;

	ifstream infile("prob_a1.txt");
	while (infile >> s >> s_prime >> prob) {
		a1[s - 1][s_prime - 1] = prob;
	}
	infile.close();
}

void Master::getA2() {
	int s, s_prime;
	float prob;

	ifstream infile("prob_a2.txt");
	while (infile >> s >> s_prime >> prob) {
		a2[s - 1][s_prime - 1] = prob;
	}
	infile.close();
}

void Master::getA3() {
	int s, s_prime;
	float prob;

	ifstream infile("prob_a3.txt");
	while (infile >> s >> s_prime >> prob) {
		a3[s - 1][s_prime - 1] = prob;
	}
	infile.close();
}

void Master::getA4() {
	int s, s_prime;
	float prob;

	ifstream infile("prob_a4.txt");
	while (infile >> s >> s_prime >> prob) {
		a4[s - 1][s_prime - 1] = prob;
	}
	infile.close();
}

void Master::valueIteration() {
	for (int k = 1; k < 4000; k++) {
		float prevVal = 0;
		float newVal = 0;
		for (int s = 0; s < 81; s++) {
			float accum1 = 0;
			float accum2 = 0;
			float accum3 = 0;
			float accum4 = 0;

			for (int sp = 0; sp < 81; sp++) {
				float vTemp = v[sp][k - 1];

				float temp1 = a1[s][sp];
				if (temp1 > 0) {
					accum1 += temp1 * vTemp;
				}

				float temp2 = a2[s][sp];
				if (temp2 > 0) {
					accum2 += temp2 * vTemp;
				}

				float temp3 = a3[s][sp];
				if (temp3 > 0) {
					accum3 += temp3 * vTemp;
				}

				float temp4 = a4[s][sp];
				if (temp4 > 0) {
					accum4 += temp4 * vTemp;
				}
			}

			float maxA = max(max(accum1, accum2), max(accum3, accum4));
			v[s][k] = rewards[s] + (gamma * maxA);
		}
	}

	for (int s2 = 0; s2 < 81; s2++) {
		vStar[s2] = v[s2][3999];
		cout << " (" << s2 + 1 << ") : " << vStar[s2] << endl;
	}
}

void Master::valueIteration2() {
	for (int i = 0; i < 81; i++) {
		int toAssign = 0;
		float accum1 = 0;
		float accum2 = 0;
		float accum3 = 0;
		float accum4 = 0;
		for (int j = 0; j < 81; j++) {
			float vTemp = vStar[j];

			float temp1 = a1[i][j];
			if (temp1 > 0) {
				accum1 += temp1 * vTemp;
			}

			float temp2 = a2[i][j];
			if (temp2 > 0) {
				accum2 += temp2 * vTemp;
			}

			float temp3 = a3[i][j];
			if (temp3 > 0) {
				accum3 += temp3 * vTemp;
			}

			float temp4 = a4[i][j];
			if (temp4 > 0) {
				accum4 += temp4 * vTemp;
			}
		}

		if ((accum1 >= accum2) && (accum1 >= accum3) && (accum1 >= accum4)) {
			toAssign = 1;
			cout << " (" << i + 1 << ") : WEST" << endl;
		}
		else if ((accum2 >= accum1) && (accum2 >= accum3) && (accum2 >= accum4)) {
			toAssign = 2;
			cout << " (" << i + 1 << ") : NORTH" << endl;
		}
		else if ((accum3 >= accum1) && (accum3 >= accum2) && (accum3 >= accum4)) {
			toAssign = 3;
			cout << " (" << i + 1 << ") : EAST" << endl;
		}
		else if ((accum4 >= accum1) && (accum4 >= accum2) && (accum4 >= accum3)) {
			toAssign = 4;
			cout << " (" << i + 1 << ") : SOUTH" << endl;
		}
		piStar[i] = toAssign;
	}
}

int main() {
	Master masterBlaster;
	cout << " Part A: Optimal State Value Function" << endl;
	masterBlaster.valueIteration();
	cout << endl << endl << " Part B: Optimal Policy" << endl;
	masterBlaster.valueIteration2();

	for (int i = 0; i < 81; ++i) {
		delete[] masterBlaster.v[i];
	}
	delete[] masterBlaster.v;

	cin.get();
	return 0;
}
