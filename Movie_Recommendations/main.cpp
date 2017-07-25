////////////////////////////////////////
// HW 5 Programming Assignment
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

class Master {
public:

	Master();

	int yVals[267];
	void getY();

	int xVals[267][23];
	void getX();

	double p_i[23];
	void setPI();
	void updatePI();

	double bigP[267];
	double noisyOr();

	int mistakes = 0;

private:

};

Master::Master() {
	getY();
	getX();
	setPI();
}

void Master::getY() {
	int yVal;
	int index = 0;

	ifstream infile("hw5_noisyOr_y.txt");

	while (infile >> yVal) {
		yVals[index] = yVal;
		index++;
	}
}

void Master::getX() {
	int x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23;
	int index = 0;

	ifstream infile("hw5_noisyOr_x.txt");

	while (infile >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10
		>> x11 >> x12 >> x13 >> x14 >> x15 >> x16 >> x17 >> x18 >> x19 >> x20 
		>> x21 >> x22 >> x23) {
		// lmaoooooooooooooooo
		xVals[index][0] = x1;
		xVals[index][1] = x2;
		xVals[index][2] = x3;
		xVals[index][3] = x4;
		xVals[index][4] = x5;
		xVals[index][5] = x6;
		xVals[index][6] = x7;
		xVals[index][7] = x8;
		xVals[index][8] = x9;
		xVals[index][9] = x10;
		xVals[index][10] = x11;
		xVals[index][11] = x12;
		xVals[index][12] = x13;
		xVals[index][13] = x14;
		xVals[index][14] = x15;
		xVals[index][15] = x16;
		xVals[index][16] = x17;
		xVals[index][17] = x18;
		xVals[index][18] = x19;
		xVals[index][19] = x20;
		xVals[index][20] = x21;
		xVals[index][21] = x22;
		xVals[index][22] = x23;

		index++;
	}
}

void Master::setPI() {
	for (int i = 0; i < 23; i++) {
		p_i[i] = (double)1 / 23;
	}
}

void Master::updatePI() {
	double numerator;
	double denom;
	double denom_2;
	double tSUM;
	double p_temp[23];
	int tVal;

	for (int i = 0; i < 23; i++) {
		tVal = 0;
		tSUM = 0;
		for (int t = 0; t < 267; t++) {
			numerator = (double) yVals[t] * xVals[t][i] * p_i[i];
			denom_2 = (double) pow((double)(1 - p_i[0]), xVals[t][0]);
			for (int j = 1; j < 23; j++) {
				denom_2 *= (double) pow((double)(1 - p_i[j]), xVals[t][j]);
			}
			denom = (double) 1 - denom_2;
			tSUM += (double)numerator / denom;
		}
		for (int t2 = 0; t2 < 267; t2++) {
			tVal += xVals[t2][i];
		}
		p_temp[i] = (double) ((double) 1 / tVal) * tSUM;
	}
	
	for (int i2 = 0; i2 < 23; i2++) {
		p_i[i2] = p_temp[i2];
	}
}

double Master::noisyOr() {
	double logVal = 0;
	double retVal;

	for (int t = 0; t < 267; t++) {
		int yTemp = yVals[t];
		double xTemp;
		double xTemp_2 = (double) pow((1 - p_i[0]), xVals[t][0]);
		for (int i = 1; i < 23; i++) {
			xTemp_2 *= (double)pow((1 - p_i[i]), xVals[t][i]);
		}
		xTemp = (double)1 - xTemp_2;
		if (yTemp == 0) {
			if (xTemp >= 0.5) {
				mistakes++;
			}
			xTemp = (double)(1 - xTemp);
		}
		else if (xTemp <= 0.5) {
			mistakes++;
		}
		logVal += log(xTemp);
	}

	retVal = (double)((double)1 / 267) * logVal;
	return retVal;
}

class Movie {
public:
	Movie();
	Movie(string);

	string title;
	double popularity;
private:

};

Movie::Movie() {

}

Movie::Movie(string name) {
	title = name;
}

