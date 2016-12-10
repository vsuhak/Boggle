// Boggle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "MyBoggleSolution.h"

using namespace std;


int main()
{
	MyBoggleSolution myboggle;

	// load a dictionary
	cout << "Loading the dictionary" << endl;
	const char* dictionary_paty = "C:\\Users\\vsu\\Documents\\Visual Studio 2015\\Projects\\Boggle\\Boggle\\twl.txt";
	myboggle.LoadDictionary(dictionary_paty);
	cout << "Dictionary has been loaded" << endl;

	char continuePlaying = 'y';
	while (continuePlaying == 'y')
	{
		cout << "=======NEW BOARD=======" << endl;
		// create a board
		cout << "input a height of the board" << endl;
		unsigned height;
		cin >> height;
		cout << "input a width of the board" << endl;;
		unsigned width;
		cin >> width;
		
		// create a board (2 dimentional matrix)
		char **board = new char*[height];
		myboggle.CreateBoard(board, width, height);

		// dump the board
		for (unsigned h = 0; h < height; h++)
		{
			for (unsigned w = 0; w < width; w++)
			{
				cout << board[h][w] << ' ';
			}
			cout << endl;
		}

		Results res = myboggle.FindWords(board, width, height);
		//dump results
		cout << "=======RESULTS=======" << endl;
		cout << "Count:" << res.Count << endl;
		cout << "Score:" << res.Score << endl;
		cout << "Words: ";
		for (unsigned i = 0; i < res.Count; i++)
		{
			cout << res.Words[i] << ' ';
		}
		cout << endl;
		
		// free resources
		myboggle.FreeWords(res);

		// free board
		for (unsigned i = 0; i < height; i++)
		{
			delete[] board[i];
		}
		delete[] board;

		cout << "do you want play more? (y/n)" << endl;
		cin >> continuePlaying;
	}

	myboggle.FreeDictionary();
	return 0;
}

