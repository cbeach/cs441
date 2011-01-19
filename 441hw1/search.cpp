#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


using namespace std;

int NumberOfSeats;

int** readFile(string);
int*  shuffle();
int*  searchLoop(int**);
int scoreTable(int* table, int** matrix);
int scoreChangedPair(int* table, int** matrix, int* pair);
int partialScore(int* table, int** matrix, int person);
int pairScore(int, int, int**);
char** parseLine(string);  //because strtok is a steaming pile!

int main(int argc, char** argv)
{

	srand(time(NULL));
	
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
	
//	searchLoop(matrix);

	int table[10] = {0,1,2,3,4,5,6,7,8,9};
	cout << "Test table score = " << scoreTable(table, matrix) << endl;


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
			if(deck[i] > deck[currentHigh])
				currentHigh = i;
		}
		shuffledDeck[i] = currentHigh;
	}
	return shuffledDeck;
}


int* searchLoop(int** matrix)
{
	int* tables[128];
	int elapsedTime = 0;
	int tableScores[128];
	int pair[2] = {-1, -1};

	time_t start, end;

	time(&start);

	for(int i = 0; i < 128; i++)
	{
		tables[i] = shuffle();
		tableScores[i] = scoreTable(tables[i], matrix);
	}

	cout << "Entering Search Loop. Count-down has begun..." << endl;

	while(elapsedTime < 10)
	{
		




		time(&end);
		elapsedTime = difftime(end, start);
	}
	cout << "60 seconds have passed" << endl;
}

int scoreTable(int* table, int** matrix)
{
	int score = 0;
	int halfOfSeats = NumberOfSeats / 2;
	for(int i = 0; i < halfOfSeats; i++)
	{
		if(i <= halfOfSeats - 1)
		{
			score += pairScore(table[i], table[i + 1], matrix); //person i and 1 to the left
			score += pairScore(table[i + 1], table[i], matrix);//vice versa

			score += pairScore(table[i + halfOfSeats], table[i + halfOfSeats + 1], matrix);//now on the other side
			score += pairScore(table[i + halfOfSeats + 1], table[i + halfOfSeats], matrix);

			if(table[i] < halfOfSeats && table[i + 1] > halfOfSeats ||
			   table[i] > halfOfSeats && table[i + 1] < halfOfSeats)
				score += 1;

		}
		if(table[i] < halfOfSeats && table[i + halfOfSeats] > halfOfSeats ||
		   table[i] > halfOfSeats && table[i + halfOfSeats] < halfOfSeats)
			score += 2;

		score += pairScore(table[i + halfOfSeats], table[i], matrix);
		score += pairScore(table[i], table[i + halfOfSeats], matrix);

	}	
	
	return score;
}

int scoreChangedPair(int* table, int** matrix, int* pair)
{



}


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

int pairScore(int person1, int person2, int ** matrix)
{
	cout << person1 << " : " << person2 << " | " << matrix[person1][person2] << endl;
	return matrix[person1][person2];
}

char ** parseLine(string line)
{
	int tokens = 0;
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
			return numbers;
		numberIndex++;
		numbers[strNumber][strIndex + 1] = '\0';
		strNumber++;
	}
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
					matrix[i] = new int[val];
				}
			}

			getline(instanceFile, line);
			cout << "line = " << line << endl;
			lineOfParsedStrings = parseLine(line);	
			tempCharArray = new char[line.length() + 1];
			if(tempCharArray)
			{
				strcpy(tempCharArray, line.c_str());
			}

			else
				exit(1);
			
			for(int i = 0; i < seats; i++)
			{
				sscanf(lineOfParsedStrings[i], "%d", &val);
				matrix[row][i] = val;
			}

			row++;
			if(seats == row)
				break;

		}
		
		cout << "Closing file" << endl;
		
	}
	
	else cout << "Could not open file" << endl;


	if(instanceFile.is_open())
		instanceFile.close();

	return matrix;
	

}


