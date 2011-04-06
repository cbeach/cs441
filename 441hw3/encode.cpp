//Copyright Casey Beach 2011


#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

bool** getInstance(char* fileName);
void encodeExistance(ofstream &myOut);
void encodeUnique(ofstream &myOut);
void encodeConnection(ofstream &myOut, bool** instance);
void encodeWires(ofstream &myOut, bool** instance);
void encodeCrossing(ofstream &myOut);
void transfer(ifstream &tempIn, ofstream &tempOut);

int leftComp, rightComp;
int termCount = 0;

int main(int argc, char**argv)
{
	leftComp = rightComp = 0;
	
	ofstream myOut("temp");

	bool** truthTable = getInstance("inst-5.txt");
	

	encodeExistance(myOut);
	encodeWires(myOut, truthTable);
	encodeConnection(myOut, truthTable);
	encodeUnique(myOut);
	encodeCrossing(myOut);

	myOut.close();
	
	
	ifstream tempIn("temp");
	assert(tempIn);

	ofstream tempOut("output");
	assert(tempOut);

	tempOut << "cnf " << 6*rightComp * rightComp << " " << termCount << endl;

	transfer(tempIn, tempOut);
	
	tempIn.close();
	tempOut.close();

}

void encodeConnection(ofstream &myOut, bool** instance)
{
	myOut << "c connected" << endl;
	for(int i = 0; i < rightComp; i++)
	{
		for(int j = 0; j < leftComp; j++)
		{
			if(instance[i][j] == false)
				myOut << "-";
			myOut << 33 << i + 1 << j + 1 + rightComp << " ";
			myOut << " 0" << endl;
		}
		termCount++;
	}


}


void encodeWires(ofstream &myOut, bool** instance)
{
	myOut << "c Wired" << endl;
	string* retVal = 0;
	retVal = new string;
	
	for(int i = 0; i < rightComp; i++) //god this is horrible, must recode.
	{
		for(int j = 0; j < leftComp; j++)
		{
			for(int k = rightComp; k < 2 * rightComp; k++)
			{
				for(int l = rightComp; l < 2 * rightComp; l++)
				{
					myOut << "-" << i+1 << k+1 << " ";
					myOut << "-" << j+1 << l+1 << " ";
					myOut << "-" << 33 << i+1 << k+1 << " ";
					myOut << 44 << k+1 << l+1 << " 0" << endl; 
					termCount++;

					myOut << "-" << i+1 << k+1 << " ";
					myOut << "-" << j+1 << l+1 << " ";
					myOut << 33 << i+1 << k+1 << " ";
					myOut << "-" << 44 << k+1 << l+1 << " 0" << endl; 
					termCount++;
				}
			}
		}
	}
}
/*
string* getAllRight()
{
	string **retString = new string*[rightComp * rightComp];
	string iString;
	for(int i = 0; i < rightComp; i++)
	{
		iString.clear();
		iString.append(i);
		retString[
		for(int j = 0; j < rightComp; j++)
		{
			retString[i+j].append(iString);
			retString[i+j].append(j);

		}
	}


}
*/
void encodeUnique(ofstream &myOut)
{
	myOut << " c Unique" << endl;
	for(int i = 0; i < rightComp; i++)
	{
		for(int j = 0; j < rightComp; j++)
		{
			for(int k = 0; k < rightComp; k++)
			{
				if(i!=j)
				{
					myOut << "-" << i + 1 << k + 1 << " " 
					<< "-" << j + 1 << k + 1 << " 0" << endl;
					termCount++;

					myOut << "-" << i + rightComp + 1 << k+rightComp + 1 << " " 
					<< "-" << j + rightComp + 1 << k + rightComp + 1 << " 0" << endl;
					termCount++;
				}

			}
		}
	}

}

void encodeCrossing(ofstream &myOut)
{
	myOut << "c Crossing" << endl;
	for(int i = 0; i < rightComp; i++)
	{
		for(int j = 0; j < rightComp; j++)
		{
			for(int k = 0; k < rightComp; k++)
			{
				for(int m = 0; m < rightComp; m++)
				{
					if((k < i) && (m > j))
					{
						myOut << "-" << 44 << i + 1 << j + 1 << " " 
						<< "-" << 44 << k + 1 << m + 1 << " 0" << endl;
						termCount++;
					}
						
					if((k > i) && (m < j))
					{
						myOut << "-" << 44 << i + 1 << j + 1 << " " 
						<< "-" << 44 << k + 1 << m + 1 << " 0" << endl;
						termCount++;
					}
				}
			}
		}
	}
}

void encodeExistance(ofstream &myOut)
{
	string* retVal = 0;
	retVal = new string;
	
	char* iString = new char[5];
	myOut << "c exists" << endl;

	for(int i = 0; i < rightComp; i++)
	{
		for(int j = 0; j < leftComp; j++)
		{
			myOut << i+1 << j+1 << " ";
		}
		myOut << " 0" << endl;
		termCount++;
	}
	
	for(int i = rightComp; i < rightComp*2; i++)
	{
		for(int j = leftComp; j < leftComp*2; j++)
		{
			myOut << i+1 << j+1 << " ";
		}
		myOut << " 0" << endl;
		termCount++;
	}

}

bool** getInstance(char* fileName)
{
	bool** retVal = 0;	
	bool** tempBool = 0;
	retVal = new bool*[128];
	bool first = true;

	string line;
	
	int l = 0;
	int r = 0;

	ifstream instFile(fileName);
	assert(instFile);

	while(instFile.good())
	{
		getline(instFile,line);
		if(line.empty())
			break;
		cout << line << endl;
		if(first)
		{
			r = line.length();
			first = false;
		}
 		retVal[l] = new bool[r];
		
		for(int i = 0; i < r; i++)
		{
			switch(line.at(i))
			{
				case 't':
					retVal[l][i] = true;
					break;
				case 'f': 
					retVal[l][i] = false;
					break;
				default:
					cout << "Hey, You shouldn't be here!" << endl;
			}
		}
		l++;
	}
	
	leftComp = l;
	rightComp = r;
	
	instFile.close();
	tempBool = new bool*[l];

	for(int i = 0; i < r; i++)
	{
		tempBool[i] = new bool[r];
		for(int j = 0; j < l; j++)
		{
			tempBool[i][j] = retVal[i][j];
		}
	}
	
	return tempBool;
}

void transfer(ifstream &tempIn, ofstream &tempOut)
{

	string line;
	while(tempIn.good())
	{
		getline(tempIn,line);
		if(line.empty())
			break;
		 tempOut << line << endl;
	}






}




