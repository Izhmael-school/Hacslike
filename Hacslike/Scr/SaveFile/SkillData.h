#pragma once
#include<vector>

class SkillData
{
public:
	std::vector<int> learned;
	std::vector<int> locked;

	void Save(char*& buffer) const;
	void Load(const char*& buffer);

};