struct less_than_prob
{
	inline bool operator() (const Movie& lhs, const Movie& rhs)
	{
		return (lhs.popularity < rhs.popularity);
	}
} myObj;

struct less_than_prob2
{
	inline bool operator() (const Movie& lhs, const Movie& rhs)
	{
		return (lhs.popularity > rhs.popularity);
	}
} myObj2;

class Master2{
public:

	Master2();
	
	char movieRatings[258][50];
	void getRatings();

	Movie movies[50];
	void getMovies();

	void sanityCheck();

	double Z_init[4];
	void getZ();

	long double R_given_Z[50][4];
	void getR_Z();

	double likelihood();
	double eStep(int, int);
	void mStep();

	void findNewMovies(int);

private:

};

Master2::Master2() {
	getRatings();
	getMovies();

	getZ();
	getR_Z();
}

void Master2::getRatings() {
	char x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20;
	char x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33, x34, x35, x36, x37, x38, x39, x40;
	char x41, x42, x43, x44, x45, x46, x47, x48, x49, x50;

	int index = 0;

	ifstream infile("hw5_movieRatings.txt");

	while (infile >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10
		>> x11 >> x12 >> x13 >> x14 >> x15 >> x16 >> x17 >> x18 >> x19 >> x20
		>> x21 >> x22 >> x23 >> x24 >> x25 >> x26 >> x27 >> x28 >> x29 >> x30
		>> x31 >> x32 >> x33 >> x34 >> x35 >> x36 >> x37 >> x38 >> x39 >> x40
		>> x41 >> x42 >> x43 >> x44 >> x45 >> x46 >> x47 >> x48 >> x49 >> x50) {

		// lmaoooooooooooooooo
		movieRatings[index][0] = x1;
		movieRatings[index][1] = x2;
		movieRatings[index][2] = x3;
		movieRatings[index][3] = x4;
		movieRatings[index][4] = x5;
		movieRatings[index][5] = x6;
		movieRatings[index][6] = x7;
		movieRatings[index][7] = x8;
		movieRatings[index][8] = x9;
		movieRatings[index][9] = x10;

		movieRatings[index][10] = x11;
		movieRatings[index][11] = x12;
		movieRatings[index][12] = x13;
		movieRatings[index][13] = x14;
		movieRatings[index][14] = x15;
		movieRatings[index][15] = x16;
		movieRatings[index][16] = x17;
		movieRatings[index][17] = x18;
		movieRatings[index][18] = x19;
		movieRatings[index][19] = x20;
	
		movieRatings[index][20] = x21;
		movieRatings[index][21] = x22;
		movieRatings[index][22] = x23;
		movieRatings[index][23] = x24;
		movieRatings[index][24] = x25;
		movieRatings[index][25] = x26;
		movieRatings[index][26] = x27;
		movieRatings[index][27] = x28;
		movieRatings[index][28] = x29;
		movieRatings[index][29] = x30;

		movieRatings[index][30] = x31;
		movieRatings[index][31] = x32;
		movieRatings[index][32] = x33;
		movieRatings[index][33] = x34;
		movieRatings[index][34] = x35;
		movieRatings[index][35] = x36;
		movieRatings[index][36] = x37;
		movieRatings[index][37] = x38;
		movieRatings[index][38] = x39;
		movieRatings[index][39] = x40;

		movieRatings[index][40] = x41;
		movieRatings[index][41] = x42;
		movieRatings[index][42] = x43;
		movieRatings[index][43] = x44;
		movieRatings[index][44] = x45;
		movieRatings[index][45] = x46;
		movieRatings[index][46] = x47;
		movieRatings[index][47] = x48;
		movieRatings[index][48] = x49;
		movieRatings[index][49] = x50;

		index++;
	}
}

void Master2::getMovies() {
	string movieTitle;
	int index = 0;

	ifstream infile("hw5_movieTitles.txt");

	while (infile >> movieTitle) {
		movies[index] = Movie(movieTitle);
		index++;
	}
}

