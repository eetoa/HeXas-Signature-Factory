#include "stdafx.h"

std::unordered_map<std::string, std::vector<std::string>>	sigMap;
std::unordered_map<std::string, std::string>				ProductMap;


// hexas new group
bool Factory::AddGroup(std::string group)
{
	// group exists
	if (sigMap.count(group))
	{
		return false;
	}
	std::vector<std::string> sigContainer;
	sigMap.insert({ group, sigContainer });
	return true;
}

// hexas group -p cod.exe -a 0x1708E3 -s 0x8
bool Factory::UpdateSigMap(std::string group, std::string sig)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return false;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		// found it
		iter->second.push_back(sig);
	}
	return true;
}

// hexas merge group
bool Factory::MergeSigs(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return false;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		ProductMap.insert(group, Merge(iter->second));
	}
	return true;
}

std::string Merge(std::vector<std::string>)
{
	// TODO: merge algorithm implementaion

}
