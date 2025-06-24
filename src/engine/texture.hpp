#pragma once
#include "inc.h"

class Texture {
public:
	Texture() = default;
	~Texture() = default;
	bool Load(const fs::path& path);
	public:
	int m_width;
	int m_height;
	int m_numChannels;
	std::vector<unsigned char> m_data;
};