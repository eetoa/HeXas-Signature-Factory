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
		if (!iter->second.size())
			return SigNotFoundError;
		// if product exists, and wanna remerge ,just overwrite with new merge
		productMap[group] = Merge(iter->second);
		/*
		// determin if remerge product
		if (productMap.count(group))
		{
			//exists product
			iter->second.push_back(productMap[group]);
			productMap.insert({ group, Merge(iter->second) });
			iter->second.pop_back();
		}
		else
		{
			productMap.insert({ group, Merge(iter->second) });
		}
		*/
	}
	return Success;
}

std::string Factory::Merge(std::vector<std::string> sigs)
{
	// at this point , sigs container has at least one item
	std::string res = "";
	int rows = sigs.size();
	if (rows == 1)
		return sigs[0];

	int min = 1e6 + 10;
	for (int r = 0; r < rows; r++)
	{
		min = sigs[r].length() < min ? sigs[r].length() : min;
	}

	for (int c = 0; c < min; c++)
	{
		char cmp = sigs[0][c];
		bool isMatch = false;
		for (int r = 1; r < rows; r++)
		{
			if (sigs[r][c] != cmp)
			{
				isMatch = false;
				break;
			}
			else
			{
				isMatch = true;
			}

		}
		res += isMatch ? cmp : '?';
	}
	return res;
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
		std::cout << dye::on_light_purple(iter->second);
	}
	std::cout << std::endl << std::endl;

	return Success;
}