void Master2::getZ() {
	double z;
	int index = 0;

	ifstream infile("hw5_probZ_init.txt");

	while (infile >> z) {
		Z_init[index] = z;
		index++;
	}
}

void Master2::getR_Z() {
	long double r1, r2, r3, r4;
	int index = 0;

	ifstream infile("hw5_probRgivenZ_init.txt");

	while (infile >> r1 >> r2 >> r3 >> r4) {
		R_given_Z[index][0] = r1;
		R_given_Z[index][1] = r2;
		R_given_Z[index][2] = r3;
		R_given_Z[index][3] = r4;
		index++;
	}
}

void Master2::sanityCheck() {
	vector<Movie> sortList;
	for (int i = 0; i < 50; i++) {
		
		int numRec = 0;
		int numSeen = 0;
		
		Movie currMov = movies[i];
		
		for (int j = 0; j < 258; j++) {
			char temp = movieRatings[j][i];
			if (temp != '?') {
				if (temp == '1') {
					numRec++;
				}
				numSeen++;
			}
		}
		currMov.popularity = (double)numRec / numSeen;
		sortList.push_back(currMov);
	}

	sort(sortList.begin(), sortList.end(), myObj);

	for (vector<Movie>::iterator it = sortList.begin(); it != sortList.end(); it++) {
		cout << " " << it->title << endl;
	}
}

double Master2::likelihood() {
	double zSum;
	double pProd;
	double logVal = 0;
	double retVal;

	for (int t = 0; t < 258; t++) {
		zSum = 0;
		for (int i = 0; i < 4; i++) {
			double zTemp = Z_init[i];
			pProd = 1;
			for (int j = 0; j < 50; j++) {
				char rateTemp = movieRatings[t][j];
				if (rateTemp == '1') {
					pProd *= R_given_Z[j][i];
				}
				else if (rateTemp == '0') {
					pProd *= (double) 1 - R_given_Z[j][i];
				}
			}
			zSum += (double)zTemp * pProd;
		}
		logVal += log(zSum);
	}

	retVal = (double) ((double)1 / 258) * logVal;
	return retVal;
}

double Master2::eStep(int zVal, int tVal) {
	double numerator;
	double nume_2;
	double denom = 0;
	double nProd;
	double dProd;
	double retVal;

	// Numerator
	numerator = Z_init[zVal];
	nProd = 1;
	for (int j = 0; j < 50; j++) {
		char rateTemp = movieRatings[tVal][j];
		if (rateTemp == '1') {
			nProd *= R_given_Z[j][zVal];
		}
		else if (rateTemp == '0') {
			nProd *= (double)1 - R_given_Z[j][zVal];
		}
	}
	numerator *= nProd;

	// Denominator
	for (int i = 0; i < 4; i++) {
		double zTemp = Z_init[i];
		dProd = 1;
		for (int j2 = 0; j2 < 50; j2++) {
			char rateTemp = movieRatings[tVal][j2];
			if (rateTemp == '1') {
				dProd *= R_given_Z[j2][i];
			}
			else if (rateTemp == '0') {
				dProd *= (double)1 - R_given_Z[j2][i];
			}
		}
		denom += (double) zTemp * dProd;
	}
	retVal = (double)numerator / denom;
	return retVal;
}

