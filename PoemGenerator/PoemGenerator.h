// Made by Vladislav Markelov. Use as you wish

#pragma once

#include <string>
#include <vector>

class Rhyme
{
private:

	std::string First;

	std::string Second;

public:

	Rhyme(std::string InFirst, std::string InSecond);

	Rhyme(std::string RhymeToParse);

	Rhyme();

	std::string GetFirst() const { return First; };

	std::string GetSecond() const { return Second; };

};

class RhymesList
{
private:

	std::vector<Rhyme> Rhymes;

public:

	void AddRhyme(Rhyme InRhyme) { Rhymes.push_back(InRhyme); }

	Rhyme GetRhyme(int Index) const { return Index >= int(Rhymes.size()) ? Rhymes[Index] : Rhyme(); }

	Rhyme GetRandomRhyme() const;

	RhymesList();

	RhymesList(std::string RhymeListToParse);

};

class PoemGenerator
{
private:

	std::vector<RhymesList> Lists;

public:

	void AddRhymeList(RhymesList InRhyme) { Lists.push_back(InRhyme); };

	std::vector<int> GetIndexesFromFormat(std::string& Format) const;

	bool IsValidFormat(std::string Format) const;

	std::string GeneratePoem(std::string Format, int CountRows);

	PoemGenerator();

	PoemGenerator(std::vector<std::string> ListsToParse);

};