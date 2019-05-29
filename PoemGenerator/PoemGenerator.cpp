// Made by Vladislav Markelov. Use as you wish

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
		int Delim = (int)ToParse.find(':');
		std::string InFirst = ToParse.substr(0, Delim);
		std::string InSecond = ToParse.substr(Delim + 1, ToParse.size() - Delim);
		if (InFirst.size() > 0)
			First = InFirst;
		if (InSecond.size() > 0)
			Second = InSecond;
	}
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
		int Start = (int)Format.find('{', FindFrom);
		int End = (int)Format.find('}', FindFrom);

		std::string Current = Format.substr(Start + 1, End - Start - 1);

		int ListIndex = std::stoi(Current);
		if (ListIndex < 0 || ListIndex >= Lists.size())
			return std::vector<int>();

		Indexes.push_back(ListIndex);

		Format.replace(Start + 1, End - Start - 1, std::to_string(CurrentIndex));

		FindFrom = Start + (int)std::to_string(CurrentIndex).length() + 2;
		CurrentIndex++;
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
			int Length = (int)ToFind.size();
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