#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <regex>
#include "MyBoggleSolution.h"



MyBoggleSolution::MyBoggleSolution()
{
	dictionary = {};
	dictionaryFull = {};
	boardChars = "";
}


MyBoggleSolution::~MyBoggleSolution()
{
	FreeDictionary();
}

void MyBoggleSolution::LoadDictionary(const char * path)
{
	ifstream dict_file(path);	
	if (!dict_file.is_open())
	{
		printf("cannot open the file: %s", path);
		return;
	}
	string line;
	while (getline(dict_file, line))
	{
		dictionaryFull.push_back(line);
	}
#ifdef _DEBUG
	cout << "dictionary size:" << dictionaryFull.size() << endl;
#endif // _DEBUG
}

Results MyBoggleSolution::FindWords(char **board, unsigned width, unsigned height)
{
	// get all chars on from the board
	boardChars.clear();
	for (unsigned h = 0; h < height; h++)
	{
		for (unsigned w = 0; w < width; w++)
		{
			// fill it with chars
			boardChars.push_back(board[h][w]);
			// handling 'q';
			if (board[h][w] == 'q') { boardChars.push_back('u'); }
		}
	}

	// reduce the size of the dictionary
	FilterDictionary();

	// create a data structure behind the board
	// create an array of nodes
	Node **boardRoot = new Node*[height];
	for (unsigned h = 0; h < height; h++)
	{
		boardRoot[h] = new Node[width];
		for (unsigned w = 0; w < width; w++)
		{
			boardRoot[h][w].data = board[h][w];
		}
	}
	// initialise arcs for every node
	for (unsigned h = 0; h < height; h++)
	{
		for (unsigned w = 0; w < width; w++)
		{
			if (w < width-1)
			{
				boardRoot[h][w].right.node = &boardRoot[h][w + 1];
				if (h < height - 1)
				{
					boardRoot[h][w].rd.node = &boardRoot[h + 1][w + 1];
				}
				if (h > 0)
				{
					boardRoot[h][w].ru.node = &boardRoot[h-1][w + 1];
				}
			}
			if (w > 0)
			{
				boardRoot[h][w].left.node = &boardRoot[h][w - 1];
				if (h < height - 1)
				{
					boardRoot[h][w].ld.node = &boardRoot[h + 1][w - 1];
				}
				if (h > 0)
				{
					boardRoot[h][w].lu.node = &boardRoot[h - 1][w - 1];
				}
			}
			if (h < height - 1)
			{
				boardRoot[h][w].down.node = &boardRoot[h+1][w];
			}
			if (h > 0)
			{
				boardRoot[h][w].up.node = &boardRoot[h-1][w];
			}
		}
	}
#ifdef _DEBUG
	cout << "-------------" << endl;
#endif // DEBUG	

	// find a word. Graph traversal
	vector<Node*> breadcrumbs;
	vector<string> wordsFound;
	for (unsigned h = 0; h < height; h++)
	{
		for (unsigned w = 0; w < width; w++)
		{
#ifdef _DEBUG
			cout << "----------------new loop: " << boardRoot[h][w].data << endl;
#endif // DEBUG

			InitBoardNodes(boardRoot,width, height);
			Node *currentNode = &boardRoot[h][w];
			breadcrumbs.push_back(currentNode);
			string word = "";
			word.push_back(currentNode->data);
			// handling 'q'
			if (currentNode->data == 'q') { word.push_back('u'); }
			currentNode->isVisted = true;
#ifdef _DEBUG
			cout << "pushback char. Word: " << word << endl;
#endif // DEBUG

			while (!breadcrumbs.empty())
			{
				currentNode = GetNotVistedNode(currentNode, word);
				// no more not visited nodes
				// dump chars from breadcrumbs
				// remove the last node
				// continue
				if (currentNode == nullptr)
				{
					// if the whole word exist in the dictionary
					// only words with more than 3 chars count.
					if (word.length() >= 3 && binary_search(dictionary.begin(), dictionary.end(), word))
					{
#ifdef _DEBUG
						cout << "-- pushback Word: " << word << endl;
#endif // DEBUG
						// adding only unique words
						if (find(wordsFound.begin(), wordsFound.end(), word) == wordsFound.end())
						{
							wordsFound.push_back(word);
						}
					}
					// remove the last char
					// checking for 'qu', remove an extra char if true
					if (word.size() >1 
						&& word.back() == 'u'
						&& word[word.size()-2] == 'q')
					{
						word.pop_back();
					}
					word.pop_back();
#ifdef _DEBUG
					cout << "removing char:" << breadcrumbs.back()->data << endl;
#endif // DEBUG
					// 'reset' the last visited node and its arcs
					breadcrumbs.back()->isVisted = false;
					for (int i = 0; i < Node::arcCount; i++)
					{
						breadcrumbs.back()->arcs[i]->isVisited = false;
					}
					// remove the last visited node
					breadcrumbs.pop_back();
					
					if (!breadcrumbs.empty())
					{
						currentNode = breadcrumbs.back();
					}
					continue;
				}
				breadcrumbs.push_back(currentNode);
				word.push_back(currentNode->data);
				// handling 'q'
				if (currentNode->data == 'q') { word.push_back('u'); }
				currentNode->isVisted = true;
#ifdef _DEBUG
				cout << "pushback char. Word: " << word << endl;
#endif // DEBUG
			}
		}
	}

	// prepare results
	Results results = {};
	char **wordsptr = new char*[wordsFound.size()];
	for (vector<string>::iterator it = wordsFound.begin(); it != wordsFound.end(); ++it)
	{
		// calculate Score
		switch ((*it).length())
		{
		case 3:
		case 4:
		{
			results.Score++;
			break;
		}
		case 5:
		{
			results.Score += 2;
			break;
		}
		case 6:
		{
			results.Score += 3;
			break;
		}
		case 7:
		{
			results.Score += 5;
			break;
		}
		default:
			results.Score += 11;
		}
		// convert string to char*
		char * sTemp = new char[(*it).length() + 1];
		strcpy_s(sTemp, (*it).length() + 1, (*it).c_str());
		wordsptr[results.Count] = sTemp;			
		results.Count++;
	}
	results.Words = wordsptr;

	//free boardRoot
	for (unsigned i = 0; i < height; i++)
	{
		delete[] boardRoot[i];
	}
	delete[] boardRoot;

	// free dictionary
	dictionary.clear();

	return results;
}

