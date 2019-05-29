// Made by Vladislav Markelov. Use as you wish

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "PoemGenerator.h"

namespace fs = std::experimental::filesystem;

int main()
{
	const static std::string RhymesFileEnd = "_Rhymes.txt";

	std::cout << "All lists must end with '" << RhymesFileEnd << "'" << std::endl;
	std::cout << "Available lists is: " << std::endl;

	std::vector<std::string> Lists;
	int ListIndex = 0;

	std::string path = fs::current_path().string();
	for (const auto & entry : fs::directory_iterator(path))
	{
		if (entry.status().type() == fs::file_type::regular)
		{
			std::string path = entry.path().string();
			std::string filename = entry.path().filename().string();

			if (filename.find(RhymesFileEnd) != std::string::npos)
			{
				std::ifstream File;
				File.open(path);

				std::stringstream StrStream;
				StrStream << File.rdbuf();
				std::string CurrentList = StrStream.str();

				Lists.push_back(CurrentList);

				std::cout << "Index: " << ListIndex << ", name: " << filename << std::endl;
				ListIndex++;
			}
		}
	}

	std::cout << "Enter poem format with indexes, for example: 'And {0}, and {0} to {1} from {2}'" << std::endl;
	std::cout << "Format: ";

	std::string Format;
	std::getline(std::cin, Format);

	std::cout << "Your format is '" << Format << "'" << std::endl;
	std::cout << "Your poem is:" << std::endl;

	PoemGenerator Generator(Lists);
	std::cout << Generator.GeneratePoem(Format, 4) << std::endl;

	system("pause");
}