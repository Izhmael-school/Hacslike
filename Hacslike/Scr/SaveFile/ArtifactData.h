#pragma once
#include<vector>

class ArtifactData
{
public:
	std::vector<int> obtained;
	std::vector<int> notobtained;

	void Save(char*& buffer) const;
	void Load(const char*& buffer);
};

