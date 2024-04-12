#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

long long getFastPowerOfTwo(int power) { //we use bit shifting, as seen in an admission test model for our faculty
	/**
	* IT calculates fast a power of 2
	*@param power - the power to calculate
	* @return 2 ^ power
	*/
	return 1 << power; //pretty simple to understand, if we shift 1 by 2 we get 100, which is 4
}

long long getNrOfBases(int n) { //if we resolve this problem on a paper, we can clearly come up with a relation for n, that being a product
	/**
	* It calculates the number of bases of Z2n over Z2 (we use the product calculated on paper)
	* @param n - integer
	* @return number of bases of Z2n over Z2
	*/
	long long p2n = getFastPowerOfTwo(n) - 1; //we get 2 to the power n(dimension) and we subtract 1 for the 0 vector
	long long p2k = 1; //we need to keep a power of 2
	long long result = 1; //the result is calculated in this variable
	for (int i = 0; i <= n - 1; i++) { //we calculate the product of i from 0 to n - 1 of (2^n - 2^i + 1)
		result *= (p2n - p2k + 1);
		p2k *= 2;
	}
	return result;
}

void printNumberOfBases(int n, long long nrOfBases) {
	/**
	* IT prints the number of bases according to the sentence
	* @param n - integer
	* @param nrOfBases - long integer, the number of bases found
	* @return output needed
	*/
	fout << "1. The number of bases of the vector space Z2" << n << " over Z2 is " << nrOfBases << "\n";
}

void printBasesSentence() {
	/**
	* IT prints the second sentence of the problem
	* @return output needed
	*/
	fout << "2. The vectors of each such basis are: \n";
}

void printVector(vector<bool> vector) {
	/**
	* IT prints a vector from the basis
	* @param vector - a vector from stl of bools, vector to print
	* @return output needed
	*/
	fout << "(";
	fout << vector[1];
	for (int i = 2; i < vector.size(); i++)
		fout << ", " << vector[i];
	fout << ")";
}

void printBasis(vector<vector<bool>> basis) {
	/**
	* IT prints a basis
	* @param basis - a vector of vectors of bools, the basis to print
	* @return output needed
	*/
	fout << "(";
	printVector(basis[1]);
	for (int i = 2; i < basis.size(); i++) {
		fout << ",";
		printVector(basis[i]);
	}
	fout << ")\n";
}

vector<vector<bool>> getNewMatrix(vector<vector<bool>> m, int n, int k) {
	/**
	* IT prints a basis
	* @param m - a vector of vectors of bools, the basis to print
	* @param n - dimension of the origin matrix
	* @param k - the line to avoid
	* @return output needed
	*/
	vector<vector<bool>> newM = vector<vector<bool>> (n);
	int i2 = 1;
	int j2 = 1;
	for (int i = 1; i <= n; i++) {
		if (i != k) {
			newM[i2] = vector<bool>(n);
			j2 = 1;
			for (int j = 2; j <= n; j++)
				newM[i2][j2] = m[i][j], j2++;
			i2++;
		}
	}
	return newM;
}

int det(vector<vector<bool>> m) {
	/**
	* IT calculates the determinant of the matrix
	* @param m - a vector of vectors of bools, the matrix
	* @return determinant, integer
	*/
	int n = m.size() - 1;

	if (n == 2)
		return m[1][1] * m[2][2] - m[1][2] * m[2][1];

	int sign;
	int sum = 0;

	for (int i = 1; i <= n; i++) {
		sign = (i % 2 == 1) ? 1 : -1;
		sum += m[i][1] * sign * det(getNewMatrix(m, n, i));
	}
	return sum;
}

vector<vector<bool>> normalize(vector<vector<bool>> m) {
	/**
	* This function was originally done to swap the elements of the matrix by the secondary diagonal, this is not needed, but I included this as to understand my way of thinking
	* @param m - a vector of vector of bools, the matrix
	* @return new matrix
	*/
	int n = m.size() - 1;

	vector<vector<bool>> normalizedM = m;

	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++)
			swap(normalizedM[i][j], normalizedM[j][i]);
	}
	return normalizedM;
}

void genBases(vector<vector<bool>>& basis, int n, int i = 1, int j = 1) {
	/**
	* This function is used to generate the bases
	* @param basis - a vector of vector of bools, the basis
	* @param n - number of elements needed in the basis
	* @param i - index 
	* @param j - another index
	*/
	if (i > n) { // you are going to ask, why is normalize not needed, well, we only search for determinants which are 0, there we do not have any problems if the matrix is swapped around by the second diagonal
		if (det(basis) % 2 != 0) //we know that a set is a basis, if their determinant is different than 0, as seen in the last seminar, problem 10
			printBasis(basis); //fun fact, one the line above I had a bug that took me two hours yesterday to find, as I was a bit tired. I forgot to add the % 2, as we are in Z2
	}
	else {
		if (j > n) {
			genBases(basis, n, i + 1, 1); // if we finished to complete the first vector of the basis, we go to the next
		}
		else {
			if (j == 1) // we initialize basis[i]
				basis[i] = vector<bool>(n + 1);
			basis[i][j] = 0; // we have 2 possibilities in Z2, either 0 or 1
			genBases(basis, n, i, j + 1);
			basis[i][j] = 1;
			genBases(basis, n, i, j + 1);
		}
	}
}

int main() {
	int n;
	fin >> n;
	printNumberOfBases(n, getNrOfBases(n)); //we print the number of bases
	if (n <= 4) { // for n <= 4 we generate the bases
		printBasesSentence();
		if (n == 1) { // if n == 1, then we only have (1), but the determinant does not calculate anything for a matrix of 1x1
			fout << "(1)";
		}
		else {
			vector<vector<bool>> basis = vector<vector<bool>>(n + 1); //we initialize basis
			genBases(basis, n);
		}
	}
	fin.close(), fout.close(); //we close the files to avoid memory leaks
	return 0;
}