void MyBoggleSolution::FreeWords(Results &results)
{
	for (unsigned i = 0; i < results.Count; i++)
	{
		delete[] results.Words[i];
	}
	delete[] results.Words;
}

void MyBoggleSolution::FreeDictionary()
{
	dictionaryFull.clear();
}


// create a board of width and height
void MyBoggleSolution::CreateBoard(char **board, const unsigned width, const unsigned height)
{
	const char* chars = "abcdefghijklmnopqrstuvwxyz";
	const int charsLength = 26;
	srand((int)time(NULL));

	for (unsigned h = 0; h < height; h++)
	{
		board[h] = new char[width];
		for (unsigned w = 0; w < width; w++)
		{
			// fill it with chars
			char chr = chars[rand() % charsLength];
			board[h][w] = chr;
		}
	}	
}

// if a word starting with specific chars exists in the dictionary
bool MyBoggleSolution::IsWordStartExist(const string chars)
{
	for (int i = 0; i < dictionary.size(); i++)
	{
		if (dictionary[i].find(chars) == 0)
		{
			return true;
		}
	}
	return false;
}

// return a node which is not visited yet
Node * MyBoggleSolution::GetNotVistedNode(Node * currentNode, const string &word)
{
	for (int i = 0; i < Node::arcCount; i++)
	{
		if (currentNode->arcs[i]->node != nullptr 
			&& !(currentNode->arcs[i]->node->isVisted)
			&& !(currentNode->arcs[i]->isVisited))
		{			
			// check if the current path is in the dictionary.
			currentNode->arcs[i]->isVisited = true;
			string wordToLookUp = word + (currentNode->arcs[i]->node->data);
			// handling 'q'
			if (currentNode->arcs[i]->node->data == 'q') { wordToLookUp += 'u'; }
#ifdef _DEBUG
			cout << "analyzing word:" << wordToLookUp << endl;
#endif // DEBUG
			if (IsWordStartExist(wordToLookUp))
			{				
				return currentNode->arcs[i]->node;
			}			
		}
	}
	return nullptr;
}

void MyBoggleSolution::InitBoardNodes(Node **boardRoot, unsigned width, unsigned height)
{
	for (unsigned h = 0; h < height; h++)
	{
		for (unsigned w = 0; w < width; w++)
		{
			boardRoot[h][w].isVisted = false;
			for (int i = 0; i < Node::arcCount; i++)
			{
				boardRoot[h][w].arcs[i]->isVisited = false;
			}
		}
	}
}

// remove words wich do not contains chars from the board
void MyBoggleSolution::FilterDictionary()
{
	dictionary.clear();
	// creating regex string
	string chars = "[" + boardChars + "]*";
#ifdef _DEBUG
	cout << "regex:" << chars << endl;
#endif // DEBUG	
	regex reg(chars);
	for (int i = 0; i < dictionaryFull.size(); i++)
	{
		if (regex_match(dictionaryFull[i], reg))
		{
			dictionary.push_back(dictionaryFull[i]);
		}
	}
#ifdef _DEBUG
	cout << "filtered dictionary size:" << dictionary.size() << endl;
#endif // _DEBUG

}


