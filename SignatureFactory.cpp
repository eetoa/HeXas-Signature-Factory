#include "stdafx.h"
#include "color.hpp"

std::unordered_map<std::string, std::vector<std::string>>	Factory::sigMap;
std::unordered_map<std::string, std::string>				Factory::productMap;


// ls -g
Status Factory::ListGroup()
{
	if (sigMap.empty())
	{
		//std::cout << "No Group Exist." << std::endl << std::endl;
		return NoneGroup;
	}
	int i = 0;
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
	{
		std::cout << dye::on_light_aqua(iter->first) << std::endl;
		i++;
	}
	std::cout << i << " in total";
	std::cout << std::endl;
	return Success;
}

// show -all
Status Factory::ShowAllGroup()
{
	if (sigMap.empty())
	{
		//std::cout << "No Group Exist." << std::endl << std::endl
		return NoneGroup;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
	{
		std::cout << dye::on_light_aqua(iter->first) << dye::on_light_aqua(" : ") << std::endl;
		for (int i = 0; i < iter->second.size(); i++)
		{
			std::cout << dye::on_light_blue(iter->second[i]) << std::endl;
		}
	}
	std::cout << std::endl;
	return Success;
}

// merge -all
Status Factory::MergeAllGroup()
{
	if (sigMap.empty())
	{
		return NoneGroup;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
	{
		productMap.insert({ iter->first, Merge(iter->second) });
	}
	return Success;
}

// get -all
Status Factory::GetAllProduct()
{
	if (productMap.empty())
	{
		return NoneGroup;
	}
	std::unordered_map<std::string, std::string>::iterator iter;
	for (iter = productMap.begin(); iter != productMap.end(); iter++)
	{
		std::cout << dye::on_light_aqua(iter->first) << dye::on_light_aqua(" : ") << dye::on_light_purple(iter->second) << std::endl;
	}
	std::cout << std::endl;
	return Success;
}

// new group
Status Factory::AddGroup(std::string group)
{
	// group exists
	if (sigMap.count(group))
	{
		return GroupExistsError;
	}
	std::vector<std::string> sigContainer;
	sigMap.insert({ group, sigContainer });
	return Success;
}

// show group
Status Factory::ShowGroup(std::string group)
{
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		// found it
		for (int i = 0; i < iter->second.size(); i++)
		{
			std::cout << dye::on_light_blue(iter->second[i]) << std::endl;
		}
		std::cout << std::endl;
		return Success;
	}
	else
	{
		return GroupNotFoundError;
	}
}

Status Factory::GenerateSig(std::string group)
{
	Proc::GetProcId();
	Proc::GetProcHandle();
	std::cout << g_procId << " - " << g_procHandle << std::endl;

	std::vector<byte> res;
	for (unsigned int i = 0; i < g_size; i++) {
		byte cur;
		res.push_back(ReadEx<byte>(g_address + i));
	}
	/*
	for (int i = 0; i < res.size(); i++) {
		std::cout << std::hex << dye::on_light_blue((unsigned int)res[i]) << dye::on_light_blue(" ");
	}
	std::cout << std::endl << std::endl;
	*/
	std::string sig = "";
	for (int i = 0; i < res.size(); i++) {
		std::string unit = Utils::ToHexString((unsigned int)res[i]);;
		if (unit.length() == 1) unit = "0" + unit;
		sig += unit;
		sig += " ";
	}
	std::cout << dye::on_light_blue(sig);
	std::cout << std::endl << std::endl;

	if (!UpdateSigMap(group, sig))
	{
		return GroupNotFoundError;
	}
	return Success;
}

// group -p cod.exe -a 0x1708E3 -s 0x8
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

// merge group
Status Factory::MergeGroup(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return GroupNotFoundError;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		productMap.insert({ group, Merge(iter->second) });
	}
	return Success;
}

std::string Factory::Merge(std::vector<std::string> sigs)
{
	// TODO: merge algorithm implementaion
	return "";
}

// get group
Status Factory::GetProduct(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return GroupNotFoundError;
	}
	std::unordered_map<std::string, std::string>::iterator iter;
	iter = productMap.find(group);
	if (iter != productMap.end())
	{
		std::cout << std::hex << iter->second << " ";
	}
	return Success;
}
