#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include "../Logger/Logger.h"

using namespace std;

class AssetSystem {
public:
	void Run();	// Check if some file needs parsing and also add every file in hashmap
	void RegisterFile(string path);	// It moves file to its inner file system

private:
	void MakeFileMeta(string path);	// Creates its .meta(where ID)
	void ParseFile(const unsigned long long int ID,const string relativePath);	// Parsing of file and putting it in .bin file

	unordered_map<unsigned long long int, string> PathRegistry;
};