void Master2::mStep() {

	double temp = (double)1 / 258;
	double Z_temp[4];
	double RZ_temp[50][4];
	double P_temp[4][258];

	// P(Z = i)
	for (int i = 0; i < 4; i++) {
		double zSum = 0;
		for (int t = 0; t < 258; t++) {
			double p_it = eStep(i, t);
			zSum += p_it;

			P_temp[i][t] = p_it;
		}
		Z_temp[i] = temp * zSum;
	}

	for (int i2 = 0; i2 < 4; i2++) {
		Z_init[i2] = Z_temp[i2];
	}

	// P(R_j = 1 | Z = i)
	for (int i3 = 0; i3 < 4; i3++) {
		for (int j = 0; j < 50; j++) {
			double sum1 = 0;
			double sum2 = 0;
			double denom = 0;
			double numerator = 0;
			for (int t2 = 0; t2 < 258; t2++) {
				char rateTemp = movieRatings[t2][j];
				if (rateTemp == '1') {
					sum1 += P_temp[i3][t2];
				}
				else if (rateTemp == '0') {
					sum1 += (double)0;
				}
				else {
					sum2 += (double)(P_temp[i3][t2] * R_given_Z[j][i3]);
				}
			}
			numerator = sum1 + sum2;
			for (int t3 = 0; t3 < 258; t3++) {
				denom += P_temp[i3][t3];
			}
			RZ_temp[j][i3] = (double)numerator / denom;
		}
	}

	for (int x = 0; x < 50; x++) {
		for (int y = 0; y < 4; y++) {
			R_given_Z[x][y] = RZ_temp[x][y];
		}
	}
}

void Master2::findNewMovies(int t) {
	double zSum;
	double P_me[4];

	double P_movies[50];

	for (int x = 0; x < 50; x++) {
		P_movies[x] = 0;
	}

	for (int i = 0; i < 4; i++) {
		P_me[i] = eStep(i, (t - 1));
	}

	for (int j = 0; j < 50; j++) {
		zSum = 0;
		for (int z = 0; z < 4; z++) {
			char rateTemp = movieRatings[t - 1][j];
			if (rateTemp == '?') {
				zSum += (double)P_me[z] * R_given_Z[j][z];
			}
		}
		P_movies[j] = zSum;
	}

	vector<Movie> sortList;

	for (int f = 0; f < 50; f++) {
		Movie currMov = movies[f];
		currMov.popularity = P_movies[f];
		sortList.push_back(currMov);
	}

	sort(sortList.begin(), sortList.end(), myObj2);

	for (vector<Movie>::iterator it = sortList.begin(); it != sortList.end(); it++) {
		cout << " " << it->title << endl;
	}
}

int main() {

	// Problem 5.2

	Master masterBlaster;
	double L;

	cout << " PROBLEM 5.2: NOISY-OR" << endl << endl;

	L = masterBlaster.noisyOr();
	cout << " Iteration(0): " << "M = " << masterBlaster.mistakes << " | " << "L = " << L << endl;

	for (int i = 0; i < 512; i++) {
		masterBlaster.mistakes = 0;
		masterBlaster.updatePI();
		L = masterBlaster.noisyOr();
		cout << " Iteration(" << i + 1 << "): " << "M = " << masterBlaster.mistakes << " | " << "L = " << L << endl;
	}

	cout << endl << endl << " FINAL P_i ESTIMATIONS" << endl;
	for (int j = 0; j < 23; j++) {
		cout << " P_i[" << j + 1 << "] = " << masterBlaster.p_i[j] << endl;
	}

	cout << endl << endl;

	//////////////////////////////////////////////////////////////////////////////////
	
	// Problem 5.3

	cout << " PROBLEM 5.3: MOVIES" << endl << endl;

	Master2 masterBlaster2;

	cout << " PART A: SANITY CHECK" << endl;
	masterBlaster2.sanityCheck();

	cout << endl << endl << " PART B/C/D/E: LOG LIKELIHOOD" << endl;
	double L2 = masterBlaster2.likelihood();
	cout << " Iteration(0): L = " << L2 << endl;

	for (int i2 = 0; i2 < 128; i2++) {
		masterBlaster2.mStep();
		L2 = masterBlaster2.likelihood();
		if (i2 == 0) {
			for (int j2 = 0; j2 < 4; j2++) {
				cout << masterBlaster2.R_given_Z[j2][0] << endl;
			}
		}
		cout << " Iteration(" << i2 + 1 << "): " << "L = " << L2 << endl;
	}

	cout << endl << endl << " PART F: PERSONAL MOVIE RECOMMENDATIONS" << endl;
	masterBlaster2.findNewMovies(180);
	
	cin.get();
	return 0;
}