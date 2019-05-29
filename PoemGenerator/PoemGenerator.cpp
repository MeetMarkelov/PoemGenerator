// PoemGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "PoemGenerator.h"

Rhyme::Rhyme(std::string InFirst, std::string InSecond) : First(InFirst), Second(InSecond)
{
	//
}

Rhyme::Rhyme(std::string ToParse) : First("First"), Second("Second")
{
	if (ToParse.find(':') != std::string::npos)
	{
		int Delim = ToParse.find(':');
		std::string InFirst = ToParse.substr(0, Delim);
		std::string InSecond = ToParse.substr(Delim + 1, ToParse.size() - Delim);
		if (InFirst.size() > 0)
			First = InFirst;
		if (InSecond.size() > 0)
			Second = InSecond;
	}
	//std::cout << "First: " << First << "; Second: " << Second << std::endl;
}

Rhyme::Rhyme() : First("First"), Second("Second")
{

}

Rhyme RhymesList::GetRandomRhyme() const
{
	static int RandSeed = -1;

	if (Rhymes.size() == 0)
		return Rhyme();
	else if (Rhymes.size() == 1)
		return Rhymes[0];
	else
	{
		RandSeed += 55;
		srand(int(time(NULL)) + RandSeed);
		return Rhymes[rand() % Rhymes.size()];
	}
}

RhymesList::RhymesList()
{

}

RhymesList::RhymesList(std::string ToParse)
{
	std::istringstream StringStream(ToParse);
	std::string CurrentLine;
	while (std::getline(StringStream, CurrentLine)) {
		Rhymes.push_back(Rhyme(CurrentLine));
	}
}

std::vector<int> PoemGenerator::GetIndexesFromFormat(std::string& Format) const
{
	int FindFrom = 0;
	int CurrentIndex = 0;
	std::vector<int> Indexes;
	while (Format.find('{', FindFrom) != std::string::npos)
	{
		int Start = Format.find('{', FindFrom);
		int End = Format.find('}', FindFrom);

		std::string Current = Format.substr(Start + 1, End - Start - 1);

		int ListIndex = std::stoi(Current);
		if (ListIndex < 0 || ListIndex >= Lists.size())
			return std::vector<int>();

		Indexes.push_back(ListIndex);

		//Format.erase(Start + 1, End - Start - 1);
		//Format.insert(Start + 1, std::to_string(CurrentIndex));
		Format.replace(Start + 1, End - Start - 1, std::to_string(CurrentIndex));

		FindFrom = Start + std::to_string(CurrentIndex).length() + 2;
		CurrentIndex++;
		//std::cout << "Index: " << Indexes[Indexes.size() - 1] << std::endl;
	}
	return Indexes;
}

bool PoemGenerator::IsValidFormat(std::string Format) const
{
	std::vector<int> Indexes = GetIndexesFromFormat(Format);
	return Indexes.size() > 0;
}

std::string PoemGenerator::GeneratePoem(std::string Format, int CountRows)
{
	std::vector<int> Indexes = GetIndexesFromFormat(Format);
	if (Indexes.size() <= 0)
		return "ERROR FORMAT";

	std::string Output = "";
	for (int i = 0; i < CountRows / 2; i++)
	{
		std::string TopRow = Format;
		std::string BottomRow = Format;

		int CurrentIndex = 0;
		for (int CurrentList : Indexes)
		{
			std::string ToFind = '{' + std::to_string(CurrentIndex) + '}';
			int Length = ToFind.size();
			Rhyme CurrentRhyme = Lists[CurrentList].GetRandomRhyme();

			static int RandSeed = -1;
			RandSeed += 33;
			srand(int(time(NULL)) + RandSeed);
			if (rand() % 2)
			{
				TopRow.replace(TopRow.find(ToFind), Length, CurrentRhyme.GetFirst());
				BottomRow.replace(BottomRow.find(ToFind), Length, CurrentRhyme.GetSecond());
			}
			else
			{
				TopRow.replace(TopRow.find(ToFind), Length, CurrentRhyme.GetSecond());
				BottomRow.replace(BottomRow.find(ToFind), Length, CurrentRhyme.GetFirst());
			}
			CurrentIndex++;
		}
		Output += TopRow + '\n' + BottomRow + '\n';
	}
	return Output;
}

PoemGenerator::PoemGenerator()
{

}

PoemGenerator::PoemGenerator(std::vector<std::string> ListsToParse)
{
	for (std::string CurrentString : ListsToParse)
	{
		RhymesList CurrentList = RhymesList(CurrentString);
		Lists.push_back(CurrentList);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
