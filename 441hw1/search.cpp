//Copyright 2011 Casey Beach


#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

#define TIME_LIMIT 60

int NumberOfSeats;



struct pairAndScore{

	int pair[2];
	int score;
};

bool pasCompare(const pairAndScore* lhs, const pairAndScore* rhs)
{
	return lhs->score > rhs->score;
}

int** readFile(string);
int*  shuffle();
int*  searchLoop(int**);
int scoreTable(int* table, int** matrix);
int scoreChangedPair(int* table, int** matrix, int* pair);
int partialScore(int* table, int** matrix, int person);
int pairScore(int, int, int**);
char** parseLine(string);  //because strtok is a steaming pile!
vector<pairAndScore*>* getAllSwaps();
void swap(int*, int*);

int main(int argc, char** argv)
{

	string fileName("");
	string fileNumber = "1";				
	
	if (argc == 2)
		fileNumber = argv[1];
	else if (argc > 2)
	{
		perror("Usage: search [instance number]\n");
		exit(1);
	}

	fileName = "hw1-inst"+fileNumber+".txt";
	int** matrix = readFile(fileName);
	searchLoop(matrix);
}

int* shuffle()
{
	int deck[NumberOfSeats];
	int* shuffledDeck = new int[NumberOfSeats];

	for(int i = 0; i < NumberOfSeats; i++)
	{
		deck[i] = rand();
	}

	int currentHigh = -1;
	
	for(int i = 0; i < NumberOfSeats; i++)
	{
		for(int j = 0; j < NumberOfSeats; j++)
		{
			if(deck[j] > deck[currentHigh])
				currentHigh = j;
		}

		shuffledDeck[i] = currentHigh;
		deck[currentHigh] = -1;
	}

	return shuffledDeck;
}


int* searchLoop(int** matrix)
{
	int* tables[128];
	int elapsedTime = 0;
	int tableScores[128];
	int pair[2] = {-1, -1};
	int firstScore = 0;
	int secondScore = 0;

	int choiceType = 0;
	
	time_t start, end;

	time(&start);

	vector<pairAndScore*> **switchedPairs= new vector<pairAndScore*>*[128];

	for(int i = 0; i < 128; i++)
	{
		switchedPairs[i] = getAllSwaps();
	}

	for(int i = 0; i < 128; i++)
	{
		tables[i] = shuffle();
		tableScores[i] = scoreTable(tables[i], matrix);
	}

	cout << "Entering Search Loop. Count-down has begun..." << endl;
	
	getAllSwaps();

	while(elapsedTime < TIME_LIMIT)
	{
		int choiceType = rand() % 10;
		for(int i = 0; i < 128; i++)
		{
			for(int j = 0; j < NumberOfSeats * NumberOfSeats; j++)//get the scores for all of the tables
			{
				swap(switchedPairs[i]->at(j)->pair, tables[i]);
				switchedPairs[i]->at(j)->score = scoreTable(tables[i],matrix);
				swap(switchedPairs[i]->at(j)->pair, tables[i]);
			}
			if(choiceType > 5)	//choose one of the 5 best tables
			{
				sort(switchedPairs[i]->begin(),switchedPairs[i]->end(),pasCompare);
				swap(switchedPairs[i]->at(rand() % 5)->pair, tables[i]);				
				
			} else if (choiceType <= 5 && choiceType > 2)	//randomly take any improvement
			{
				sort(switchedPairs[i]->begin(), switchedPairs[i]->end(), pasCompare);
				for(int k = 0; k < NumberOfSeats * NumberOfSeats; k++)
				{
					if(k > 0 && switchedPairs[i]->at(k)->score <= 0)
						swap(switchedPairs[i]->at(rand() % k)->pair, tables[i]);		
				}
			
			} else if (choiceType <= 2)	//randomly take ANY pair swap.
			{
				swap(switchedPairs[i]->at(rand() % (NumberOfSeats * NumberOfSeats))->pair, tables[i]);
			}
		}

		time(&end);
		elapsedTime = difftime(end, start);
	}

	int bestTable = 0;

	for(int i = 1; i < 128; i ++)
	{
		if(scoreTable(tables[i], matrix) > scoreTable(tables[bestTable], matrix))
			bestTable = i;
	}
	
	cout << TIME_LIMIT << " seconds have passed" << endl;
	cout << "The best table has a score of " << scoreTable(tables[bestTable], matrix) << endl;


	return 0;
}

