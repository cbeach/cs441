//Copyright Casey Beach 2011


#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

double* fillF0(int**);
double* fillF1(int**);

int** getTrainingInstance(char* fileName);
int** getTestInstance(char* fileName);

double** getL(double* F0, double* F1, int** testingInst);
void output(double** L, int **testingInst, const char* header);

int trainingFeatures;
int trainingRows;
int trainingOne;
int trainingZero;

int testFeatures;
int testRows;
int testOne;
int testZero;


int main(int argc, char**argv) {

	int **trainingInst, **testingInst;
	double *F0, *F1;
	double **L;
	double possTest = 0, totTest = 0; //possTest = number of class 1 in test case, totTest = number of cases in test case
	double correctPossGuess = 0;
	double correctNegGuess = 0;

	trainingInst = getTrainingInstance("./spect-orig.train.csv");

	F0 = fillF0(trainingInst);
	F1 = fillF1(trainingInst);

	testingInst = getTestInstance("./spect-orig.test.csv");


	L = getL(F0, F1, testingInst);
	output(L, testingInst, "orig: ");

	trainingInst = getTrainingInstance("./spect-itg.train.csv");

	F0 = fillF0(trainingInst);
	F1 = fillF1(trainingInst);

	testingInst = getTestInstance("./spect-itg.test.csv");

	L = getL(F0, F1, testingInst);
	output(L, testingInst, "itg: ");

	trainingInst = getTrainingInstance("./spect-resplit.train.csv");

	F0 = fillF0(trainingInst);
	F1 = fillF1(trainingInst);

	testingInst = getTestInstance("./spect-resplit.test.csv");


	L = getL(F0, F1, testingInst);
	output(L, testingInst, "resize: ");



}

void output(double **L, int **testingInst, const char* header) {

	int correctPossGuess = 0;
	int correctNegGuess = 0;

	for(int i = 0; i < testRows; i++) {
		if(L[1][i] > L[0][i]) {
			if(testingInst[i][0] == 1)
				correctPossGuess++;
		}
		else{
			if(testingInst[i][0] == 0)
				correctNegGuess++;
		}
	}

	cout << header << correctPossGuess + correctNegGuess << "/" << L[2][1] << "(" << ((correctPossGuess + correctNegGuess) / L[2][1]) << ") " 
	<< correctNegGuess << "/" << L[2][1] - L[2][0] << "(" << correctNegGuess / (L[2][1] - L[2][0]) << ") "  
	<< correctPossGuess << "/" << L[2][0] << "(" << correctPossGuess / L[2][0] << ")"<< endl;


}

double** getL(double *F0, double *F1, int ** testingInst) {

	double **L;
	L = new double*[3];
	L[0] = new double[testRows];
	L[1] = new double[testRows];
	L[2] = new double[2];	//L[2][0] number of class 1 in test set, L[2][1]= total number of cases in test set.

	for(int i = 0; i < testRows; i++){
		L[0][i] = log10(trainingZero + .5) - log10(trainingRows + .5);
		L[1][i] = log10(trainingOne + .5) - log10(trainingRows + .5);
		for(int j = 1; j < testFeatures; j++){
			if(testingInst[i][j] == 1) {
				L[0][i] += (log10( (F0[j]) + 0.5) - log10( trainingZero + 0.5));
				L[1][i] += (log10( (F1[j]) + 0.5) - log10( trainingOne + 0.5));
			}
			else if(testingInst[i][j] == 0) {
				L[0][i] += (log10( (trainingZero - F0[j]) + 0.5) - log10( trainingZero + 0.5));
				L[1][i] += (log10( (trainingOne - F1[j]) + 0.5) - log10( trainingOne + 0.5));
			}
		}

		if(testingInst[i][0] == 1)
			L[2][0]++;
		L[2][1]++;
	}

	return L;
}


double* fillF0(int** training) {
	
	double *F0 = new double[trainingFeatures - 1];

	for(int i = 0; i < trainingFeatures - 1; i++)
		F0[i] = 0;

	for(int i = 0; i < trainingRows; i++) {
		if(training[i][0] == 0) {
			trainingZero++;
			for(int j = 0; j < trainingFeatures - 1; j++) {
				F0[j] += training[i][j+1];
			}
		}
	}
	return F0;

}

double* fillF1(int **training ) {
	
	double *F1 = new double[trainingFeatures - 1];

	for(int i = 0; i < trainingFeatures - 1; i++)
		F1[i] = 0;


	for(int i = 0; i < trainingRows; i++) {
		if(training[i][0] == 1) {
			trainingOne++;
			for(int j = 0; j < trainingFeatures - 1; j++) {
				F1[j] += training[i][j+1];
			}
		}
	}
	return F1;

}

int** getTrainingInstance(char* fileName) {

	int** retVal = 0;	
	int* tempBool = 0;
	retVal = new int*[512];

	trainingFeatures = 0;
	trainingRows = 0;
	trainingOne = 0;
	trainingZero = 0;

	tempBool = new int[512];

	string line;
	
	int counter = 0;

	ifstream instFile(fileName);
	assert(instFile);

	while(instFile.good()) {

		getline(instFile,line);
		if(line.empty())
			break;
		for(unsigned int i = 0; i < line.length(); i++) {
			switch(line.at(i)) {
				case '1':
					tempBool[counter] = 1;
					counter++;
					break;
				case '0': 
					tempBool[counter] = 0;
					counter++;
					break;
				case ',':
					break;
				default:
					cout << "Hey, You shouldn't be here!" << endl;
			}
		}
		retVal[trainingRows] = new int[counter];
		for(int i = 0; i <= counter - 1; i++){
			retVal[trainingRows][i] = tempBool[i];

		}
		if(trainingRows != 0)
			assert(trainingFeatures == counter);

		trainingFeatures = counter;
		trainingRows++;
		counter = 0;

		
	}
	instFile.close();
	
	return retVal;
}

int** getTestInstance(char* fileName) {

	int** retVal = 0;	
	int* tempBool = 0;
	retVal = new int*[512];

	testFeatures = 0;
	testRows = 0;
	testOne = 0;
	testZero = 0;

	tempBool = new int[512];

	string line;
	
	int counter = 0;

	ifstream instFile(fileName);
	assert(instFile);

	while(instFile.good()) {


		getline(instFile,line);
		if(line.empty())
			break;
		for(unsigned int i = 0; i < line.length(); i++) {
			switch(line.at(i)) {
				case '1':
					tempBool[counter] = 1;
					counter++;
					break;
				case '0': 
					tempBool[counter] = 0;
					counter++;
					break;
				case ',':
					break;
				default:
					cout << "Hey, You shouldn't be here!" << endl;
			}
		}
		retVal[testRows] = new int[counter];
		for(int i = 0; i <= counter - 1; i++){
			retVal[testRows][i] = tempBool[i];

		}
		if(testRows != 0)
			assert(testFeatures == counter);

		testFeatures = counter;
		testRows++;
		counter = 0;

		
	}
	instFile.close();
	
	return retVal;
}

