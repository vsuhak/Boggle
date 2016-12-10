#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "CppUnitTest.h"
#include "../Boggle/MyBoggleSolution.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestBoggle
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestFindWords)
		{
			MyBoggleSolution myboggle;
			const char* dictionary_paty = "C:\\Users\\vsu\\Documents\\Visual Studio 2015\\Projects\\Boggle\\TestBoggle\\twl.txt";
			myboggle.LoadDictionary(dictionary_paty);
						unsigned height = 3;
			unsigned width = 3;
			char **board = new char*[height];
			myboggle.CreateBoard(board, width, height);
			board[0][0] = 'd';
			board[0][1] = 'z';
			board[0][2] = 'x';
			board[1][0] = 'e';
			board[1][1] = 'a';
			board[1][2] = 'i';
			board[2][0] = 'q';
			board[2][1] = 'u';
			board[2][2] = 't';
			Results res = myboggle.FindWords(board, width, height);
			vector<string> expectedRes = { "adz", "adze", "ait", "daut", 
				"daze", "eat", "eau", "qua", "quad", "quai", 
				"tad", "tae", "tau", "tax", "taxi", "tui", 
				"tuque", "uta", "zax", "zed", "zit" };
			Assert::IsTrue(expectedRes.size() == res.Count);

			for (unsigned iWords = 0; iWords < res.Count; iWords++)
			{
				Logger::WriteMessage("checking the word: ");
				Logger::WriteMessage(res.Words[iWords]);
				bool found = false;
				for (int jExpect = 0; jExpect < expectedRes.size(); jExpect++)
				{
					if (res.Words[iWords] == expectedRes[jExpect])
					{
						found = true;
						break;
					}
				}				
				Assert::IsTrue(found);
			}
		}
	};
}