void swap(int *pair, int* table)
{
	int tempI = 0;
	tempI = table[pair[0]]; 
	table[pair[0]] = table[pair[1]];
	table[pair[1]] = tempI;
}

vector<pairAndScore*>* getAllSwaps()
{
	vector<pairAndScore*> *switchedPairs;
	switchedPairs = new vector<pairAndScore*> (NumberOfSeats * NumberOfSeats);
	
	for(int i = 0; i < NumberOfSeats; i++)	//try every possible pair swap
	{
		for(int j = 0; j < NumberOfSeats; j++)
		{
			switchedPairs->at(NumberOfSeats * i + j) = new pairAndScore();
			switchedPairs->at(NumberOfSeats * i + j)->pair[0] = i;
			switchedPairs->at(NumberOfSeats * i + j)->pair[1] = j;
		}
	}
	return switchedPairs;
}

int scoreTable(int* table, int** matrix)
{
	int score = 0;
	int halfOfSeats = NumberOfSeats / 2;

	for(int i = 0; i < halfOfSeats; i++)
	{
		if(i < halfOfSeats - 1)
		{
			score += pairScore(table[i], table[i + 1], matrix); //person i and 1 to the left
			score += pairScore(table[i + 1], table[i], matrix);//vice versa
			
			if(i + halfOfSeats + 1 < NumberOfSeats)
			{
				score += pairScore(table[i + halfOfSeats], table[i + halfOfSeats + 1], matrix);//now on the other side
				score += pairScore(table[i + halfOfSeats + 1], table[i + halfOfSeats], matrix);
			}

			if((table[i] < halfOfSeats && table[i + 1] >= halfOfSeats) ||
			   (table[i] >= halfOfSeats && table[i + 1] < halfOfSeats))
				score += 1;

		}
		if((table[i] < halfOfSeats && table[i + halfOfSeats] >= halfOfSeats) ||
		   (table[i] >= halfOfSeats && table[i + halfOfSeats] < halfOfSeats))
		{
			score += 2;
		}
		score += pairScore(table[i + halfOfSeats], table[i], matrix);
		score += pairScore(table[i], table[i + halfOfSeats], matrix);

	}	
	
	return score;
}

