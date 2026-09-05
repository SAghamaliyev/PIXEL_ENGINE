#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../../dependencies/stb_image.h"
#include <unordered_map>
using namespace std;

class TextureManager {
private:
	unordered_map<unsigned int, unsigned int> TextureList;
	unsigned int
};