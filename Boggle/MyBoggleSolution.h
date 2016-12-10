#pragma once

#include <vector>
#include <string>

using namespace std;

struct Results
{
	const char* const* Words;    // pointers to unique found words, each terminated by a non-alpha char
	unsigned           Count;    // number of words found
	unsigned           Score;    // total score
	void*              UserData; // ignored
};

// defining the structure of a graph
struct Node;
struct Arc
{
	Node *node = nullptr;
	bool isVisited = false;
};

struct Node
{
	char data;						// data in the board's cell
	bool isVisted = false;			// if the node is visited
	Arc right, left, up, down, ru, rd, lu, ld;	// links from one node to another
	static const int arcCount = 8;
	Arc *arcs[arcCount] = {&right,&left,&up,&down,&ru,&rd,&lu,&ld};
};


class MyBoggleSolution
{
public:
	MyBoggleSolution();
	~MyBoggleSolution();

	// input dictionary is a file with one word per line
	void LoadDictionary(const char* path);

	// this func may be called multiple times
	// board: will be exactly width * height chars, and 'q' represents the 'qu' Boggle cube
	Results FindWords(char** board, unsigned width, unsigned height); 
	void FreeWords(Results &results);
	void FreeDictionary();
	// create a board of width and height
    void CreateBoard(char **board, const unsigned width, const unsigned height);

private:
	vector<string> dictionaryFull;
	vector<string> dictionary;
	string boardChars;
	// return a node which is not visited yet
	Node* GetNotVistedNode(Node *currentNode, const string &word);
	void InitBoardNodes(Node **boardRoot, unsigned width, unsigned height);
	// if a word starting with specific chars exists in the dictionary
	bool IsWordStartExist(const string chars);
	// remove words wich do not contains chars from the board
	void FilterDictionary();
	void FreeBoard(char **board, const unsigned height);
};