int scoreChangedPair(int* table, int** matrix, int* pair)
{


	return 0;
}
/*

int partialScore(int* table, int** matrix, int person)
{
	int halfOfSeats = NumberOfSeats / 2;
	
	int personScore = 0;

	//if pair[0] isn't in either seat on the far left hand end
	//then tally the points for the person on their left.
	if(person != 0 && person != halfOfSeats)	
	{
		//regular Likeability
		personScore += pairScore(table[person], table[person - 1], matrix);
		personScore += pairScore(table[person - 1], table[person], matrix);

		//male female pair?
		if(table[person] < halfOfSeats && table[person - 1] > halfOfSeats ||
		   table[person] > halfOfSeats && table[person - 1] < halfOfSeats)
			personScore += 1;
		
	}

	//and if they aren't in either seat on the far right,
	//then tally the points for the person on their right.
	if(person != halfOfSeats - 1 && person != NumberOfSeats)
	{
		//regular likeability
		personScore += pairScore(table[person], table[person + 1], matrix);
		personScore += pairScore(table[person + 1], table[person], matrix);
		
		//male female points.
		if(table[person] < halfOfSeats && table[person + 1] > halfOfSeats ||
		   table[person] > halfOfSeats && table[person + 1] < halfOfSeats)
			personScore += 1;
	}	

	
	
	if(person >= halfOfSeats)
	{
		//regular likability
		personScore += pairScore(table[person], table[person - halfOfSeats], matrix);
		personScore += pairScore(table[person - halfOfSeats], table[person], matrix);
		
		//male female pair
		if(table[person] < halfOfSeats && table[person - halfOfSeats] > halfOfSeats ||
		   table[person] > halfOfSeats && table[person - halfOfSeats] < halfOfSeats)
			personScore += 2;
	}	

	if(person <= halfOfSeats)
	{
		//regular likeability
		personScore += pairScore(person, table[person + halfOfSeats], matrix);
		personScore += pairScore(table[person + halfOfSeats], person, matrix);

		//male female pair
		if(table[person] > halfOfSeats  && table[person - halfOfSeats] > halfOfSeats ||
		   table[person] < halfOfSeats  && table[person - halfOfSeats] < halfOfSeats)
			personScore += 2;
	}	

	return personScore;	
}
*/
int pairScore(int person1, int person2, int ** matrix)
{
	return matrix[person1][person2];
}

char ** parseLine(string line)
{
	int strIndex = 0;
	int strNumber = 0;
	int numberIndex = 0;

	char* tempCStr = 0;
	char** numbers = new char*[NumberOfSeats];
	for(int i = 0; i < NumberOfSeats; i++)
	{
		numbers[i] = new char[10];
	}

	tempCStr = new char[line.length() + 1];
	strcpy(tempCStr, line.c_str());

	while(strNumber < NumberOfSeats)
	{
		strIndex = 0;
		while(tempCStr[numberIndex] != ' ' && tempCStr[numberIndex] != '\0')
		{
			numbers[strNumber][strIndex] = tempCStr[numberIndex];
			strIndex++;
			numberIndex++;
		}

		if(tempCStr[numberIndex] == '\0')
			break;
		numberIndex++;
		numbers[strNumber][strIndex + 1] = '\0';
		strNumber++;
	}

	return numbers;

}



int** readFile(string fileName)
{

	cout << "Opening file" << fileName << endl;
	ifstream instanceFile(fileName.c_str());

	string line("");
	
	char* tempCharArray = 0;
	char* token = 0;
	int row = 0;
	int val = 0;
	int seats = 1;
	bool start = true;
	
	int** matrix;
	
	char** lineOfParsedStrings;

	if(instanceFile.is_open())
	{
		while(instanceFile.good())
		{

			if(start)
			{
				getline(instanceFile, line);
				token = new char[line.length()+1];
				strcpy(token, line.c_str());
				sscanf(token, "%d", &seats);
				NumberOfSeats = seats;
				start = false;
				matrix = new int*[seats];
				for(int i = 0; i < seats; i++)
				{
					matrix[i] = new int[NumberOfSeats];
				}
				for( int i = 0 ; i < NumberOfSeats; i++)
				{
					for ( int j = 0; j < NumberOfSeats; j++)
					{
						matrix[i][j] = 0;
					}
				}


			}

			getline(instanceFile, line);
			lineOfParsedStrings = parseLine(line);

			tempCharArray = new char[line.length() + 1];
			if(tempCharArray)
			{
				strcpy(tempCharArray, line.c_str());
			}

			else
			{
				exit(1);
			}
			for(int i = 0; i < NumberOfSeats; i++)
			{
				sscanf(lineOfParsedStrings[i], "%d", &val);
				matrix[row][i] = val;
			}

			row++;

			if(seats == row)
				break;

		}
		
		cout << "Closing file" << endl;
		
	} else
	{
		cout << "Could not open file" << endl; 
	}

	if(instanceFile.is_open())
		instanceFile.close();


	return matrix;
	

}


